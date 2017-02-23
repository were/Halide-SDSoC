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

        // Because storage folding runs before simplification, it's useful to
        // expand all the lets before running it. By doing this, we can analyze
        // the movement of stencils easily.
        class ExpandLets : public IRMutator {
            using IRMutator::visit;

            Scope<Expr> scope;
            Scope<Interval> bounds;

            void visit(const For *loop) {
                Expr loop_min = simplify(mutate(loop->min), false, bounds);
                Expr loop_extent = simplify(mutate(loop->extent), false, bounds);
                Expr loop_max = simplify(loop_min + loop_extent - 1, false, bounds);

                bounds.push(loop->name, Interval(loop_min, loop_max));
                Stmt body = mutate(loop->body);
                bounds.pop(loop->name);

                if (body.same_as(loop->body)) {
                    stmt = loop;
                } else {
                    if (is_const(loop_extent)) {
                        if (*as_const_int(loop_extent) == 0) {
                            stmt = Evaluate::make(0);
                        } else {
                            stmt = For::make(loop->name, loop->min, loop_extent, loop->for_type, loop->device_api, body);
                        }
                    } else {
                        stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type, loop->device_api, body);
                    }
                }
            }

            void visit(const LetStmt *op) {
                Expr value = simplify(mutate(op->value), false, bounds);

                Stmt body;
                scope.push(op->name, value);
                body = mutate(op->body);
                scope.pop(op->name);

                if (body.same_as(op->body) && value.same_as(op->value)) {
                    stmt = op;
                } else {
                    //I do not mean to make the code too redundant after expanding. Thus, unless
                    //it is constant, we do not apply the expanding of the RHS of LetStmt.
                    stmt = LetStmt::make(op->name, is_const(value) ? value : op->value, body);
                }
            }

            //Expanding the indexes of array reading and writing makes the problem super confusing,
            //so it is a bad idea to expand them, just skip them!
            void visit(const Provide *provide) {
                /*vector<Expr> values;
                bool value_changed = false;
                for (const Expr &expr : provide->values) {
                    values.push_back(mutate(expr));
                    if (!expr.same_as(values.back())) {
                        value_changed = true;
                    }
                }
                if (!value_changed) {*/
                stmt = provide;
                /*} else {
                    stmt = Provide::make(provide->name, values, provide->args);
                }*/
            }

            void visit(const Call *call) {
                expr = call;
            }

            void visit(const Realize *realize) {
                Stmt body = mutate(realize->body);
                if (body.same_as(realize->body)) {
                    stmt = realize;
                } else {
                    stmt = Realize::make(realize->name, realize->types, realize->bounds, realize->condition, body);
                }
            }

            void visit(const Variable *op) {
                if (scope.contains(op->name)) {
                    expr = scope.get(op->name);
                } else {
                    expr = op;
                }
            }

        public:
            ExpandLets() {}
        };

        class ExpandExpr : public IRMutator {
            using IRMutator::visit;
            const Scope<Expr> &scope;

            void visit(const Variable *var) {
                if (scope.contains(var->name)) {
                    expr = mutate(scope.get(var->name));
                    //debug(3) << "Fully expanded " << var->name << " -> " << expr << "\n";
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

// Perform all the substitutions in a scope
        Expr expand_expr(Expr e, const Scope<Expr> &scope) {
            ExpandExpr ee(scope);
            Expr result = ee.mutate(e);
            //debug(3) << "Expanded " << e << " into " << result << "\n";
            return result;
        }
    }

/*This function analyzes how to convert input images to stencils,
 * including the stencil size, the stencil's movement stride, and the size
 * of sub-image. */
    struct Stencil {
        std::string consumer;
        std::vector<Expr> image_mins;
        std::vector<int> image_bounds;
        std::vector<Expr> stencil_mins;
        std::vector<int> stencil_bounds;
        std::vector<int> movement;
        std::string scan_level;
        Stencil(const string &consumer,
                const Box &consumed_box,
                const Box &stencil_box,
                const vector<const For *> &traverse_loop) : consumer(consumer), movement(stencil_box.size(), 0), scan_level(traverse_loop.back()->name) {
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
                    }
                }
            }
        }
    };

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
            } /*else if (*as_const_int(loop->extent) == (int) 1){
                stmt = substitute(loop->name, loop_min, body);
                return ;
            }*/
            //debug(3) << loop->name << "'s extent " << loop->extent << "\n";
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
            internal_assert(false) << "Type not found in this body!\n";
        }
        return visitor.type;
    }

    struct MMAReplacer : IRMutator {
        //FIXME: it is somewhat creepy, but I haven't come up with a better idea

        vector<Expr> provide_args;
        void visit(const For *loop) {
            Stmt body = loop->body;
            if (!is_zero(loop->min)) {
                body = substitute(loop->name, Var(loop->name) + loop->min, body);
            }
            bounds.push(loop->name, Interval(Expr(0), loop->extent));
            body = mutate(loop->body);
            bounds.pop(loop->name);
            if (is_zero(loop->min) && body.same_as(loop->body)) {
                stmt = loop;
            } else {
                stmt = For::make(loop->name, Expr(0), loop->extent, loop->for_type, loop->device_api, body);
            }
        }

        void visit(const LetStmt *let) {
            Expr value = simplify(expand_expr(mutate(let->value), lets), false, bounds);
            lets.push(let->name, value);
            IRMutator::visit(let);
            lets.pop(let->name);
        }

        void visit(const Provide *provide) {
            if (mins.find(provide->name) != mins.end()) {
                vector<Expr> values(provide->values);
                for (size_t i = 0; i < values.size(); ++i) {
                    values[i] = mutate(values[i]);
                }
                vector<Expr> args(provide->args);
                for (size_t i = 0; i < args.size(); ++i) {
                    args[i] = simplify(expand_expr(mutate(args[i]) - mins[provide->name][i], lets), false, bounds);
                }
                if (provide->name == output_stage) {
                    if (realizations.find(provide->name) != realizations.end()) {
                        for (size_t i = 0; i < realizations[provide->name].size(); ++i) {
                            args[i] += realizations[provide->name][i].min;
                        }
                    } else {
                        stmt = Provide::make(provide->name, values, args);
                        int stride = 1;
                        for (size_t i = 0; i < args.size(); ++i) {
                            stmt = LetStmt::make(provide->name + ".stride." + std::to_string(i), stride, stmt);
                            stmt = LetStmt::make(provide->name + ".extent." + std::to_string(i), params.back().sub[i].extent, stmt);
                            stmt = LetStmt::make(provide->name + ".min." + std::to_string(i), 0, stmt);
                            stride *= (int) *as_const_int(params.back().sub[i].extent);
                        }
                        return ;
                    }
                }
                stmt = Provide::make(provide->name, values, args);
            } else {
                IRMutator::visit(provide);
            }
        }

        void visit(const Call *call) {
            if (mins.find(call->name) != mins.end()) {
                vector<Expr> args(call->args);
                for (size_t i = 0; i < args.size(); ++i) {
                    args[i] = simplify(expand_expr(mutate(args[i]) - mins[call->name][i], lets), false, bounds);
                }
                expr = Call::make(call->type, call->name, args, call->call_type, call->func, call->value_index, call->image, call->param);
            } else if (call->call_type == Call::CallType::Halide || call->call_type == Call::CallType::Image){
                vector<Expr> args(call->args);
                for (size_t i = 0; i < args.size(); ++i) {
                    args[i] = simplify(expand_expr(mutate(args[i]), lets), false, bounds);
                }
                expr = Call::make(call->type, call->name, args, call->call_type, call->func, call->value_index, call->image, call->param);
            } else {
                expr = call;
            }
        }

        /*void visit(const Realize *realize) {
            Stmt body = mutate(realize->body);
            if (offloaded_stages.find(realize->name) != offloaded_stages.end()) {
                Region new_region;
                for (const Range &range : realize->bounds) {
                    user_assert(is_const(range.extent)) << "All of the inner stages should be fixing-bounded!\n";
                    new_region.push_back(Range(0, range.extent));
                }
                //stmt = realize;
                stmt = Realize::make(realize->name, realize->types, new_region, realize->condition, body);
            } else {
                if (!body.same_as(realize->body)) {
                    stmt = Realize::make(realize->name, realize->types, realize->bounds, realize->condition, body);
                } else {
                    stmt = realize;
                }
            }
        }*/

        const vector<HWParam> &params;
        map<string, vector<Expr>> mins;
        set<string> offloaded_stages;
        Scope<Expr> &lets;
        Scope<Interval> &bounds;
        const string &output_stage;
        map<string, Region> &realizations;

        MMAReplacer(const vector<HWParam> &params, const vector<string> &offloads, Scope<Expr> &lets, Scope<Interval> &bounds, map<string, Region> &realizations) : params(params), lets(lets), bounds(bounds), output_stage(params.back().name), realizations(realizations) {
            for (const string &stage_name : offloads) {
                offloaded_stages.insert(stage_name);
            }
            for (size_t i = 0; i < params.size(); ++i) {
                vector<Expr> offset;
                for (size_t j = 0; j < params[i].dim(); ++j) {
                    offset.push_back(params[i].sub[j].min);
                }
                mins[params[i].name] = offset;
            }
            //debug(3) << "MMA init done\n";
        }
    };

    Stmt offload_optimize(Stmt s, const vector<pair<string, Box>> &param, const vector<string> &offloads, Scope<Expr> &lets, Scope<Interval> &bounds, map<string, Region> &realizations) {
        vector<HWParam> hw_param;
        for (size_t i = 0; i < param.size(); ++i) {
            const string &name = param[i].first;
            const Box &box = param[i].second;
            Region full, sub;
            for (size_t j = 0; j < box.size(); ++j) {
                full.push_back(Range(
                        Var(name + ".min." + std::to_string(j)), Var(name + ".extent." + std::to_string(j))
                ));
            }
            for (size_t j = 0; j < box.size(); ++j) {
                sub.push_back(Range(box[j].min, simplify(box[j].max - box[j].min + 1)));
            }
            hw_param.push_back(HWParam(type_of_call_or_provide(s, name), name, full, sub));
            //debug(3) << "Parameter type: "<< hw_param.back().type << "\n";
        }
        s = MMAReplacer(hw_param, offloads, lets, bounds, realizations).mutate(s);
        //debug(3) << "Offload optimized...\n";
        return Offload::make(param.back().first, hw_param, s);
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

    struct StreamInjectorConsumerSide : IRMutator {
        string get_template_name(const string &template_name) {
            return producer + (is_input_param ? ".to." : "to.") + stencil.consumer + template_name;
        }

        void visit(const For *loop) {
            bounds.push(loop->name, Interval(loop->min, simplify(expand_expr(loop->min + loop->extent - 1, lets), false, bounds)));
            if (starts_with(loop->name, stencil.consumer)) {
                if (loop->name == stencil.scan_level) {
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
                    }*/
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

    Stmt inject_stream(Stmt s, const string &producer, const Stencil &stencil, const Box &producer_box, bool is_input_param) {
        //TODO: release it later...
        //s = StreamInjectorProducerSide(producer, stencil).mutate(s);
        s = StreamInjectorConsumerSide(producer, stencil, producer_box, is_input_param).mutate(s);
        return s;
    }

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
                    for (const Stencil stencil : consumers) {
                        if (hsh.find(stencil.consumer) == hsh.end()) {
                            hsh.insert(input.first);
                            images[stencil.consumer] = box_of_stage(new_body, stencil.consumer);
                        } else {
                            box_of_stage(new_body, stencil.consumer);
                        }
                        internal_assert(stencil.image_mins.size() == input.second.size()) << "Image and stencil should have same dim!\n" << stencil.image_mins.size() << " != " << input.second.size() << "\n";
                        for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                            Expr bound_min = simplify(expand_expr(stencil.image_mins[i] - expanded[i].min, lets), false, bounds);
                            internal_assert(is_const(bound_min)) << "Consumed image should have fixed bound!\n" << bound_min << "\n";
                        }
                        consume_graph[stencil.consumer].push_back(HWStageEdge(input.first, stencil));
                    }
                    if (is_input_param) {
                        offload_param.push_back(input);
                        hw_param.push_back(HWParam(type_of_call_or_provide(new_body, input.first), input.first, whole, sub_input));
                        debug(3) << input.first << " as a parameter added...\n";
                    } else {
                        debug(3) << input.first << " stencil analysis done...\n";
                    }
                }
                for (const pair<string, vector<HWStageEdge>> node : consume_graph) {
                    internal_assert(!node.second.empty());
                    const string &scan_level = node.second.begin()->stencil.scan_level;
                    for (const HWStageEdge &edge : node.second) {
                        const Stencil &stencil = edge.stencil;
                        //TODO: More case study required, maybe I can handle non-equal situation later.
                        internal_assert(stencil.scan_level == scan_level) << "Scan level of stencil should be all the same!\n";
                        debug(3) << edge.producer << "(" << (input_names.find(edge.producer) != input_names.end() ? "input" : "stage") << ")" << " -> " << stencil.consumer << "\n"
                                 << "Stecil bound: ";
                        for (const int &bound : stencil.stencil_bounds) {
                            debug(3) << "[" << bound << "]";
                        }
                        const Box &expanded = stage_bounds[edge.producer];
                        internal_assert(expanded.size() == stencil.image_mins.size()) << "WTF?!\n";
                        debug(3) << "\n" << "Consumed bound: ";
                        for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                            Expr bound_min = simplify(expand_expr(stencil.image_mins[i] - expanded[i].min, lets), false, bounds);
                            debug(3) << "[" << bound_min << ", " << stencil.image_bounds[i] << "]";
                        }
                        debug(3) << "\n" << "Movement: ";
                        for (const int &stride : stencil.movement) {
                            debug(3) << "[" << stride << "]";
                        }
                        debug(3) << "\nScan level: " << stencil.scan_level << "\n";
                        //TODO: right now, only 2 conditions can be handled: no overlapping, and all movement is one.
                        for (size_t i = 0; i < stencil.stencil_bounds.size(); ++i) {
                        }
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
        s = ExpandLets().mutate(s);

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
