#include "OffloadSDS.h"

namespace Halide {
namespace Internal {

    using std::vector;
    using std::make_pair;
    using std::pair;
    using std::string;
    using std::map;
    using std::set;

    namespace {
        /* Giving a string like func.s{stage}.var, returning {stage} */
        int get_stage(const string &s) {
            for (size_t i = 1; i < s.size(); ++i) {
                if (s[i - 1] == '.' && s[i] == 's') {
                    size_t r = i + 1;
                    int res = 0;
                    while (r < s.size() && s[r] >= '0' && s[r] <= '9') {
                        res = res * 10 + s[r] - '0';
                        ++r;
                    }
                    return res;
                }
            }
            internal_assert(false) << s << " is not a stage variable!\n";
            return -1;
        }

        /* Giving a string like func.s{stage}.var, returning {func} */
        string strip_stage(const string &s) {
            for (size_t i = 0; i < s.size(); ++i) {
                if (s[i] == '.') {
                    return s.substr(0, i);
                }
            }
            internal_assert(false) << s << " is not a stage variable!\n";
            return "";
        }

        /* Giving a string like func.s{stage}.var, returning {func}.s{stage}. */
        string get_prefix(const string &s) {
            return strip_stage(s) + ".s" + std::to_string(get_stage(s)) + ".";
        }

        /* In this stage, we need a very aggressive constant propagation, but we cannot actually do it, which may cause
         * strange error in later lowering stage (some constant propagation will not be done correctly). Thus, we can
         * only expand the expressions when necessary. */
        class ExpandExpr : public IRMutator {
            using IRMutator::visit;
            const Scope<Expr> &scope;

            void visit(const Variable *var) {
                if (scope.contains(var->name)) {
                    //expr = scope.get(var->name); is not fully expanded variable!
                    //use mutate method to fully expand it!
                    expr = mutate(scope.get(var->name));
                } else {
                    expr = var;
                }
            }

            void visit(const Call *call) {
                if (call->is_intrinsic(Call::likely) || call->is_intrinsic(Call::likely_if_innermost)) {
                    internal_assert(call->args.size() == 1);
                    expr = mutate(call->args[0]);
                } else {
                    IRMutator::visit(call);
                }
            }

        public:
            ExpandExpr(const Scope<Expr> &s) : scope(s) {}

        };

        //The expression expanding helper function
        Expr expand_expr(Expr e, const Scope<Expr> &scope) {
            ExpandExpr ee(scope);
            Expr result = ee.mutate(e);
            return result;
        }

        struct Stencil {
            string consumer;
            vector <Expr> image_mins;

            /* This is not calculated when analyzing. After analysis, it will be calculated when checking
             * if this stencil is eligible. */
            //@{
            vector<int> consumed_mins;
            //@}

            /* Maybe I should use Region to represent these bunch of stuffs. Just because there are a lot of constants,
             * I decided to write them in this fashion.
             */
            //@{
            vector<int> image_bounds;
            vector <Expr> stencil_mins;
            vector<int> stencil_bounds;
            //@}

            /* NOTE: movement is the step of each dim of the stencil and traverse represents the corresponding iteration
             * loop name. However, scan level is not either end of traverse, because some stencil does not move (like the
             * weight in convolution NN).
             */
            vector<int> movement;
            vector <string> traverse;
            string scan_level;
            bool is_param;
            Type type;

            Stencil(const string &consumer,
                    const Box &consumed_box,
                    const Box &stencil_box,
                    const vector<const For *> &traverse_loop) : consumer(consumer), movement(stencil_box.size(), 0),
                                                                traverse(stencil_box.size(), "<NonSerial>"),
                                                                scan_level(traverse_loop.back()->name),
                                                                is_param(false) {
                internal_assert(consumed_box.size() == stencil_box.size())
                        << "Consumed and stencil should have the same dimension!\n";
                for (size_t i = 0; i < consumed_box.size(); ++i) {
                    image_mins.push_back(consumed_box[i].min);
                    Expr image_extent = simplify(consumed_box[i].max - consumed_box[i].min + 1);
                    internal_assert(is_const(image_extent)) << "Image consumed should have constant bounds!\n";
                    image_bounds.push_back((int) *as_const_int(image_extent));

                    stencil_mins.push_back(stencil_box[i].min);
                    Expr stencil_extent = simplify(stencil_box[i].max - stencil_box[i].min + 1);
                    internal_assert(is_const(stencil_extent)) << "Stencil should have constant bounds!\n";
                    stencil_bounds.push_back((int) *as_const_int(stencil_extent));
                }
                internal_assert(movement.size() == stencil_mins.size()) << "Movement init failed?!\n" << movement.size()
                                                                        << " != " << stencil_mins.size() << "\n";
                for (const For *loop : traverse_loop) {
                    for (size_t i = 0; i < stencil_mins.size(); ++i) {
                        const Expr &expr = stencil_mins[i];
                        if (expr_uses_var(expr, loop->name)) {
                            Expr stride = simplify(expr - substitute(loop->name, Var(loop->name) - 1, expr));
                            internal_assert(is_const(stride)) << "Stencil should have constant stride of movement!\n";
                            internal_assert(i < movement.size()) << i << " >= " << movement.size() << "\n";
                            movement[i] = (int) *as_const_int(stride);
                            traverse[i] = loop->name;
                        }
                    }
                }
            }

            /* If the stencil does not move, we need to fully partition this array */
            bool fully_partition_required() const {
                for (int i : movement) {
                    if (i) {
                        return false;
                    }
                }
                return true;
            }

            /* If the stencil is only one value, we do not need a linebuffer */
            bool only_one() const {
                for (int i : stencil_bounds) {
                    if (i != 1) {
                        return false;
                    }
                }
                return true;
            }
        };

        /* Functions for the name of hls templates. */
        //@{
        string append_tail(const string &prefix, const string &suffix) {
            return prefix + (prefix.back() == '.' ? suffix : "." + suffix);
        }

        string make_window_name(const string &stage) {
            return append_tail("partitioned." + stage, "buffer");
        }

        string make_window_name(const string &producer, const string &consumer) {
            return append_tail(append_tail(producer, "to.") + consumer, "stencil");
        }

        string make_iter_of_distributor(const string &prefix, const int &dim) {
            return append_tail("distributor." + prefix, std::to_string(dim));
        }

        string make_stream_name(const string &producer, const string &consumer) {
            return append_tail(append_tail(producer, "to.") + consumer, "stream");
        }

        string make_linebuffer_name(const string &producer, const string &consumer) {
            return append_tail(append_tail(producer, "to.") + consumer, "linebuffer");
        }
        //@}

        /* Functions for distribution input parameters to each's consumers. */
        //@{
        Stmt make_distributor(Type type, const string &producer, const Stencil &stencil) {
            Expr condition;
            for (size_t i = 0; i < stencil.consumed_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                Expr cur_cond = iter_var >= Expr(stencil.consumed_mins[i]) &&
                                iter_var < Expr(stencil.consumed_mins[i] + stencil.image_bounds[i]);
                condition = condition.defined() ? condition && cur_cond : cur_cond;
            }
            Expr stream_name = Expr(make_stream_name(producer, stencil.consumer));
            Expr holder = Call::make(type, Call::sds_single_holder, {Expr(producer)}, Call::CallType::Intrinsic);
            Expr sender = Call::make(Type(), Call::sds_stream_write, {stream_name, holder}, Call::CallType::Intrinsic);
            return IfThenElse::make(condition, Evaluate::make(sender));
        }

        //TODO: later I don't need it!
        Stmt make_partitioner(Type type, const string &producer, const Stencil &stencil) {
            vector <Expr> args{Expr(make_window_name(producer))};
            for (size_t i = 0; i < stencil.consumed_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                args.push_back(iter_var);
            }
            Expr holder = Call::make(type, Call::sds_single_holder, {Expr(producer)}, Call::CallType::Intrinsic);
            vector <Expr> window_args(args.begin() + 1, args.end());
            std::reverse(window_args.begin(), window_args.end());
            window_args.push_back(holder);
            window_args.insert(window_args.begin(), args[0]);
            return Evaluate::make(
                    Call::make(type, Call::sds_windowbuffer_access, window_args, Call::CallType::Intrinsic));
        }

        Stmt make_holder(Type type, const string &producer, const Stencil &stencil) {
            vector <Expr> args{Expr(producer)};
            for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                args.push_back(iter_var);
            }
            return Evaluate::make(Call::make(type, Call::sds_single_holder, args, Call::CallType::Intrinsic));
        }
        //@}

       Expr get_stream_depth(const string &producer, const string &consumer, const map<string, Function> &env) {
           string consumer_name = strip_stage(consumer);
           string producer_name = producer.back() == '.' ? strip_stage(producer) : producer;
           const map<string, int> &depth_map = env.find(consumer_name)->second.schedule().depth_of_streams();

           debug(5) << depth_map.size() << " depths specified\n";
           for (auto iter : depth_map) {
               debug(5) << iter.first << ", " << iter.second << "\n";
           }
           debug(5) << "\n";

           if ((int) (env.find(consumer_name)->second.updates().size()) != get_stage(consumer)) {
               debug(4) << consumer << " is not the last stage of " << consumer_name << ", "
                        << "so the depth is 1\n";
               return Expr(1);
           }
           if (depth_map.find(producer_name) == depth_map.end()) {
               debug(4) << "Depth between <" << producer << ", " << consumer << "> is not specified, "
                        << "so the depth is 1\n";
               return Expr(1);
           }
           debug(4) << "Depth between <" << producer << ", " << consumer << "> is "
                    << depth_map.find(producer_name)->second << "\n";
           return Expr(depth_map.find(producer_name)->second);
        }
    }

    /* A producer-consumer relationship inside the dependency graph. */
    struct HWStageEdge {
        string producer;
        Stencil stencil;

        HWStageEdge(const string &producer, const Stencil &stencil) :
                producer(producer), stencil(stencil) {}

        string pure_name() const {
            return stencil.is_param ? producer : strip_stage(producer);
        }
    };

    struct StencilAnalyzer : IRVisitor {
		using IRVisitor::visit;

        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            IRVisitor::visit(let);
            lets.pop(let->name);
        }

        void visit(const For *op) {
            bounds.push(op->name,
                        Interval(op->min, simplify(expand_expr(op->min + op->extent - 1, lets), false, bounds)));
            for_stack.push_back(op);
            IRVisitor::visit(op);
            for_stack.pop_back();
            bounds.pop(op->name);
        }

        void visit(const Provide *provide) {
            if (env.find(provide->name) != env.end()) {
                vector<const For *> traverse_loop;
                int pre_dim = for_stack.size() + 1;
                //FIXME: the code here may need refactor later, cuz the nested loops make the result of traverse loop from outer to the inner, which conflicts the design concept of halide
                for (const For *loop : for_stack) {
                    int depend_cnt = 0;
                    for (size_t i = 0; i < provide->args.size(); ++i) {
                        if (expr_uses_var(provide->args[i], loop->name)) {
                            if (loop->for_type == ForType::Serial) {
                                user_assert(++depend_cnt <= 1)
                                        << Stmt(provide)
                                        << "\nMore than one dimension depend on the traverse loop, abort!\n";
                                user_assert(pre_dim > (int) i)
                                        << Stmt(provide)
                                        << "\nShould scan the dimension from outer to inner!\n"
                                        << pre_dim << " < " << i << "\n";
                                traverse_loop.push_back(loop);
                                pre_dim = i;
                            } else {
                                internal_assert(loop->for_type == ForType::Unrolled)
                                                << "Vectorized dim's iterative variable should be ForType::Unrolled!\n";
                            }
                        }
                    }
                }

                string producer_name = is_input_param ? producer : strip_stage(producer);
                string consumer_prefix = get_prefix(traverse_loop[0]->name);
                if (!is_input_param) {
                    internal_assert(env.find(producer_name) != env.end()) << producer_name
                                                                                            << " is not an offloaded stage!\n";
                    if (producer_name == strip_stage(consumer_prefix)) {
                        debug(3) << "Update of " << producer_name << " detected...\n";
                        if (get_stage(producer) + 1 != get_stage(consumer_prefix)) {
                            debug(3) << get_stage(producer) << " and " << get_stage(consumer_prefix)
                                     << " are not adjacent stages!\n";
                            if (get_stage(producer) == get_stage(consumer_prefix)) {
                                //It is somewhat shitty, but I have no choice
                                debug(3) << "Under " << producer_name
                                         << "'s own computation loop, so update the traverse loop!\n";
                                vector <string> traverse_loop_names;
                                for (vector<const For *>::reverse_iterator riter = traverse_loop.rbegin();
                                     riter != traverse_loop.rend(); ++riter) {
                                    traverse_loop_names.push_back((*riter)->name);
                                }
                                traverse_collection[consumer_prefix] = traverse_loop_names;
                            }
                            return;
                        }
                    } else if (((int) env.find(producer_name)->second.updates().size()) != get_stage(producer)) {
                        debug(3) << producer << " is not the last update stage of " << producer_name << "!\n";
                        debug(3) << "Only " << get_stage(producer) + 1 << " of " << producer_name
                                 << " is expected...\n";
                        return;
                    }
                }
                Box stencil_box = box_required(traverse_loop.back()->body, producer_name);
                if (stencil_box.size() != 0) {
                    debug(3) << get_prefix(traverse_loop.front()->name) << " consumes " << producer << "\n";
                    debug(3) << Stmt(provide) << "\n";
                    debug(3) << "Traverse loop(s):\n";
                    for (auto loop : traverse_loop) {
                        debug(3) << loop->name << "\n";
                    }
                    debug(3) << "\n";
                    Box consumed_box = box_required(Stmt(traverse_loop[0]),
                                                    is_input_param ? producer : strip_stage(producer));
                    for (size_t i = 0; i < consumed_box.size(); ++i) {
                        consumed_box[i].min = simplify(expand_expr(consumed_box[i].min, lets), false, bounds);
                        consumed_box[i].max = simplify(expand_expr(consumed_box[i].max, lets), false, bounds);
                    }
                    for (size_t i = 0; i < stencil_box.size(); ++i) {
                        stencil_box[i].min = simplify(expand_expr(stencil_box[i].min, lets), false, bounds);
                        stencil_box[i].max = simplify(expand_expr(stencil_box[i].max, lets), false, bounds);
                    }
                    res.push_back(Stencil(consumer_prefix, consumed_box, stencil_box, traverse_loop));
                    vector <string> traverse_loop_names;
                    for (vector<const For *>::reverse_iterator riter = traverse_loop.rbegin();
                         riter != traverse_loop.rend(); ++riter) {
                        traverse_loop_names.push_back((*riter)->name);
                    }
                    traverse_collection[consumer_prefix] = traverse_loop_names;
                }
            }
        }

        string producer;
        const map<string, Function> &env;
        bool is_input_param;
        vector<const For *> for_stack;
        Scope<Expr> lets;
        Scope<Interval> bounds;
        map <string, vector<string>> traverse_collection;

        vector <Stencil> res;

        StencilAnalyzer(const string &producer, const string &func, const map<string, Function> &env,
                        bool is_input_param)
                : producer(producer), env(env), is_input_param(is_input_param) {}

    };

    vector <Stencil>
    analyze_stencil(Stmt s, const string &producer, const string &offload_func, const map<string, Function> &env,
                    bool is_input_param, map <string, vector<string>> &traverse_collection) {
        StencilAnalyzer collector(producer, offload_func, env, is_input_param);
        s.accept(&collector);
        for (Stencil &stencil : collector.res) {
            stencil.is_param = is_input_param;
        }
        for (const pair <string, vector<string>> &iter : collector.traverse_collection) {
            if (traverse_collection.find(iter.first) == traverse_collection.end()) {
                traverse_collection[iter.first] = iter.second;
            } else {
                vector <string> &another = traverse_collection[iter.first];
                internal_assert(another.size() == iter.second.size()) << "Traverse loop should be all the same!\n";
                for (size_t i = 0; i < iter.second.size(); ++i) {
                    internal_assert(another[i] == iter.second[i]) << "Traverse loop should be all the same!\n";
                }
            }
        }
        return collector.res;
    }

    struct OffloadPruner : public IRMutator {
        using IRMutator::visit;

        //Inside the offloaded body, we do not need loop partition, but loop perfection.
        /*void visit(const Call *call) {
            if (call->is_intrinsic(Call::likely) || call->is_intrinsic(Call::likely_if_innermost)) {
                internal_assert(call->args.size() == 1);
                expr = mutate(call->args[0]);
            } else {
                expr = call;
            }
        }

        void visit(const Provide *provide) {
            vector <Expr> values(provide->values);
            for (Expr &expr : values) {
                expr = simplify(mutate(expr), false, bounds);
            }
            stmt = Provide::make(provide->name, values, provide->args);
        }*/

        void visit(const For *loop) {
            Stmt body = mutate(loop->body);
            Expr loop_min = mutate(loop->min);
            Expr loop_extent = simplify(expand_expr(loop->extent, lets), false, bounds);
            bounds.push(loop->name, Interval(simplify(expand_expr(loop_min, lets), false, bounds),
                                             simplify(expand_expr(loop_min + loop_extent - 1, lets), false, bounds)));
            internal_assert(is_const(loop_extent))
                    << "Among Offloaded body, all of the loop supposed to be fix bounded...\n";
            if (*as_const_int(loop_extent) == (int) 0) {
                stmt = Evaluate::make(Expr(0));
                return;
            }
            /* In certain version, simplify trivial loops will cause some over propagated constant,
             * which cannot be simplified in later lowering stage, so I choose not to simplify trivial
             * loops here!
             *else if (*as_const_int(loop->extent) == (int) 1){
             *  stmt = substitute(loop->name, loop_min, body);
             *  return ;
             *}*/
            if (body.same_as(loop->body) && loop_extent.same_as(loop->extent)) {
                stmt = loop;
            } else {
                stmt = For::make(loop->name, loop->min, loop_extent, loop->for_type, loop->device_api, body);
            }
            bounds.pop(loop->name);
        }

        void visit(const LetStmt *let) {
            lets.push(let->name, simplify(expand_expr(mutate(let->value), lets), false, bounds));
            IRMutator::visit(let);
            lets.pop(let->name);
        }

        Scope<Expr> &lets;
        Scope<Interval> &bounds;

        OffloadPruner(Scope<Expr> &lets, Scope<Interval> &bounds) : lets(lets), bounds(bounds) {}
    };

    struct OffloadInputs : public IRVisitor {
        using IRVisitor::visit;

        void visit(const ProducerConsumer *pc) {
            if (pc->is_producer) {
                already.insert(pc->name);
            }
            IRVisitor::visit(pc);
        }

        void visit(const Call *call) {
            if (already.find(call->name) == already.end() &&
                (call->call_type == Call::Halide || call->call_type == Call::Image)) {
                res.insert(call->name);
            }
        }

        OffloadInputs(const string &offload_func) {
            already.insert(offload_func);
        }

        void visit(const For *loop) {
            bounds.push(loop->name, Interval(simplify(expand_expr(loop->min, lets), false, bounds),
                                             simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            IRVisitor::visit(loop);
        }

        void visit(const LetStmt *let) {
            lets.push(let->name, simplify(expand_expr(let->value, lets), false, bounds));
            IRVisitor::visit(let);
        }

        Scope<Expr> lets;
        Scope<Interval> bounds;
        set <string> already;
        set <string> res;

        OffloadInputs() {}
    };

    map <string, Box> offload_inputs(Stmt s, const string &offload_func) {
        OffloadInputs collector(offload_func);
        s.accept(&collector);
        map <string, Box> res;
        for (const string &input : collector.res) {
            res[input] = box_required(s, input);
            Box &box = res[input];
            for (size_t i = 0; i < box.size(); ++i) {
                box[i].min = simplify(expand_expr(box[i].min, collector.lets), false, collector.bounds);
                box[i].max = simplify(expand_expr(box[i].max, collector.lets), false, collector.bounds);
            }
        }
        return res;
    }

    /* This is somewhat foolish. Maybe later I do not need it. */
    struct TypeOfCallOrProvide : IRVisitor {
		using IRVisitor::visit;

        void visit(const Call *call) {
            if (call->name == call_name) {
                if (!found) {
                    type = call->type;
                    found = true;
                } else {
                    internal_assert(call->type == type);
                }
            }
            IRVisitor::visit(call);
        }

        void visit(const Provide *provide) {
            if (provide->name == call_name) {
                if (!found) {
                    type = provide->values[0].type();
                    found = true;
                } else {
                    internal_assert(type == provide->values[0].type());
                }
            }
            IRVisitor::visit(provide);
        }

        string call_name;
        bool is_param;
        Type type;
        bool found;

        TypeOfCallOrProvide(const string &s, bool is_param) : call_name(s), is_param(false), found(false) {
            if (!is_param) {
                call_name = strip_stage(call_name);
            }
        }
    };

    Type type_of_call_or_provide(Stmt s, const string &name, bool is_param) {
        TypeOfCallOrProvide visitor(name, is_param);
        debug(3) << "Infer type of " << name << "\n";//" in \n" << s << "\n";
        s.accept(&visitor);
        if (visitor.found) {
            debug(3) << "Final result " << visitor.type << "\n";
        } else {
            internal_assert(false) << name << "'type not found in this body!\n";
        }
        return visitor.type;
    }

    struct BoxOfStage : public IRVisitor {
		using IRVisitor::visit;

        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            IRVisitor::visit(let);
            lets.pop(let->name);
        }

        void visit(const For *op) {
            bounds.push(op->name,
                        Interval(op->min, simplify(expand_expr(op->min + op->extent - 1, lets), false, bounds)));
            if (starts_with(op->name, stage_prefix)) {
                box = box_provided(Stmt(op), strip_stage(stage_prefix));
                for (size_t i = 0; i < box.size(); ++i) {
                    box[i].min = simplify(expand_expr(box[i].min, lets), false, bounds);
                    box[i].max = simplify(expand_expr(box[i].max, lets), false, bounds);
                }
            } else {
                IRVisitor::visit(op);
            }
            bounds.pop(op->name);
        }

        Scope<Interval> bounds;
        Scope<Expr> lets;
        Box box;
        const string &stage_prefix;

        BoxOfStage(const string &stage_prefix) : stage_prefix(stage_prefix) {}
    };

    /* On the producer's side, we need to inject the declaration of streams. */
    struct StreamAllocInjector : IRMutator {
		using IRMutator::visit;

        void visit(const Realize *realize) {
            if (realize->name == strip_stage(producer)) {
                stmt = mutate(realize->body);
            } else {
                IRMutator::visit(realize);
            }
        }

        void visit(const For *loop) {
            if (starts_with(loop->name, producer)) {
                for_stack.push_back(loop->name);
                Stmt body = mutate(loop->body);
                if (loop->name == scan_level) {
                    Expr send_condition;
                    for (size_t i = 0; i < traverse.size(); ++i) {
                        Expr condition = Var(traverse[i]) >= stencil.consumed_mins[i] &&
                                         Var(traverse[i]) < stencil.consumed_mins[i] + stencil.image_bounds[i];
                        send_condition = send_condition.defined() ? send_condition && condition : condition;
                    }
                    string name = stencil.is_param ? producer : strip_stage(producer);
                    Expr holder = Call::make(stencil.type, Call::sds_single_holder, {Expr(name)}, Call::PureIntrinsic);
                    Expr sender = Call::make(Type(), Call::sds_stream_write,
                                             {Expr(make_stream_name(producer, stencil.consumer)), holder},
                                             Call::CallType::Intrinsic);
                    body = Block::make(body, IfThenElse::make(send_condition, Evaluate::make(sender)));
                    //debug(3) << "Sender injected body!\n" << body << "\n";
                }
                for_stack.pop_back();
                stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type, loop->device_api, body);
                if (for_stack.empty()) {
                    Stmt allocator = Evaluate::make(
                            Call::make(type, Call::sds_stream_alloc, {make_stream_name(producer, consumer), get_stream_depth(producer, consumer, env)},
                                       Call::CallType::Intrinsic));
                    stmt = Block::make(allocator, stmt);
                }
            } else {
                stmt = loop;
            }
        }

        Type type;
        string producer, consumer;
        const Stencil &stencil;
        const string &scan_level;
        const vector <string> &traverse;
        vector <string> for_stack;
        const map<string, Function> &env;

        StreamAllocInjector(Type type, const string &producer, const Stencil &stencil, const vector <string> &traverse, const map<string, Function> &env)
                :
                type(type), producer(producer), consumer(stencil.consumer), stencil(stencil),
                scan_level(traverse.front()), traverse(traverse), env(env) {}
    };

    struct PureProducer : IRMutator {
		using IRMutator::visit;

        void visit(const For *loop) {
            if (under_scan_level) {
                IRMutator::visit(loop);
            } else {
                if (starts_with(loop->name, producer)) {
                    bool old_under = under_scan_level;
                    under_scan_level |= loop->name == scan_level;
                    stmt = For::make(loop->name, Expr(0), loop->extent,
                                     loop->name == scan_level ? ForType::SDSPipeline : loop->for_type, loop->device_api,
                                     mutate(loop->body));
                    under_scan_level = old_under;
                } else {
                    stmt = loop;
                }
            }
        }

        void visit(const Provide *provide) {
            if (provide->name == strip_stage(producer)) {
                internal_assert(provide->values.size() == 1) << "Anything wrong with the providers' values?!\n";
                Expr value = mutate(provide->values[0]);
                stmt = Evaluate::make(
                        Call::make(value.type(), Call::sds_single_holder, {Expr(strip_stage(producer)), value},
                                   Call::Intrinsic)
                );
            } else {
                IRMutator::visit(provide);
            }
        }

        const string &producer, &scan_level;
        bool under_scan_level;

        PureProducer(const string &producer, const string &scan_level) : producer(producer), scan_level(scan_level),
                                                                         under_scan_level(false) {}
    };

    /*struct PartitionInjector : IRMutator {
        void visit(const Realize *realize) {
            if (realize->name == strip_stage(producer)) {
                stmt = mutate(realize->body);
            } else {
                stmt = realize;
            }
        }
        void visit(const For *loop) {
            if (starts_with(loop->name, producer)) {
                Stmt allocator = Evaluate::make(Call::make(type, Call::sds_stream_alloc, {make_stream_name(producer, consumer)}, Call::CallType::Intrinsic));
                stmt = Block::make(allocator, Stmt(loop));
            } else {
                stmt = loop;
            }
        }
        Type type;
        string producer;
        const vector<Expr> &extents;
        PartitionInjector(Type type, const string &producer, const vector<Expr> &extents, Stmt partitioner) : type(type), produer(producer), extents(extents) {}
    };*/

    /* Under the most inner loop, the loop body should be refactored vastly, so I write a independent module to do it. */
    struct ConsumerMaker : IRMutator {
		using IRMutator::visit;

        void visit(const Call *call) {
            if (call->name == strip_stage(consumer)) {
                expr = Call::make(call->type, Call::sds_single_holder, {Expr(strip_stage(consumer))},
                                  Call::PureIntrinsic);
                return;
            }
            for (const HWStageEdge &edge : edges) {
                const Stencil &stencil = edge.stencil;
                if ((stencil.is_param && edge.producer == call->name) ||
                    (!stencil.is_param && strip_stage(edge.producer) == call->name)) {
                    vector <Expr> args(call->args);
                    //debug(3) << "The original call: " << Expr(call) << "\n";


                    for (size_t i = 0; i < args.size(); ++i) {
                        args[i] = simplify(expand_expr(args[i] - stencil.stencil_mins[i], lets), false, bounds);
                    }
                    std::reverse(args.begin(), args.end());
                    /*for (int i = (int) args.size() - 1; i >= 0; --i) {
                        args[i] = simplify(expand_expr(args[i] - stencil.stencil_mins[i], lets), false, bounds);
                    }*/

                    if (stencil.only_one()) {
                        expr = Call::make(call->type, Call::sds_single_holder, {Expr(edge.pure_name())},
                                          Call::PureIntrinsic);
                    } else if (stencil.fully_partition_required()) {
                        args.insert(args.begin(), make_window_name(edge.producer));
                        expr = Call::make(call->type, Call::sds_windowbuffer_access, args, Call::PureIntrinsic);
                    } else {
                        args.insert(args.begin(), make_window_name(edge.producer, stencil.consumer));
                        expr = Call::make(call->type, Call::sds_windowbuffer_access, args, Call::PureIntrinsic);
                    }

                    debug(3) << "Stencil access: "
                             << expr << "\n";
                    debug(3) << "\n";
                    return;
                }
            }
            expr = call;
        }

        void visit(const Provide *provide) {
            if (provide->name == strip_stage(consumer)) {
                internal_assert(provide->values.size() == 1) << "Anything wrong with the providers' values?!\n";
                Expr value = mutate(provide->values[0]);
                stmt = Evaluate::make(
                        Call::make(value.type(), Call::sds_single_holder, {Expr(strip_stage(consumer)), value},
                                   Call::Intrinsic)
                );
            } else {
                IRMutator::visit(provide);
            }
        }

        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            Stmt body = mutate(let->body);
            lets.pop(let->name);
            if (body.same_as(let->body)) {
                stmt = let;
            } else {
                stmt = LetStmt::make(let->name, let->value, body);
            }
        }

        void visit(const For *loop) {
            bounds.push(loop->name,
                        Interval(loop->min, simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            Stmt body = mutate(loop->body);
            stmt = For::make(loop->name, loop->min, loop->extent, ForType::Serial, loop->device_api,
                             body.same_as(loop->body) ? loop->body : body);
            bounds.pop(loop->name);
        }

        const vector <HWStageEdge> &edges;
        Scope<Expr> &lets;
        Scope<Interval> &bounds;
        string consumer;

        ConsumerMaker(const vector <HWStageEdge> &edges, Scope<Expr> &lets, Scope<Interval> &bounds) : edges(edges),
                                                                                                       lets(lets),
                                                                                                       bounds(bounds) {
            consumer = edges.front().stencil.consumer;
        }

    };


    /* On the consumers' side, we need to consume the stream and push the value from stream to linebuffers.
     * Also, we need to inject the declaration of linebuffers. */
    struct InjectStreamConsumer : IRMutator {
		using IRMutator::visit;

        void visit(const For *loop) {
            bounds.push(loop->name,
                        Interval(loop->min, simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            if (starts_with(loop->name, consumer)) {
                for_stack.push_back(loop->name);
                Stmt body;
                if (loop->name == scan_level) {
                    //Make specialized body
                    body = ConsumerMaker(edges, lets, bounds).mutate(loop->body);
                    Expr start_condition;
                    for (int i = for_stack.size() - 1; i >= 0; --i) {
                        Expr condition = Var(for_stack[i]) >= Expr(0);
                        start_condition = start_condition.defined() ? start_condition && condition : condition;
                    }
                    body = IfThenElse::make(start_condition, body);
                    for (const HWStageEdge &edge : edges) {
                        const Stencil &stencil = edge.stencil;
                        if (!stencil.fully_partition_required()) {
                            Expr load_condition;
                            for (size_t i = 0; i < stencil.traverse.size(); ++i)
                                if (stencil.traverse[i] != "<NonSerial>") {
                                    Expr condition = Var(stencil.traverse[i]) >= Expr(-stencil.stencil_bounds[i] + 1);
                                    load_condition = load_condition.defined() ? load_condition && condition : condition;
                                }
                            //s.read()
                            Expr stream_reader = Call::make(stencil.type, Call::sds_stream_read,
                                                            {Expr(make_stream_name(edge.producer, stencil.consumer))},
                                                            Call::Intrinsic);
                            //T holder = s.read()
                            Stmt stream_holder = Evaluate::make(Call::make(stencil.type, Call::sds_single_holder,
                                                                           {Expr(edge.pure_name()), stream_reader},
                                                                           Call::Intrinsic));
                            if (stencil.only_one()) {
                                //A trivial condition which requires no linebuffer
                                body = Block::make(IfThenElse::make(load_condition, stream_holder), body);
                            } else {
                                Expr window_name = Expr(make_window_name(edge.producer, stencil.consumer));
                                Expr buffer_name = Expr(make_linebuffer_name(edge.producer, stencil.consumer));
                                Expr value_holder = Call::make(stencil.type, Call::sds_single_holder,
                                                               {Expr(edge.pure_name())}, Call::Intrinsic);
                                //w.shift_pixels_left(); Then the right most column will have open space for new pixels
                                Stmt window_shift = Evaluate::make(Call::make(Type(), Call::sds_windowbuffer_update,
                                                                              {window_name, Expr("shift_pixels_left")},
                                                                              Call::Intrinsic));
                                //for (int i = 0; i < stencil_height - 1; ++i) w(i, stencil_width - 1) = l(i, current_column);
                                //Load the first (stencil_height - 1) rows of values in the linebuffer to the windowbuffer.
                                Stmt window_loader = For::make(
                                        "linebuffer.update", 0, stencil.stencil_bounds[0] - 1, ForType::Serial,
                                        DeviceAPI::Host,
                                        Evaluate::make(Call::make(
                                                stencil.type,
                                                Call::sds_windowbuffer_access,
                                                {
                                                        window_name,
                                                        Var("linebuffer.update"),
                                                        stencil.stencil_bounds[1] - 1,
                                                        Call::make(
                                                                stencil.type,
                                                                Call::sds_linebuffer_access,
                                                                {
                                                                        buffer_name,
                                                                        Var("linebuffer.update"),
                                                                        Var(loop->name) + stencil.stencil_bounds[0] - 1
                                                                },
                                                                Call::PureIntrinsic
                                                        )
                                                },
                                                Call::Intrinsic
                                        ))
                                );
                                //w(stencil_height - 1, stencil_width - 1) = holder
                                Stmt window_last_pix = Evaluate::make(Call::make(
                                        stencil.type,
                                        Call::sds_windowbuffer_access,
                                        {
                                                window_name,
                                                stencil.stencil_bounds[1] - 1,
                                                stencil.stencil_bounds[0] - 1,
                                                value_holder
                                        },
                                        Call::CallType::Intrinsic
                                ));
                                //l.shift_pixels_up(current_column);
                                //l.insert_buttom_row(current_column, holder);
                                Stmt buffer_shift = Evaluate::make(Call::make(
                                        stencil.type,
                                        Call::sds_linebuffer_update,
                                        {
                                                buffer_name,
                                                Var(loop->name) + stencil.stencil_bounds[0] - 1,
                                                value_holder
                                        },
                                        Call::CallType::Intrinsic
                                ));
                                body = Block::make(IfThenElse::make(load_condition, Block::make(
                                        {stream_holder, window_shift, window_loader, window_last_pix, buffer_shift})),
                                                   body);
                            }
                        }
                    }
                } else {
                    body = mutate(loop->body);
                }
                //Refactor the loops
                int loop_min = 0;
                for (const HWStageEdge &edge : edges) {
                    const Stencil &stencil = edge.stencil;
                    for (size_t i = 0; i < stencil.traverse.size(); ++i) {
                        if (stencil.traverse[i] == loop->name) {
                            loop_min = loop_min < stencil.stencil_bounds[i] ? stencil.stencil_bounds[i] : loop_min;
                        }
                    }
                }
                for_stack.pop_back();
                stmt = For::make(loop->name, Expr(-loop_min + 1), loop->extent + (loop_min - 1),
                                 loop->name == scan_level ? ForType::SDSPipeline : loop->for_type, loop->device_api,
                                 body);
                if (for_stack.empty()) {
                    vector <Stmt> buffer_allocators;
                    for (const HWStageEdge edge : edges) {
                        const Stencil &stencil = edge.stencil;
                        if (!stencil.fully_partition_required() && !stencil.only_one()) {
                            vector <Expr> args;
                            buffer_allocators.push_back(Evaluate::make(
                                    Call::make(
                                            stencil.type,
                                            Call::sds_linebuffer_alloc,
                                            {
                                                    Expr(make_linebuffer_name(edge.producer, stencil.consumer)),
                                                    Expr(stencil.stencil_bounds[0] - 1),
                                                    Expr(stencil.image_bounds[1])
                                            },
                                            Call::PureIntrinsic
                                    )
                            ));
                            buffer_allocators.push_back(Evaluate::make(
                                    Call::make(
                                            stencil.type,
                                            Call::sds_windowbuffer_alloc,
                                            {
                                                    Expr(make_window_name(edge.producer, stencil.consumer)),
                                                    Expr(stencil.stencil_bounds[0]),
                                                    Expr(stencil.stencil_bounds[1])
                                            },
                                            Call::PureIntrinsic
                                    )
                            ));
                        }
                    }
                    buffer_allocators.push_back(stmt);
                    stmt = Block::make(buffer_allocators);
                }
            } else {
                stmt = loop;
            }
            bounds.pop(loop->name);
        }

        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            Stmt body = mutate(let->body);
            lets.pop(let->name);
            if (body.same_as(let->body)) {
                stmt = let;
            } else {
                stmt = LetStmt::make(let->name, let->value, body);
            }
        }

        vector <string> for_stack;
        const vector <HWStageEdge> &edges;
        Scope<Expr> lets;
        Scope<Interval> bounds;
        string scan_level;
        string consumer;

        InjectStreamConsumer(const vector <HWStageEdge> &edges) : edges(edges),
                                                                  scan_level(edges.front().stencil.scan_level),
                                                                  consumer(edges.front().stencil.consumer) {}
    };

    Box box_of_stage(Stmt s, const string &stage_prefix) {
        //Box total_box = box_touched(s, strip_stage(stage_prefix));
        BoxOfStage finder(stage_prefix);
        s.accept(&finder);
        /*FIXME: is this constraint necessary?
        internal_assert(total_box.size() == finder.box.size());
        for (size_t i = 0; i < total_box.size(); ++i) {
            internal_assert(total_box[i].min.same_as(finder.box[i].min))
                    << total_box[i].min << "!=" << finder.box[i].min
                    << "\nOnly updating the whole image is allowed right now!\n";
            internal_assert(total_box[i].max.same_as(finder.box[i].max))
                    << total_box[i].max << "!=" << finder.box[i].max
                    << "\nOnly updating the whole image is allowed right now!\n";
        }*/
        return finder.box;
    }

    /* Find all holders in order to allocate single memory unit for them. */
    struct HolderFinder : public IRVisitor {
		using IRVisitor::visit;
        void visit(const Call *call) {
            if (call->is_intrinsic(Call::sds_single_holder)) {
                string holder_name = call->args[0].as<StringImm>()->value;
                if (holder_type.find(holder_name) == holder_type.end()) {
                    holder_type[holder_name] = call->type;
                } else {
                    internal_assert(holder_type[holder_name] == call->type)
                            << "Holder different type detected! Abort!\n";
                }
            }
        }

        map <string, Type> holder_type;
    };

    /* Replace holders by memory load or store. */
    struct HolderReplacer : IRMutator {
		using IRMutator::visit;
        void visit(const Evaluate *eval) {
            const Call *call = eval->value.as<Call>();
            if (call && call->is_intrinsic(Call::sds_single_holder)) {
                if (call->args.size() == 1) {
                    internal_assert(false) << "Should not put here!\n";
                } else {
                    string holder_name = call->args[0].as<StringImm>()->value;
                    for (size_t i = 0; i < params.size(); ++i) {
                        if (holder_name == params[i].name) {
                            if (i == params.size() - 1) {
                                stmt = Store::make(holder_name, mutate(call->args[1]), write_back_index, Parameter(),
                                                   const_true());
                            } else {
                                if (call->args[1].as<Variable>()) {
                                    //It is from a distributor, so memory load needed
                                    int stride = 1;
                                    Expr read_index;
                                    for (size_t j = 1; j < call->args.size(); ++j) {
                                        read_index = read_index.defined() ? read_index + call->args[j] * stride :
                                                     call->args[j] * stride;
                                        stride *= (int) *as_const_int(params[i].sub[j - 1].extent);
                                    }
                                    stmt = Store::make(holder_name + "_holder",
                                                       Load::make(call->type, holder_name, read_index, Buffer<>(),
                                                                  Parameter(), const_true()),
                                                       Expr(0), Parameter(), const_true());
                                } else {
                                    //It is from an intermediate stage, so only one single holder needed
                                    stmt = Store::make(holder_name + "_holder", mutate(call->args[1]), Expr(0),
                                                       Parameter(), const_true());
                                }
                            }
                            return;
                        }
                    }
                    stmt = Store::make(holder_name + "_holder", mutate(call->args[1]), Expr(0), Parameter(),
                                       const_true());
                }
            } else {
                IRMutator::visit(eval);
            }
        }

        void visit(const Call *call) {
            if (call->is_intrinsic(Call::sds_single_holder)) {
                string holder_name = call->args[0].as<StringImm>()->value;
                if (call->args.size() == 1) {
                    expr = Load::make(call->type, holder_name + "_holder", Expr(0), Buffer<>(), Parameter(), Expr(1));
                } else if (call->args.size() == 2) {
                    internal_assert(false) << "Should not come here!\n";
                }
            } else {
                IRMutator::visit(call);
            }
        }

        const vector <HWParam> &params;
        Expr write_back_index;

        HolderReplacer(const vector <HWParam> &params, const vector <string> &traverse, const vector<int> &extents)
                : params(params) {
            int stride = 1;
            for (size_t i = 0; i < traverse.size(); ++i) {
                write_back_index = !write_back_index.defined() ? Var(traverse[i]) * stride :
                                   write_back_index + Var(traverse[i]) * stride;
                stride *= extents[i];
            }
        }
    };

    /* Lower all the memory holder to single unit memory access */
    struct OffloadLower : public IRMutator {
		using IRMutator::visit;

        void visit(const For *loop) {
            if (loop->for_type == ForType::SDSPipeline) {
                HolderFinder finder;
                loop->body.accept(&finder);
                Stmt body = HolderReplacer(params, output_traverse, extents).mutate(loop->body);
                for (const pair <string, Type> holder : finder.holder_type) {
                    if (holder.first != params.back().name) {
                        body = Allocate::make(holder.first + "_holder", holder.second, {Expr(1)}, const_true(), body);
                    }
                }
                if (!body.same_as(loop->body)) {
                    stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type, loop->device_api, body);
                } else {
                    stmt = loop;
                }
            } else {
                Stmt body = mutate(loop->body);
                if (!body.same_as(loop->body)) {
                    stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type, loop->device_api, body);
                } else {
                    stmt = loop;
                }
            }
        }

        const vector <HWParam> &params;
        const vector <string> &output_traverse;
        vector<int> extents;

        OffloadLower(const vector <HWParam> &params, const vector <string> &traverse_loop) : params(params),
                                                                                             output_traverse(
                                                                                                     traverse_loop) {
            for (size_t i = 0; i < params.back().dim(); ++i) {
                extents.push_back((int) *as_const_int(params.back().sub[i].extent));
            }
        }
    };

    struct MakeTheSameCall : public IRVisitor {
		using IRVisitor::visit;
        void visit(const Call *call) {
            if (!res.defined() && call->name == name) {
                res = Call::make(call->type, call->name, args, call->call_type,
                                 call->func, call->value_index, call->image, call->param);
            }
        }

        const string &name;
        const vector <Expr> &args;
        Expr res;

        MakeTheSameCall(const string &name, const vector <Expr> &args) : name(name), args(args) {}
    };

    /* DEPRECATED: copying code from inner loops is not a good idea!
    struct WriteBackMaker : public IRVisitor {
        void visit(const For *loop) {
            if (starts_with(loop->name, prefix)) {
                for_stack.push_back(loop);
                IRVisitor::visit(loop);
                for_stack.pop_back();
            } else {
                IRVisitor::visit(loop);
            }
        }

        void visit(const Provide *provide) {
            if (provide->name == strip_stage(prefix)) {
                vector <Expr> args;
                for (size_t i = 0; i < provide->args.size(); ++i) {
                    for (const For *loop : for_stack) {
                        if (expr_uses_var(provide->args[i], loop->name)) {
                            args.push_back(Var(loop->name) - loop->min);
                        }
                    }
                }
                internal_assert(args.size() == provide->args.size());
                write_back = Provide::make(provide->name, {Call::make(image, args)}, provide->args);
                for (size_t i = 0; i < provide->args.size(); ++i) {
                    for (const For *loop : for_stack) {
                        if (expr_uses_var(provide->args[i], loop->name)) {
                            write_back = For::make(loop->name, loop->min, loop->extent, loop->for_type,
                                                   loop->device_api, write_back);
                        }
                    }
                }
            } else {
                IRVisitor::visit(provide);
            }
        }

        vector<const For *> for_stack;
        string prefix;
        Buffer<> image;
        Stmt write_back;

        WriteBackMaker(const Function &func, const Buffer<> &image) : prefix(func.name() + ".s0."), image(image) {}
    };*/

    struct OffloadAnnotator : public IRMutator {
		using IRMutator::visit;

        void visit(const LetStmt *let) {
            Expr value = simplify(mutate(expand_expr(let->value, lets)), false, bounds);
            lets.push(let->name, value);
            Stmt body = mutate(let->body);
            if (body.same_as(let->body)) {
                stmt = let;
            } else {
                stmt = LetStmt::make(let->name, is_const(value) ? value : let->value, body);
            }
            lets.pop(let->name);
        }

        void visit(const For *op) {
            bool is_offload = offload_level.match(op->name);
            Interval interval(simplify(mutate(expand_expr(op->min, lets)), false, bounds),
                              simplify(expand_expr(mutate(op->min + op->extent - 1), lets), false, bounds));
            bounds.push(op->name, interval);
            if (is_offload) {
                Stmt new_body;
                Stmt unpruned = op->body;

                new_body = OffloadPruner(lets, bounds).mutate(op->body);

                //TODO: later vectorize the output parameter
                Box output_box = box_provided(new_body, offload_level.func());
                Region sub_output, output_whole;
                debug(3) << "The output of offloaded body is: ";
                Type output_type = type_of_call_or_provide(new_body, offload_level.func(), true);
                for (size_t i = 0; i < output_box.size(); ++i) {
                    Expr extent = simplify(expand_expr(output_box[i].max - output_box[i].min + 1, lets), false, bounds);
                    internal_assert(is_const(extent));
                    debug(3) << "[" << output_box[i].min << ", " << extent << "]";
                    sub_output.push_back(Range(simplify(expand_expr(output_box[i].min, lets), false, bounds), extent));
                    output_whole.push_back(
                            Range(Var(offload_level.func() + ".min." + std::to_string(i)),
                                  Var(offload_level.func() + ".extent." + std::to_string(i))
                            )
                    );
                }
                debug(3) << "\n";

                map <string, Box> images = offload_inputs(new_body, offload_level.func()), stage_bounds;
                vector <HWParam> hw_param;
                map <string, vector<HWStageEdge>> consume_graph;
                map <string, vector<string>> traverse_collection;
                set <string> input_names, hsh;
                for (const pair <string, Box> &i : images) {
                    input_names.insert(i.first);
                    hsh.insert(i.first);
                }

                vector <Stmt> data_duplicators;
                for (const pair <string, Box> &input : images) {
                    Box box = box_required(unpruned, input.first);
                    vector <Expr> args, provider_args;
                    for (size_t j = 0; j < box.size(); ++j) {
                        args.push_back(Var("dup." + input.first + "." + std::to_string(j)) + box[j].min);
                        provider_args.push_back(Var("dup." + input.first + "." + std::to_string(j)));
                    }
                    MakeTheSameCall maker(input.first, args);
                    new_body.accept(&maker);
                    Stmt duplicator = Provide::make("dup$$" + input.first, {maker.res}, provider_args);
                    Region region;
                    for (size_t j = 0; j < box.size(); ++j) {
                        Range range(
                                simplify(expand_expr(box[j].min, lets), false, bounds),
                                simplify(expand_expr(box[j].max - box[j].min + 1, lets), false, bounds)
                        );
                        duplicator =
                                For::make("dup." + input.first + "." + std::to_string(j), 0, range.extent,
                                          ForType::Serial, DeviceAPI::Host, duplicator);
                        region.push_back(range);
                    }
                    //debug(3) << "Duplicator\n" << duplicator << "\n";
                    Expr stride(1);
                    vector <Expr> extents;
                    for (size_t j = 0; j < input.second.size(); ++j) {
                        duplicator = LetStmt::make("dup$$" + input.first + ".min." + std::to_string(j), 0, duplicator);
                        duplicator = LetStmt::make("dup$$" + input.first + ".extent." + std::to_string(j),
                                                   region[j].extent, duplicator);
                        duplicator = LetStmt::make("dup$$" + input.first + ".stride." + std::to_string(j), stride,
                                                   duplicator);
                        stride = stride * region[j].extent;
                        extents.push_back(region[j].extent);
                    }
                    duplicator = Allocate::make("dup$$" + input.first, type_of_call_or_provide(unpruned, input.first, true),
                                                extents, const_true(), duplicator);
                    data_duplicators.push_back(duplicator);
                }

                Stmt data_write_back;
                {
                    Box box = box_provided(unpruned, offload_level.func());
                    vector<int> sizes;
                    for (size_t i = 0; i < sub_output.size(); ++i) {
                        internal_assert(is_const(sub_output[i].extent));
                        sizes.push_back(*as_const_int(sub_output[i].extent));
                    }
                    //Buffer<> out_buffer(output_type, sizes, "dup$$" + offload_level.func());
                    vector<Expr> provide_args, call_args;
                    for (size_t i = 0; i < box.size(); ++i) {
                        provide_args.push_back(Var("dup$$" + offload_func.name() + std::to_string(i)) + box[i].min);
                        call_args.push_back(Var("dup$$" + offload_func.name() + std::to_string(i)));
                    }
                    Stmt write_back = Provide::make(offload_func.name(), {Call::make(output_type, "dup$$" + offload_func.name(), call_args, Call::CallType::Halide)}, provide_args);
                    for (size_t i = 0; i < box.size(); ++i) {
                        write_back = For::make("dup$$" + offload_func.name() + std::to_string(i), Expr(0), Expr(sizes[i]), ForType::Serial, DeviceAPI::Host, write_back);
                    }
                    int stride = 1;
                    vector <Expr> extents;
                    for (size_t j = 0; j < box.size(); ++j) {
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".min." + std::to_string(j), 0,
                                                   write_back);
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".extent." + std::to_string(j),
                                                   sizes[j], write_back);
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".stride." + std::to_string(j),
                                                   stride,
                                                   write_back);
                        stride *= sizes[j];
                        extents.push_back(Expr(sizes[j]));
                    }
                    debug(3) << "Write it back: "
                             << write_back << "\n";
                    data_write_back = Allocate::make("dup$$" + offload_func.name(), output_type, extents, const_true(),
                                                     write_back);
                }

                //For every producer, analyze the stencil of consumers
                while (!images.empty()) {
                    pair <string, Box> input = *images.begin();
                    bool is_input_param = input_names.find(input.first) != input_names.end();
                    //Shitty fix...
                    debug(3) << "Try analyzing " << input.first << "\n";
                    if (!is_input_param) {
                        int stage = get_stage(input.first) - 1;
                        if (stage >= 0) {
                            string previous_stage = strip_stage(input.first) + ".s" + std::to_string(stage) + ".";
                            if (hsh.find(previous_stage) == hsh.end()) {
                                hsh.insert(previous_stage);
                                images[previous_stage] = box_of_stage(new_body, previous_stage);
                                debug(3) << "Previous stage added: " << previous_stage << "\n";
                                debug(3) << "Abort!\n";
                                continue;
                            }
                        }
                    }
                    images.erase(images.begin());

                    Box expanded = input.second;
                    Region whole, sub_input;
                    for (size_t i = 0; i < expanded.size(); ++i) {
                        expanded[i].min = simplify(expand_expr(expanded[i].min, lets), false, bounds);
                        expanded[i].max = simplify(expand_expr(expanded[i].max, lets), false, bounds);
                        Expr extent = simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds);
                        internal_assert(is_const(extent))
                                << "All stage or input inside offloaded body supposed to have fixed bound!\n"
                                << (simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds))
                                << "\n";
                        if (is_input_param) {
                            sub_input.push_back(Range(expanded[i].min, extent));
                            whole.push_back(Range(Var(input.first + ".min." + std::to_string(i)),
                                                  Var(input.first + ".extent." + std::to_string(i))));
                        }
                    }
                    stage_bounds[input.first] = expanded;
                    vector <Stencil> consumers = analyze_stencil(
                            new_body,
                            input.first,
                            offload_level.func(),
                            env,
                            is_input_param,
                            traverse_collection
                    );
                    //debug(3) << (input_names.find(input.first) != input_names.end() ? "Image " : "Stage ") << input.first << ":\n";
                    Type type = type_of_call_or_provide(new_body, input.first, is_input_param);
                    vector <Stmt> distributors;
                    bool partitioned = false;
                    for (Stencil &stencil : consumers) {
                        stencil.type = type;
                        if (hsh.find(stencil.consumer) == hsh.end()) {
                            hsh.insert(input.first);
                            images[stencil.consumer] = box_of_stage(new_body, stencil.consumer);
                        } else {
                            box_of_stage(new_body, stencil.consumer);
                        }
                        internal_assert(stencil.image_mins.size() == input.second.size())
                                << "Image and stencil should have same dim!\n" << stencil.image_mins.size() << " != "
                                << input.second.size() << "\n";
                        internal_assert(stencil.consumed_mins.size() == 0) << "Data corrupted!\n";
                        for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                            Expr bound_min = simplify(expand_expr(stencil.image_mins[i] - expanded[i].min, lets), false,
                                                      bounds);
                            internal_assert(is_const(bound_min)) << "Consumed image should have fixed bound!\n"
                                                                 << bound_min << "\n";
                            stencil.consumed_mins.push_back(*as_const_int(bound_min));
                        }
                        //Gather all the producers at the consumer side.
                        consume_graph[stencil.consumer].push_back(HWStageEdge(input.first, stencil));
                        //Make distributor
                        if (stencil.fully_partition_required() && !partitioned) {
                            distributors.clear();
                            distributors.push_back(make_partitioner(type, input.first, stencil));
                            partitioned = true;
                        } else if (!partitioned) {
                            distributors.push_back(make_distributor(type, input.first, stencil));
                        }
                    }
                    if (is_input_param) {
                        debug(3) << input.first << " is a parameter!\n";
                        hw_param.push_back(HWParam(type, input.first, whole, sub_input));
                        //TODO: If possible, we need to vectorize certain dim of both input and output parameters later
                        Stmt dp_stmt = Block::make(distributors);
                        dp_stmt = Block::make(make_holder(type, input.first, *consumers.begin()), dp_stmt);
                        for (size_t i = 0; i < input.second.size(); ++i) {
                            dp_stmt = For::make(make_iter_of_distributor(input.first, i), Expr(0), sub_input[i].extent,
                                                i == 0 ? ForType::SDSPipeline : ForType::Serial, DeviceAPI::Host,
                                                dp_stmt);
                        }
                        if (partitioned) {
                            debug(3) << input.first << " is being partitioned!\n";
                            //internal_assert(sub_input.size() == 2) << "Right now only 2d is supported...\n";
                            vector <Expr> args{Expr(make_window_name(input.first))};
                            for (size_t i = 0; i < sub_input.size(); ++i) {
                                args.push_back(sub_input[i].extent);
                            }
                            Stmt window_alloc = Evaluate::make(
                                    Call::make(type, Call::sds_windowbuffer_alloc, args, Call::CallType::Intrinsic));
                            dp_stmt = Block::make(window_alloc, dp_stmt);
                            debug(3) << input.first << " partitioned!\n";
                        } else {
                            vector <Stmt> stream_allocs;
                            for (const Stencil &stencil : consumers) {
                                Expr stream_alloc =
                                        Call::make(type, Call::sds_stream_alloc,
                                                   {Expr(make_stream_name(input.first, stencil.consumer)), get_stream_depth(input.first, stencil.consumer, env)},
                                                   Call::CallType::Intrinsic);
                                stream_allocs.push_back(Evaluate::make(stream_alloc));
                            }
                            stream_allocs.push_back(dp_stmt);
                            dp_stmt = Block::make(stream_allocs);
                        }
                        debug(3) << input.first << " as a parameter added...\n";
                        new_body = Block::make(dp_stmt, new_body);
                    } else {
                        debug(3) << input.first << " stencil analysis done...\n";
                        if (!partitioned) {
                            for (const Stencil &stencil : consumers) {
                                internal_assert(traverse_collection.find(input.first) != traverse_collection.end())
                                        << "The producer " << input.first << " should have been analyzed...\n";
                                StreamAllocInjector sai(type, input.first, stencil, traverse_collection[input.first], env);
                                //debug(3) << sai.mutate(new_body) << "\n";
                                new_body = sai.mutate(new_body);
                            }
                        } else {
                            //PartitionInjector(input.first, distributors[0]);
                            internal_assert(false) << "Right now only input parameters can be partitioned!\n";
                        }
                    }
                }
                debug(3) << "========== Stage(s) consumes no stages ==========\n";
                for (const string &stage : hsh) {
                    if (consume_graph.find(stage) == consume_graph.end() &&
                        input_names.find(stage) == input_names.end()) {
                        debug(3) << stage << "\n";
                        new_body = PureProducer(stage, traverse_collection[stage].front()).mutate(new_body);
                        debug(3) << new_body << "\n";
                    }
                }
                debug(3) << "========== End ==========\n";
                for (const pair <string, vector<HWStageEdge>> node : consume_graph) {
                    internal_assert(!node.second.empty());
                    for (const HWStageEdge &edge : node.second) {
                        const Stencil &stencil = edge.stencil;
                        debug(3) << edge.producer << "("
                                 << (input_names.find(edge.producer) != input_names.end() ? "input" : "stage") << ")"
                                 << " -> " << stencil.consumer << "\n"
                                 << "Stecil bound: ";
                        for (const int &bound : stencil.stencil_bounds) {
                            debug(3) << "[" << bound << "]";
                        }
                        const Box &expanded = stage_bounds[edge.producer];
                        internal_assert(expanded.size() == stencil.image_mins.size()) << "WTF?!\n";
                        debug(3) << "\n" << "Consumed bound: ";
                        for (size_t i = 0; i < stencil.consumed_mins.size(); ++i) {
                            debug(3) << "["
                                     << stencil.consumed_mins[i] << ", "
                                     << stencil.consumed_mins[i] + stencil.image_bounds[i] - 1
                                     << "]";
                        }
                        debug(3) << "\nMovement: ";
                        for (const int &stride : stencil.movement) {
                            debug(3) << "[" << stride << "]";
                        }
                        debug(3) << "\nName of traverse loop(s):\n";
                        for (const string &loop_name : stencil.traverse) {
                            debug(3) << loop_name << "\n";
                        }
                        debug(3) << "\n";

                    }
                    new_body = InjectStreamConsumer(node.second).mutate(new_body);
                }
                hw_param.push_back(HWParam(output_type, offload_level.func(), output_whole, sub_output));
                internal_assert(traverse_collection.find(offload_level.func() + ".s0.") != traverse_collection.end())
                        << "Traverse loop of out put not found?!\n";
                /*Lower single holder to memory access:
                 * 1. For input parameter, lower it to flattened Load
                 * 2. For output parameter, lower it to flattened Store
                 * 3. For intermediate result, lower it to a single unit memory Load or Store
                 * TODO: Here is some kind of short-eye-sighted decision. Right now I have to lower holder to AP_INT
                 * */
                new_body = OffloadLower(hw_param, traverse_collection[offload_level.func() + ".s0."]).mutate(new_body);
                new_body = Offload::make(offload_level.func(), hw_param, new_body);

                /*Inject data duplication so that we can pass data to FPGA part. */
                {
                    const Allocate *allocate = data_write_back.as<Allocate>();
                    internal_assert(allocate);
                    new_body = Allocate::make(allocate->name, allocate->type, allocate->extents, allocate->condition,
                                              Block::make(new_body, allocate->body));
                }
                for (size_t i = 0; i < data_duplicators.size(); ++i) {
                    const Allocate *allocate = data_duplicators[i].as<Allocate>();
                    internal_assert(allocate);
                    new_body = Allocate::make(allocate->name, allocate->type, allocate->extents, allocate->condition,
                                              Block::make(allocate->body, new_body));
                }

                stmt = For::make(op->name, op->min, op->extent, op->for_type, op->device_api, new_body);
            } else {
                Stmt new_body = mutate(op->body);
                if (new_body.same_as(op->body)) {
                    stmt = op;
                } else {
                    stmt = For::make(op->name, op->min, op->extent, op->for_type, op->device_api, new_body);
                }
            }
            bounds.pop(op->name);
        }

        void visit(const Realize *realize) {
            realizations[realize->name] = realize->bounds;
            IRMutator::visit(realize);
            realizations.erase(realize->name);
        }

        const Function &offload_func;
        const LoopLevel &offload_level;
        const map<string, Function> &env;
        Scope<Expr> lets;
        Scope<Interval> bounds;
        map <string, Region> realizations;

        OffloadAnnotator(const Function &func, const map<string, Function> &env)
                : offload_func(func), offload_level(func.schedule().offload_level()), env(env) {}
    };

    Stmt offload_functions(Stmt s,
                           const vector <Function> &outputs,
                           const map <string, Function> &env) {
        for (const pair <string, Function> &function : env) {
            const vector <string> &offloads(function.second.schedule().offloaded_stages());
            //if (offloads.size() != 0) {
            if (function.second.schedule().offload_level().func() != "") {
                map<string, Function> sub_env;
                for (auto i : offloads) {
                    sub_env[i] = env.find(i)->second;
                }
                sub_env[function.first] = function.second;
                OffloadAnnotator mutator(function.second, sub_env);
                s = mutator.mutate(s);
            }
        }
        return s;
    }

}

}
