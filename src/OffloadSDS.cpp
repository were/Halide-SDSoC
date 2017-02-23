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

        string strip_stage(const string &s) {
            for (size_t i = 0; i < s.size(); ++i) {
                if (s[i] == '.') {
                    return s.substr(0, i);
                }
            }
            internal_assert(false) << s << " is not a stage variable!\n";
            return "";
        }

        string get_prefix(const string &s) {
            return strip_stage(s) + ".s" + std::to_string(get_stage(s)) + ".";
        }

        //In this stage, we need a very aggressive constant propagation, but we cannot acutually do it, which may cause
        //strange error in later lowering stage. Thus, we can only expand the expressions when necessary.
        class ExpandExpr : public IRMutator {
            using IRMutator::visit;
            const Scope<Expr> &scope;

            void visit(const Variable *var) {
                if (scope.contains(var->name)) {
                    //expr = scope.get(var->name); is not fully expanded variable!
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
            vector<Expr> image_mins;
            vector<int> consumed_mins; //This is not calculated when analyzing...
            vector<int> image_bounds;
            vector<Expr> stencil_mins;
            vector<int> stencil_bounds;
            vector<int> movement;
            vector<string> traverse;
            string scan_level;
            Stencil(const string &consumer,
                    const Box &consumed_box,
                    const Box &stencil_box,
                    const vector<const For *> &traverse_loop) : consumer(consumer), movement(stencil_box.size(), 0), traverse(stencil_box.size(), "<NonSerial>"), scan_level(traverse_loop.back()->name) {
                internal_assert(consumed_box.size() == stencil_box.size()) << "Consumed and stencil should have the same dimension!\n";
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
                internal_assert(movement.size() == stencil_mins.size()) << "Movement init failed?!\n" << movement.size() << " != " << stencil_mins.size() << "\n";
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
            bool fully_partition_required() {
                for (int i : movement) {
                    if (i) {
                        return false;
                    }
                }
                return true;
            }
        };

        string append_tail(const string &prefix, const string &suffix) {
            return prefix + (prefix.back() == '.' ? suffix : "." + suffix);
        }

        string make_iter_of_distributor(const string &prefix, const int &dim) {
            return append_tail("distributor." + prefix, std::to_string(dim));
        }

        string make_stream_name(const string &producer, const string &consumer) {
            return append_tail(append_tail(producer, "to.") + consumer, "stream");
        }

        Stmt make_distributor(Type type, const string &producer, const Stencil &stencil) {
            Expr condition;
            for (size_t i = 0; i < stencil.consumed_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                Expr cur_cond = iter_var >= Expr(stencil.consumed_mins[i]) && iter_var < Expr(stencil.consumed_mins[i] + stencil.image_bounds[i]);
                condition = condition.defined() ? condition && cur_cond : cur_cond;
            }
            Expr stream_name = Expr(make_stream_name(producer, stencil.consumer));
            Expr holder = Call::make(type, Call::sds_single_holder, {}, Call::CallType::Intrinsic);
            Expr sender = Call::make(Type(), Call::sds_stream_write, {stream_name, holder}, Call::CallType::Intrinsic);
            return IfThenElse::make(condition, Evaluate::make(sender));
        }

        Stmt make_partitioner(Type type, const string &producer, const Stencil &stencil) {
            vector<Expr> args{Expr(append_tail("partitioned." + producer, "buffer"))};
            for (size_t i = 0; i < stencil.consumed_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                args.push_back(iter_var);
            }
            Expr holder = Call::make(type, Call::sds_single_holder, {}, Call::CallType::Intrinsic);
            args.push_back(holder);
            return Evaluate::make(Call::make(type, Call::sds_windowbuffer_stencil, args, Call::CallType::Intrinsic));
        }

        Stmt make_holder(Type type, const string &producer, const Stencil &stencil) {
            vector<Expr> args{Expr(producer)};
            for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                args.push_back(iter_var);
            }
            return Evaluate::make(Call::make(type, Call::sds_single_holder, args, Call::CallType::Intrinsic));
        }
    }

    struct HWStageEdge {
        string producer;
        Stencil stencil;
        HWStageEdge(const string &producer, const Stencil &stencil) :
                producer(producer), stencil(stencil) {}
    };

    struct StencilAnalyzer : IRVisitor {
        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            IRVisitor::visit(let);
            lets.pop(let->name);
        }

        void visit(const For *op) {
            bounds.push(op->name, Interval(op->min, simplify(expand_expr(op->min + op->extent - 1, lets), false, bounds)));
            for_stack.push_back(op);
            IRVisitor::visit(op);
            for_stack.pop_back();
            bounds.pop(op->name);
        }

        void visit(const Provide *provide) {
            if (offloaded.find(provide->name) != offloaded.end()) {
                vector<const For *> traverse_loop;
                int pre_dim = for_stack.size() + 1;
                for (const For *loop : for_stack) {
                    int depend_cnt = 0;
                    for (size_t i = 0; i < provide->args.size(); ++i) {
                        if (expr_uses_var(provide->args[i], loop->name) && loop->for_type == ForType::Serial) {
                            user_assert(++depend_cnt <= 1)
                                    << Stmt(provide)
                                    << "\nMore than one dimension depend on the traverse loop, abort!\n";
                            user_assert(pre_dim > (int) i)
                                    << Stmt(provide)
                                    << "\nShould scan the dimension from outer to inner!\n"
                                    << pre_dim << " < " << i << "\n";
                            traverse_loop.push_back(loop);
                            pre_dim = i;
                        }
                    }
                }

                Box stencil_box = box_required(traverse_loop.back()->body, is_input_param ? producer : strip_stage(producer));
                if (stencil_box.size() != 0) {
                    debug(3) << get_prefix(traverse_loop.front()->name) << " consumes " << producer << "\n";
                    debug(3) << Stmt(provide) << "\n";
                    debug(3) << "Traverse loop(s):\n";
                    for (auto loop : traverse_loop) {
                        debug(3) << loop->name << "\n";
                    }
                    debug(3) << "\n";
                    Box consumed_box = box_required(Stmt(traverse_loop[0]), is_input_param ? producer : strip_stage(producer));
                    for (size_t i = 0; i < consumed_box.size(); ++i) {
                        consumed_box[i].min = simplify(expand_expr(consumed_box[i].min, lets), false, bounds);
                        consumed_box[i].max = simplify(expand_expr(consumed_box[i].max, lets), false, bounds);
                    }
                    for (size_t i = 0; i < stencil_box.size(); ++i) {
                        stencil_box[i].min = simplify(expand_expr(stencil_box[i].min, lets), false, bounds);
                        stencil_box[i].max = simplify(expand_expr(stencil_box[i].max, lets), false, bounds);
                    }
                    res.push_back(Stencil(get_prefix(traverse_loop[0]->name), consumed_box, stencil_box, traverse_loop));
                }
            }
        }

        string producer;
        set <string> offloaded;
        bool is_input_param;
        vector<const For *> for_stack;
        using IRVisitor::visit;
        Scope<Expr> lets;
        Scope<Interval> bounds;

        vector<Stencil> res;

        StencilAnalyzer(const string &producer, const string &func, const vector<string> &offloaded_stages, bool is_input_param) : producer(producer), is_input_param(is_input_param) {
            for (const string &stage : offloaded_stages)
                offloaded.insert(stage);
            offloaded.insert(func);
        }

    };

    vector<Stencil> analyze_stencil(Stmt s, const string &producer, const string &offload_func, const vector<string> &offloaded_stages, bool is_input_param) {
        StencilAnalyzer collector(producer, offload_func, offloaded_stages, is_input_param);
        s.accept(&collector);
        return collector.res;
    }

    struct OffloadPruner : public IRMutator {
        using IRMutator::visit;

        //Inside the offloaded body, we do not need loop partition, but loop perfection.
        void visit(const Call *call) {
            if (call->is_intrinsic(Call::likely) || call->is_intrinsic(Call::likely_if_innermost)) {
                internal_assert(call->args.size() == 1);
                expr = mutate(call->args[0]);
            } else {
                expr = call;
            }
        }

        void visit(const Provide *provide) {
            vector<Expr> values(provide->values);
            for (Expr &expr : values) {
                expr = simplify(mutate(expr), false, bounds);
            }
            stmt = Provide::make(provide->name, values, provide->args);
        }

        void visit(const For *loop) {
            Stmt body = mutate(loop->body);
            Expr loop_min = mutate(loop->min);
            Expr loop_extent = simplify(expand_expr(loop->extent, lets), false, bounds);
            bounds.push(loop->name, Interval(simplify(expand_expr(loop_min, lets), false, bounds), simplify(expand_expr(loop_min + loop_extent - 1, lets), false, bounds)));
            internal_assert(is_const(loop_extent)) << "Among Offloaded body, all of the loop supposed to be fix bounded...\n";
            if (*as_const_int(loop_extent) == (int) 0) {
                stmt = Evaluate::make(Expr(0));
                return ;
            }
            //In certain version, simplify trivial loops will cause some over propagted constant,
            //which cannot be simplified in later lowering stage, so I choose not to simplify trivial
            //loops here!
            /*else if (*as_const_int(loop->extent) == (int) 1){
                stmt = substitute(loop->name, loop_min, body);
                return ;
            }*/
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
            if (already.find(call->name) == already.end() && (call->call_type == Call::Halide || call->call_type == Call::Image)) {
                res.insert(call->name);
            }
        }

        OffloadInputs(const string &offload_func) {
            already.insert(offload_func);
        }

        void visit(const For *loop) {
            bounds.push(loop->name, Interval(simplify(expand_expr(loop->min, lets), false, bounds), simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            IRVisitor::visit(loop);
        }

        void visit(const LetStmt *let) {
            lets.push(let->name, simplify(expand_expr(let->value, lets), false, bounds));
            IRVisitor::visit(let);
        }

        Scope<Expr> lets;
        Scope<Interval> bounds;
        set<string> already;
        set<string> res;
        OffloadInputs() {}
    };

    map<string, Box> offload_inputs(Stmt s, const string &offload_func) {
        OffloadInputs collector(offload_func);
        s.accept(&collector);
        map<string, Box> res;
        for (const string &input : collector.res) {
            res[input] = box_required(s, input);
            Box &box = res[input];
            for (size_t i = 0; i < box.size(); ++i) {
                box[i].min = simplify(expand_expr(box[i].min, collector.lets), false, collector.bounds);
                box[i].max = simplify(expand_expr(box[i].max, collector.lets), false, collector.bounds);
            }
            //debug(3) << input;
            /*
            for (size_t i = 0; i < res[input].size(); ++i) {
                Expr extent = simplify(res[input][i].max - res[input][i].min);
                debug(3) << "[" << res[input][i].min << ", " << extent << "]";
                internal_assert(is_const(extent)) << "All inputs are supposed to have constant bounds!\nBut " << extent << "\n";
            }*/
            //debug(3) << "\n";
        }
        return res;
    }

    struct TypeOfCallOrProvide : IRVisitor {
        void visit(const Call *call) {
            if (call->name == call_name) {
                if (!found) {
                    type = call->type;
                    found = true;
                    //debug(3) << "find! The type of " << call->name << " is " << type << "\n";
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
                    //debug(3) << "find! The type of " << provide->name << " is " << type << "\n";
                } else {
                    internal_assert(type == provide->values[0].type());
                }
            }
            IRVisitor::visit(provide);
        }
        const string &call_name;
        Type type;
        bool found;
        TypeOfCallOrProvide(const string &s) : call_name(s), found(false) {}
    };

    Type type_of_call_or_provide(Stmt s, const string &name) {
        TypeOfCallOrProvide visitor(name);
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
        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            IRVisitor::visit(let);
            lets.pop(let->name);
        }

        void visit(const For *op) {
            bounds.push(op->name, Interval(op->min, simplify(expand_expr(op->min + op->extent - 1, lets), false, bounds)));
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

    struct StreamInjectorProducerSide : IRMutator {
        //TODO: implement this IRMutator
    };

    /*struct StreamInjectorConsumerSide : IRMutator {
        string get_template_name(const string &template_name) {
            return producer + (is_input_param ? ".to." : "to.") + stencil.consumer + template_name;
        }

        void visit(const For *loop) {
            bounds.push(loop->name, Interval(loop->min, simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            if (starts_with(loop->name, stencil.consumer)) {
                if (loop->name == stencil.scan_level()) {
                    under_scan_level = true;
                    Stmt body = mutate(loop->body);
                    under_scan_level = false;
                    stmt = For::make(loop->name, loop->min, loop->extent, ForType::SDSPipeline, loop->device_api, body.same_as(loop->body) ? loop->body : body);
                    //debug(3) << "Pipelined loop level mutated\n";
                } else {
                    /*if (!under_scan_level) {
                        IRMutator::visit(loop);
                    } else {
                        Stmt body = mutate(loop->body);
                        stmt = For::make(loop->name, loop->min, loop->extent, ForType::Unrolled, loop->device_api, body.same_as(loop->body) ? loop->body : body);
                    }
                    IRMutator::visit(loop);
                }
            } else {
                stmt = loop;
            }
            bounds.pop(loop->name);
        }

        void visit(const Call *call) {
            if ((is_input_param && (call->name == producer)) || (!is_input_param && call->name == strip_stage(producer))) {
                vector<Expr> args(call->args);
                debug(3) << "The original call: " << Expr(call) << "\n";
                for (size_t i = 0; i < args.size(); ++i) {
                    args[i] = simplify(expand_expr(args[i] - stencil.stencil_mins[i], lets), false, bounds);
                }

                args.insert(args.begin(), Expr(get_template_name("stencil")));
                debug(3) << "Stencil: "
                         << Call::make(call->type, Call::sds_windowbuffer_stencil, args, Call::PureIntrinsic) << "\n";
                debug(3) << "\n";
                expr = call;
            } else {
                expr = call;
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

        const string &producer;
        const Stencil &stencil;
        const Box &producer_box;
        bool is_input_param;
        bool under_scan_level;
        Scope<Expr> lets;
        Scope<Interval> bounds;
        StreamInjectorConsumerSide(const string &producer, const Stencil &stencil, const Box &producer_box, bool is_input_param) :
                producer(producer), stencil(stencil), producer_box(producer_box), is_input_param(is_input_param), under_scan_level(false) {
            debug(3) << "Consumer side...\n";
        }
    };

    Stmt inject_stream(Stmt s, const string &producer, const Stencil &stencil, const Box &producer_box) {
        //TODO: release it later...
        //s = StreamInjectorProducerSide(producer, stencil).mutate(s);
        s = StreamInjectorConsumerSide(producer, stencil, producer_box).mutate(s);
        return s;
    }*/

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

    struct OffloadAnnotator : public IRMutator {
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
            Interval interval(simplify(mutate(expand_expr(op->min, lets)), false, bounds), simplify(expand_expr(mutate(op->min + op->extent - 1), lets), false, bounds));
            bounds.push(op->name, interval);
            if (is_offload) {
                Stmt new_body;

                new_body = OffloadPruner(lets, bounds).mutate(op->body);
                //new_body = op->body;
                vector<pair<string, Box>> offload_param;

                //TODO: later vectorize the output parameter
                Box output_box = box_provided(new_body, offload_level.func());
                Region sub_output, output_whole;
                debug(3) << "The output of offloaded body is: ";
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

                map<string, Box> images = offload_inputs(new_body, offload_level.func()), stage_bounds;
                vector<HWParam> hw_param;
                map<string, vector<HWStageEdge>> consume_graph;
                set<string> input_names, hsh;
                for (const pair<string, Box> &i : images) {
                    input_names.insert(i.first);
                    hsh.insert(i.first);
                }
                //For every producer, analyze the stencil of consumers
                while (!images.empty()) {
                    pair<string, Box> input = *images.begin();
                    images.erase(images.begin());

                    bool is_input_param = input_names.find(input.first) != input_names.end();
                    Box expanded = input.second;
                    Region whole, sub_input;
                    for (size_t i = 0; i < expanded.size(); ++i) {
                        expanded[i].min = simplify(expand_expr(expanded[i].min, lets), false, bounds);
                        expanded[i].max = simplify(expand_expr(expanded[i].max, lets), false, bounds);
                        Expr extent = simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds);
                        internal_assert(is_const(extent))
                                << "All stage or input inside offloaded body supposed to have fixed bound!\n"
                                << (simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds)) << "\n";
                        if (is_input_param) {
                            sub_input.push_back(Range(expanded[i].min, extent));
                            whole.push_back(Range(Var(input.first + ".min." + std::to_string(i)), Var(input.first + ".extent." + std::to_string(i))));
                        }
                    }
                    stage_bounds[input.first] = expanded;
                    vector<Stencil> consumers = analyze_stencil(
                            new_body,
                            input.first,
                            offload_level.func(),
                            offloads,
                            is_input_param
                    );
                    //debug(3) << (input_names.find(input.first) != input_names.end() ? "Image " : "Stage ") << input.first << ":\n";
                    Type type;
                    if (is_input_param) {
                        type = type_of_call_or_provide(new_body, input.first);
                    }
                    vector<Stmt> distributors;
                    bool partitioned = false;
                    for (Stencil &stencil : consumers) {
                        if (hsh.find(stencil.consumer) == hsh.end()) {
                            hsh.insert(input.first);
                            images[stencil.consumer] = box_of_stage(new_body, stencil.consumer);
                        } else {
                            box_of_stage(new_body, stencil.consumer);
                        }
                        internal_assert(stencil.image_mins.size() == input.second.size()) << "Image and stencil should have same dim!\n" << stencil.image_mins.size() << " != " << input.second.size() << "\n";
                        internal_assert(stencil.consumed_mins.size() == 0) << "Data corrupted!\n";
                        for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                            Expr bound_min = simplify(expand_expr(stencil.image_mins[i] - expanded[i].min, lets), false, bounds);
                            internal_assert(is_const(bound_min)) << "Consumed image should have fixed bound!\n" << bound_min << "\n";
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
                        offload_param.push_back(input);
                        hw_param.push_back(HWParam(type, input.first, whole, sub_input));
                        //TODO: If possible, we need to vectorize certain dim of both input and output parameters later
                        Stmt dp_stmt = Block::make(distributors);
                        dp_stmt = Block::make(make_holder(type, input.first, *consumers.begin()), dp_stmt);
                        for (size_t i = 0; i < input.second.size(); ++i) {
                            dp_stmt = For::make(make_iter_of_distributor(input.first, i), Expr(0), sub_input[i].extent, ForType::Serial, DeviceAPI::Host, dp_stmt);
                        }
                        if (partitioned) {
                            vector<Expr> args{Expr(append_tail("partitioned." + input.first, "buffer"))};
                            for (size_t i = 0; i < sub_input.size(); ++i) {
                                args.push_back(sub_input[i].extent);
                            }
                            Stmt window_alloc = Evaluate::make(Call::make(type, Call::sds_windowbuffer_alloc, args, Call::CallType::Intrinsic));
                            dp_stmt = Block::make(window_alloc, dp_stmt);
                        } else {
                            vector<Stmt> stream_allocs;
                            for (const Stencil &stencil : consumers) {
                                Expr stream_alloc =
                                        Call::make(type, Call::sds_stream_alloc, {Expr(make_stream_name(input.first, stencil.consumer))}, Call::CallType::Intrinsic);
                                stream_allocs.push_back(Evaluate::make(stream_alloc));
                            }
                            stream_allocs.push_back(dp_stmt);
                            dp_stmt = Block::make(stream_allocs);
                        }
                        debug(3) << input.first << " as a parameter added...\n";
                        new_body = Block::make(dp_stmt, new_body);
                    } else {
                        debug(3) << input.first << " stencil analysis done...\n";
                    }
                }
                for (const pair<string, vector<HWStageEdge>> node : consume_graph) {
                    internal_assert(!node.second.empty());
                    const string &scan_level = node.second.begin()->stencil.scan_level;
                    for (const HWStageEdge &edge : node.second) {
                        const Stencil &stencil = edge.stencil;
                        debug(3) << edge.producer << "(" << (input_names.find(edge.producer) != input_names.end() ? "input" : "stage") << ")" << " -> " << stencil.consumer << "\n"
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

                        //TODO: More case study required, maybe I can handle non-equal situation later.
                        internal_assert(stencil.scan_level == scan_level)
                                << "Scan level of stencil should be all the same!\n"
                                << stencil.scan_level << " != " << scan_level << "\n";

                        //Right now only 2d can be handled. Only the stride of moving in every dim is 1 or 0 can be handled.
                        //inject_stream(new_stmt, edge);
                    }
                }
                hw_param.push_back(HWParam(type_of_call_or_provide(new_body, offload_level.func()), offload_level.func(), output_whole, sub_output));
                //offload_param.push_back(make_pair(offload_level.func(), output_box));
                //new_body = offload_optimize(new_body, offload_param, offloads, lets, bounds, realizations);
                new_body = Offload::make(offload_level.func(), hw_param, new_body);
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

        const LoopLevel &offload_level;
        const vector <string> &offloads;
        Scope<Expr> lets;
        Scope<Interval> bounds;
		map<string, Region> realizations;

        OffloadAnnotator(const LoopLevel &ol, const vector <string> &offloads) : offload_level(ol), offloads(offloads) {}
    };

    Stmt inject_offload(Stmt s, const string &func, const vector <string> &offloads, const LoopLevel &offload_level) {
        OffloadAnnotator mutator(offload_level, offloads);
        s = mutator.mutate(s);
        return s;
    }

    Stmt offload_functions(Stmt s,
                           const vector <Function> &outputs,
                           const map <string, Function> &env) {
        for (const pair<string, Function> &function : env) {
            const vector <string> &offloads(function.second.schedule().offloaded_stages());
            if (offloads.size() != 0) {
                s = inject_offload(s, function.first, offloads, function.second.schedule().offload_level());
            }
        }
        return s;
    }

}

}
