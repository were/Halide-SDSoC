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
            // 'image' is the producer
            // 'consumer' is the consumer
            string consumer;

            // 'image_mins' is the expr for the index of the top-left hand corner of the image
            vector <Expr> image_mins;
            // 'consumed_mins' is a multi-dimensional integer offset from 'image_mins'.
            // 'image_mins + consumed_mins' is the top-left hand corner of the region consumed
            // by this stencil. 'consumed_mins' is not calculated in 'analyze_stencil'
            vector<int> consumed_mins;
            // 'image_bounds' is the bounds of the consumed region
            vector<int> image_bounds;
            // 'stencil_mins' is the top-left hand corner of the stencil sub-image which moves
            // across the input image, it changes with each iteration of the innermost loop
            vector <Expr> stencil_mins;
            // 'stencil_bounds' is the bounds of the stencil sub-image
            vector<int> stencil_bounds;

            // 'stencil_stride' is the stride of the stencil movement in each dimension. It's different from
            // regular pixel stride in Halide
            vector<int> stencil_stride;
            // 'traverse' is the names of the loops which move the stencil
            vector <string> traverse;
            // 'scan_level' is the name of the innermost loop (moves the stencil in innermost dim)
            string scan_level;

            // Producer's info
            bool is_param;
            Type type;

            // Constructor
            Stencil(const string &consumer,
                    const Box &consumed_box,
                    const Box &stencil_box,
                    const vector<const For *> &traverse_loop
                   )
                  : consumer(consumer), stencil_stride(stencil_box.size(), 0),
                    traverse(stencil_box.size(), "<NonSerial>"),
                    scan_level(traverse_loop.back()->name),
                    is_param(false)
            {
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
                internal_assert(stencil_stride.size() == stencil_mins.size()) << "Movement init failed?!\n" << stencil_stride.size()
                                                                        << " != " << stencil_mins.size() << "\n";
                for (const For *loop : traverse_loop) {
                    for (size_t i = 0; i < stencil_mins.size(); ++i) {
                        const Expr &expr = stencil_mins[i];
                        if (expr_uses_var(expr, loop->name)) {
                            Expr stride = simplify(expr - substitute(loop->name, Var(loop->name) - 1, expr));
                            internal_assert(is_const(stride)) << "Stencil should have constant stride of movement!\n";
                            internal_assert(i < stencil_stride.size()) << i << " >= " << stencil_stride.size() << "\n";
                            stencil_stride[i] = (int) *as_const_int(stride);
                            traverse[i] = loop->name;
                        }
                    }
                }
            }

            // Currently we assume if a stencil does not move it is the weight array of a convolution
            // so it should be fully partitioned
            bool is_fully_partitioned() const {
                for (int i : stencil_stride) {
                    if (i) {
                        return false;
                    }
                }
                return true;
            }

            // If there is only 1 pixel in the stencil (not counting vectorized dimensions)
            bool is_single_pixel() const {
                for (size_t i = 0; i < stencil_bounds.size(); ++i) {
                    // stencil_stride[i] == 0 indicates ith dim is vectorized
                    if (stencil_bounds[i] != 1 && stencil_stride[i] != 0) {
                        return false;
                    }
                }
                return !is_fully_partitioned();
            }

            /* If this dim is reqiured to be vectorized... */
            bool is_vectorized_dim(int dim) const {
                return stencil_stride[dim] == 0 && !is_fully_partitioned();
            }

            // If any dim is vectorized
            bool is_vectorize_required() const {
                if (is_fully_partitioned()) {
                    return false;
                }
                for (auto i : stencil_stride) {
                    if (!i) {
                        return true;
                    }
                }
                return false;
            }

            // Checks if *this and stencil has the same access pattern
            bool same_access_pattern(const Stencil &stencil) const {
                if (image_mins.size() == stencil.image_mins.size()) {
                    size_t size = image_mins.size();
                    for (size_t i = 0; i < size; ++i) {
                        if (stencil_bounds[i] != stencil.stencil_bounds[i]) {
                            return false;
                        }
                    }
                    for (size_t i = 0; i < size; ++i) {
                        if (stencil_stride[i] && stencil.stencil_stride[i]) {
                            // This check exists to merge 2 stencils in unsharp
                            // FIXME: Doesn't seem to make sense in general
                            if (consumed_mins[i] % stencil_stride[i] != stencil.consumed_mins[i] % stencil.stencil_stride[i]) {
                                return false;
                            }
                        } else if (stencil_stride[i] != stencil.stencil_stride[i]) {
                            return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
            }

            // This width is the width of the window buffer needed for the stencil
            int stencil_width() const {
                for (size_t i = 0; i < stencil_bounds.size(); ++i) {
                    if (!is_vectorized_dim(i))
                        return stencil_bounds[i];
                }
                internal_assert(false);
                return -1;
            }

            int sub_img_width() const {
                for (size_t i = 0; i < stencil_bounds.size(); ++i) {
                    if (!is_vectorized_dim(i))
                        return image_bounds[i];
                }
                internal_assert(false);
                return -1;
            }

            // Height of the window buffer
            int stencil_height() const {
                for (int i = stencil_bounds.size() - 1; i >= 0; --i) {
                    if (!is_vectorized_dim(i)) {
                        if (i == 0)
                          return 1;
                        for (int j = i - 1; j >= 0; --j) {
                            if (!is_vectorized_dim(j))
                                return stencil_bounds[i];
                        }
                        return 1;
                    }
                }
                internal_assert(false);
                return -1;
            }

            int sub_img_height() const {
                for (int i = stencil_bounds.size() - 1; i >= 0; --i) {
                    if (!is_vectorized_dim(i)) {
                        if (i == 0)
                            return 1;
                        for (int j = i - 1; j >= 0; --j) {
                            if (!is_vectorized_dim(j))
                                return image_bounds[i];
                        }
                        return 1;
                    }
                }
                internal_assert(false);
                return -1;
            }

            // Number of pixels required in the big ap_int needed to vectorize the stencil
            int stencil_wrapped() const {
                int res = 1;
                for (size_t i = 0; i < stencil_bounds.size(); ++i) {
                    if (is_vectorized_dim(i))
                        res *= stencil_bounds[i];
                }
                return res;
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
                if (!stencil.is_vectorized_dim(i)) {
                    Expr iter_var = Var(make_iter_of_distributor(producer, i));
                    Expr cur_cond = iter_var >= Expr(stencil.consumed_mins[i]) &&
                                    iter_var < Expr(stencil.consumed_mins[i] + stencil.image_bounds[i]);
                    condition = condition.defined() ? condition && cur_cond : cur_cond;
                }
            }
            Expr stream_name = Expr(make_stream_name(producer, stencil.consumer));
            Expr holder = Call::make(type, Call::sds_tmp_access, {Expr(producer)}, Call::CallType::Intrinsic);
            Expr sender = Call::make(Type(), Call::sds_stream_write, {stream_name, holder}, Call::CallType::Intrinsic);
            return IfThenElse::make(condition, Evaluate::make(sender));
        }

        /* I used to suppose that we can partition the input parameter directly, but I am wrong. Because due to the bug of
         * SDSoC (older than 2016.2), so far, we cannot partition the input parameter. Also, loading data to a
         * hls::Window will cause address mapping error! If this bug is fixed, maybe later we no long need this function,
         * just partition the input parameter!
         */
        Stmt make_partitioner(Type type, const string &producer, size_t dim) {
            vector <Expr> args{Expr(make_window_name(producer))};
            for (size_t i = 0; i < dim; ++i) {
                Expr iter_var = Var(make_iter_of_distributor(producer, i));
                args.push_back(iter_var);
            }
            Expr holder = Call::make(type, Call::sds_tmp_access, {Expr(producer)}, Call::CallType::Intrinsic);
            vector <Expr> window_args(args.begin() + 1, args.end());
            std::reverse(window_args.begin(), window_args.end());
            window_args.push_back(holder);
            window_args.insert(window_args.begin(), args[0]);
            return Evaluate::make(
                    Call::make(type, Call::sds_windowbuffer_access, window_args, Call::CallType::Intrinsic));
        }

        //@}

        /* I think simply by doing a topological sort we can infer the depth of each stream easily, but right now I decide
         * to left this burden to programmers. */
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

        // Check if all the stencils' consumer access patterns are the same so that they could be merged
        // 'merged' means that a single producer sends data to both consumers
        bool can_be_merged(const vector<Stencil> &stencil_list) {
            for (size_t i = 1; i < stencil_list.size(); ++i) {
                if (!stencil_list[i].same_access_pattern(stencil_list[0])) {
                    return false;
                }
            }
            return true;
        }

        int upper_bit(int x) {
            int i;
            for (i = 0; (1ull << i) < (uint64_t) x; ++i);
            return 1 << i;
        }

        Type pad_lanes(int lanes, Type type) {
            while (upper_bit(lanes * type.bits()) != lanes * type.bits()) {
                ++lanes;
            }
            internal_assert(lanes * type.bits() <= 1024);
            return type.with_lanes(lanes);
        }
    }

    /* A producer-consumer relationship inside the dependency graph. */
    struct HWStageEdge {
        string producer;
        vector<int> producer_extents;
        Stencil stencil;

        HWStageEdge(const string &producer, const vector<int> &producer_extents, const Stencil &stencil) :
                producer(producer), producer_extents(producer_extents), stencil(stencil) {}

        string pure_name() const {
            return stencil.is_param ? producer : strip_stage(producer);
        }
    };

    // -------------------------------------------------------------------------
    // Main analysis pass for stencil patterns
    // It analyzes producer-consumer pairs and adds those which form 
    // a stencil pattern to a Stencil list
    // -------------------------------------------------------------------------
    struct StencilAnalyzer : IRVisitor {
        using IRVisitor::visit;

        // For constant propagation
        void visit(const LetStmt *let) {
            lets.push(let->name, let->value);
            IRVisitor::visit(let);
            lets.pop(let->name);
        }

        // Maintain list of loops we are currently in
        void visit(const For *op) {
            bounds.push(op->name,
                        Interval(op->min, simplify(expand_expr(op->min + op->extent - 1, lets), false, bounds)));
            for_stack.push_back(op);
            IRVisitor::visit(op);
            for_stack.pop_back();
            bounds.pop(op->name);
        }

        // Main analysis visitor
        void visit(const Provide *provide) {
            // env is the list of stages offloaded to HW
            if (env.find(provide->name) != env.end()) {
                vector<const For *> traverse_loop;
                int pre_dim = for_stack.size() + 1;
                //FIXME: the code here may need refactor later,
                //cuz the nested loops make the result of traverse loop from outer to the inner,
                //which conflicts the design concept of halide
                for (const For *loop : for_stack) {
                    // counts how many dimensions depend on this loop induction var
                    int n_dependent_dims = 0;
                    for (size_t i = 0; i < provide->args.size(); ++i) {
                        // provide->args are the indices to a store buf[x][y] = ...
                        // loop->name is the induction variable of the loop
                        if (expr_uses_var(provide->args[i], loop->name)) {
                            if (loop->for_type == ForType::Serial) {
                                // check that each loop var is used in one dimension
                                user_assert(++n_dependent_dims <= 1)
                                        << Stmt(provide)
                                        << "\nMore than one dimension depend on the traverse loop, abort!\n";
                                // check that the traversal order is outer loop major
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

                // Only used to support 2nd way of writing a convolution
                // Checks that each stage (s0,s1) only sends data to the immediate next stage
                // so there's no data from s0 (pure def) to s2 (consumer)
                if (!is_input_param) {
                    internal_assert(env.find(producer_name) != env.end()) << producer_name
                                                                          << " is not an offloaded stage!\n";
                    if (producer_name == strip_stage(consumer_prefix)) {
                        if (get_stage(producer) + 1 != get_stage(consumer_prefix)) {
                            //If it is not the last stage of update, this stage can only send data to next update stage
                            if (get_stage(producer) == get_stage(consumer_prefix)) {
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
                      //Only the last stage of update can send data to its consumers...
                        return;
                    }
                }

                // If a consumer has multiple producers, check that the produce_box for each producer is identical
                // stencil_box is the region of the producer required for 1 iter of the inner loop of the consumer
                Box stencil_box = box_required(traverse_loop.back()->body, producer_name);
                {
                    // produce_box is the region computed in 1 iter of the inner loop of the producer
                    Box produce_box = box_provided(traverse_loop.back()->body, strip_stage(consumer_prefix));
                    // rate is the integer dims of produce_box
                    vector<int> rate;
                    for (size_t i = 0; i < produce_box.size(); ++i) {
                        Expr extent = simplify(expand_expr(produce_box[i].max - produce_box[i].min + 1, lets), false, bounds);
                        internal_assert(is_const(extent)) << "Producing rate should be constant bounded!\n";
                        rate.push_back((int) *as_const_int(extent));
                    }
                    // produce_rate is a map from the consumer name to its produce_box
                    // check current produce box against stored produce box of this consumer
                    if (producing_rate.find(consumer_prefix) == producing_rate.end()) {
                        /*debug(3) << "Producing rate of " << consumer_prefix << "is: ";
                        for (size_t i = 0; i < rate.size(); ++i) {
                            debug(3) << "[" << rate[i] << "]";
                        }
                        debug(3) << "\n";*/
                        //If the producing rate is not inferred, infer it!
                        producing_rate[consumer_prefix] = rate;
                    } else {
                        //Else, check if it is the same.
                        const vector<int> &the_rate = producing_rate[consumer_prefix];
                        internal_assert(the_rate.size() == rate.size());
                        for (size_t i = 0; i < the_rate.size(); ++i) {
                            internal_assert(the_rate[i] == rate[i]);
                        }
                    }
                }

                // Add an element to the stencil list
                if (stencil_box.size() != 0) {
                    debug(3) << get_prefix(traverse_loop.front()->name) << " consumes " << producer << "\n";
                    debug(3) << Stmt(provide) << "\n";
                    debug(3) << "Traverse loop(s):\n";
                    for (auto loop : traverse_loop) {
                        debug(3) << loop->name << "\n";
                    }
                    debug(3) << "\n";
                    // consumed_box here is the entire image subregion used by all loops over the consumer
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
                    // add this producer-consumer pair to the stencil list
                    res.push_back(Stencil(consumer_prefix, consumed_box, stencil_box, traverse_loop));
                    vector <string> traverse_loop_names;
                    for (vector<const For *>::reverse_iterator riter = traverse_loop.rbegin();
                         riter != traverse_loop.rend(); ++riter) {
                        traverse_loop_names.push_back((*riter)->name);
                    }
                    // traverse_loop_names is the list of loops over the consumer, inner loop first
                    traverse_collection[consumer_prefix] = traverse_loop_names;
                }
            }
        }

        string producer;
        const map<string, Function> &env;
        map <string, vector<int>> &producing_rate;
        bool is_input_param;
        vector<const For *> for_stack;
        Scope<Expr> lets;
        Scope<Interval> bounds;
        map <string, vector<string>> traverse_collection;

        vector <Stencil> res;

        StencilAnalyzer(const string &producer, const string &func, const map<string, Function> &env,
                        map<string, vector<int>> &producing_rate,
                        bool is_input_param)
                : producer(producer), env(env), producing_rate(producing_rate), is_input_param(is_input_param) {}

    };

    vector <Stencil>
    analyze_stencil(Stmt s, const string &producer, const string &offload_func, const map<string, Function> &env,
                    bool is_input_param, map <string, vector<string>> &traverse_collection, map<string, vector<int>> &producing_rate)
    {
        StencilAnalyzer collector(producer, offload_func, env, producing_rate, is_input_param);
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

        // Gets rid of zero-extent loops and asserts loops are constant bounds
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

        // Keep track of lets for constant propagation
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

        void visit(const Realize *realize) {
            already.insert(realize->name);
            IRVisitor::visit(realize);
        }

        void visit(const Call *call) {
            if (call->call_type == Call::Halide || call->call_type == Call::Image) {
                bool is_input = true;
                for (const auto & name : already) {
                    // Match the start of the name instead of the entire name
                    // Necessary for multi-output function
                    // E.g. f(x,y) = {0,0}
                    //FIXME: if (call->name==name || starts_with(call->name, name+".")) {
                    if (starts_with(call->name, name)) {
                        is_input = false;
                        break;
                    }
                }
                if (is_input) {
                    res.insert(call->name);
                }
            }
            IRVisitor::visit(call);
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

    // Function to call the OffloadInputs pass to collect inputs to the hardware function
    // and perform bounds inference on them
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
        debug(3) << "Inputs are:\n";
        for (auto &input : collector.res) {
            debug(3) << input << "\n";
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

    // This visitor is used to inject the declration and the writes to the streams
    // which connect stages in the HW
    struct StreamAllocInjector : IRMutator {
        using IRMutator::visit;

        // Replace Realize nodes which match the name we are looking for
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
                    // Construct the if condition which guards when we send pixels to a consumer
                    // stencil.consumed_mins is the consumed subregion
                    Expr send_condition;
                    for (size_t i = 0, j = 0; i < stencil.consumed_mins.size(); ++i) {
                        if (!stencil.is_vectorized_dim(i)) {
                            Expr condition = Var(traverse[j]) >= stencil.consumed_mins[i] &&
                                             Var(traverse[j]) < stencil.consumed_mins[i] + stencil.image_bounds[i];
                            send_condition = send_condition.defined() ? send_condition && condition : condition;
                            ++j;
                        }
                    }
                    string name = stencil.is_param ? producer : strip_stage(producer);
                    // 'holder' is the temp var used to hold the current pixel, it is defined somewhere else
                    Expr holder = Call::make(stencil.type, Call::sds_tmp_access, {Expr(name)}, Call::Intrinsic);
                    Expr sender = Call::make(Type(), Call::sds_stream_write,
                                             {Expr(make_stream_name(producer, stencil.consumer)), holder},
                                             Call::CallType::Intrinsic);
                    body = Block::make(body, IfThenElse::make(send_condition, Evaluate::make(sender)));
                    //debug(3) << "Sender injected body!\n" << body << "\n";
                }
                for_stack.pop_back();

                // Inject declaration of the stream before the loop nest
                stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type, loop->device_api, body);
                if (for_stack.empty()) {
                    Stmt allocator = Evaluate::make(
                            Call::make(type, Call::sds_stream_alloc, {make_stream_name(producer, consumer),
                                                                      get_stream_depth(producer, consumer, env)},
                                       Call::CallType::Intrinsic));
                    debug(3) << "Stream Injector: " << make_stream_name(producer, consumer) << " " << type << "\n";
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
                        Call::make(value.type(), Call::sds_tmp_access, {Expr(strip_stage(producer)), value},
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

    // Pass the innermost loop's body (the body containing pipeline pragma) to this mutator
    // and transform it to HLS specialized code.
    struct ConsumerMaker : IRMutator {
        using IRMutator::visit;

        // Calls are loads, we replace loads with accesses to the appropriate data structure
        void visit(const Call *call) {
            if (call->name == strip_stage(consumer)) {
                // This branch examines self update, when the load is from the update def of the current function
                // For a function with self-reference (i.e. f(x) = f(x) + 1) there is an update def
                // TODO: Fix it later. There's no self consuming example right now.
                expr = Call::make(call->type, Call::sds_tmp_access, {Expr(strip_stage(consumer))},
                                  Call::Intrinsic);
                return;
            }

            for (const HWStageEdge &edge : edges) {
                const Stencil &stencil = edge.stencil;
                // Check the call is a load from the producer of this consumer
                if ((stencil.is_param && edge.producer == call->name) ||
                    (!stencil.is_param && strip_stage(edge.producer) == call->name)) {
                    vector<Expr> stencil_args;
                    Expr vectorized_args = 0;
                    int vectorized_stride= 1;
                    vector <Expr> args(call->args);

                    // Calculate index for stencil access with possible bit slicing for vectorized dim
                    for (size_t i = 0; i < args.size(); ++i) {
                        args[i] = simplify(expand_expr(args[i] - stencil.stencil_mins[i], lets), false, bounds);
                        if (stencil.is_vectorized_dim(i)) {
                            vectorized_args += args[i] * vectorized_stride;
                            vectorized_stride *= stencil.stencil_bounds[i];
                        } else {
                            stencil_args.push_back(args[i]);
                        }
                    }

                    if (stencil.is_single_pixel()) {
                        // Non-vectorized, do a simple access from a register
                        if (stencil.stencil_wrapped() == 1) {
                            expr = Call::make(call->type, Call::sds_tmp_access, {Expr(edge.pure_name())},
                                              Call::CallType::Intrinsic);
                        }
                        // Vectorized, perform bit slice
                        else {
                            expr = Call::make(
                                    call->type, Call::sds_bit_range,
                                    {Call::make(stencil.is_param ? pad_lanes(stencil.stencil_wrapped(), call->type) :
                                                call->type.with_lanes(stencil.stencil_wrapped()),
                                                Call::sds_tmp_access, {Expr(edge.pure_name())},
                                                Call::CallType::Intrinsic), vectorized_args},
                                    Call::CallType::Intrinsic
                            );
                        }
                        //debug(3) << "Is only one!\n";
                    } else if (stencil.is_fully_partitioned()) {
                        // Here we load from a partitioned array
                        internal_assert(stencil_args.size() == edge.producer_extents.size());
                        Expr index = 0;
                        int stride = 1;
                        for (size_t i = 0; i < edge.producer_extents.size(); ++i) {
                            index += stride * stencil_args[i];
                            stride*= edge.producer_extents[i];
                        }
                        expr = Load::make(edge.stencil.type, "buffered$$" + call->name, index, Buffer<>(), Parameter(), const_true());
                        debug(3) << "Partitioned buffer!\n";
                    } else {
                        // Here we load from a window buffer
                        internal_assert(stencil_args.size() <= 2);
                        if (stencil_args.size() == 1) {
                            stencil_args.push_back(0);
                        } else if (stencil_args.size() == 0) {
                            internal_assert(false);
                        }
                        internal_assert(stencil_args.size() == 2);
                        std::reverse(stencil_args.begin(), stencil_args.end());

                        stencil_args.insert(stencil_args.begin(), make_window_name(edge.producer, stencil.consumer));
                        if (stencil.stencil_wrapped() == 1) {
                            expr = Call::make(call->type, Call::sds_windowbuffer_access, stencil_args, Call::CallType::Intrinsic);
                        } else {
                            // Vectorized dim, perform bit slice
                            //stencil_args.push_back(vectorized_args);
                            expr = Call::make(
                                    call->type, Call::sds_bit_range,
                                    {Call::make(call->type.with_lanes(stencil.stencil_wrapped()),
                                              Call::sds_windowbuffer_access, stencil_args, Call::CallType::Intrinsic), vectorized_args},
                                    Call::CallType::Intrinsic
                            );
                        }
                        debug(3) << "Stencil access!\n";
                    }

                    debug(3) << "Stencil access: " << expr << "\n\n";
                    return;
                }
            }
            //expr = call;
            IRMutator::visit(call);
        }

        // Provides are stores, write the value to a register, which is later written to a stream
        void visit(const Provide *provide) {
            if (provide->name == strip_stage(consumer)) {
                internal_assert(provide->values.size() == 1) << "Anything wrong with the providers' values?!\n";
                Expr value = mutate(provide->values[0]);
                if (for_stack.empty()) {
                    // We are in the innermost loop body, no loops mean no vectorization
                    stmt = Evaluate::make(
                            Call::make(value.type(), Call::sds_tmp_access, {Expr(strip_stage(consumer)), value},
                                       Call::Intrinsic)
                    );
                } else {
                    // There is vectorization, we must perform bit slicing
                    Expr stride = 1, index = 0;
                    for (int i = for_stack.size() - 1; i >= 0; --i) {
                        const For *loop = for_stack[i];
                        index += stride * (Var(loop->name) - loop->min);
                        stride *= loop->extent;
                    }
                    index = simplify(expand_expr(index, lets), false, bounds);
                    stride = simplify(expand_expr(stride, lets), false, bounds);
                    internal_assert(is_const(stride));
                    int lanes = (int) *as_const_int(stride);
                    Expr holder_writer =
                            Call::make(is_last_stage ? pad_lanes(lanes, value.type()) : value.type().with_lanes(lanes),
                                                Call::sds_tmp_access,
                                                {Expr(strip_stage(consumer))},
                                                Call::Intrinsic
                            );
                    if (lanes != 1) {
                        holder_writer = Call::make(
                                value.type(),
                                Call::sds_bit_range,
                                {holder_writer, index, value},
                                Call::Intrinsic
                        );
                    }
                    stmt = Evaluate::make(holder_writer);
                    if (is_last_stage) {
                        debug(3) << "last stages' type: " << pad_lanes(lanes, value.type());
                    }
                }
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
            for_stack.push_back(loop);
            Stmt body = mutate(loop->body);
            stmt = For::make(loop->name, loop->min, loop->extent, loop->for_type/**/, loop->device_api,
                             body.same_as(loop->body) ? loop->body : body);
            for_stack.pop_back();
            bounds.pop(loop->name);
        }

        const vector <HWStageEdge> &edges;
        Scope<Expr> &lets;
        Scope<Interval> &bounds;
        string consumer;
        bool is_last_stage;
        vector<const For *> for_stack;

        ConsumerMaker(const vector <HWStageEdge> &edges, Scope<Expr> &lets, Scope<Interval> &bounds, bool is_last_stage) :
            edges(edges), lets(lets), bounds(bounds), is_last_stage(is_last_stage) {
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
                    // Make specialized body
                    body = ConsumerMaker(edges, lets, bounds, is_last_stage).mutate(loop->body);
                    // 'start_condition' is the condition to begin generate a new pixel
                    Expr start_condition;
                    for (int i = for_stack.size() - 1; i >= 0; --i) {
                        Expr condition = Var(for_stack[i]) >= Expr(0);
                        start_condition = start_condition.defined() ? start_condition && condition : condition;
                    }
                    // If this is the last stage, we want to do a memory write to the output argument.
                    // However, there are issues when we have a vectorized dimension.
                    // Halide defines vectorization as a vectorized store (storing 3 elems at once) but we
                    // only want to store a single elem (large ap_int with 3 pixels).
                    // This leads to assert failures later in the lowering process.
                    // For now we use an IR stream write instead of a memory write.
                    if (is_last_stage) {
                        debug(3) << "last stage's type: " << output_type << "\n";
                        body = Block::make(
                                {
                                        Evaluate::make(Call::make(pad_lanes(output_type.lanes(), output_type), Call::sds_tmp_alloc,
                                                                  {Expr(strip_stage(consumer))}, Call::CallType::Intrinsic)
                                        ),
                                        body,
                                        Evaluate::make(Call::make(output_type, Call::sds_stream_write,
                                                                  {
                                                                          Expr(strip_stage(consumer)),
                                                                          Call::make(output_type, Call::sds_tmp_access,
                                                                                     {Expr(strip_stage(consumer))},
                                                                                     Call::CallType::Intrinsic)
                                                                  }, Call::CallType::Intrinsic))
                                }
                        );
                    }
                    // Wrap the write in the start condition
                    body = IfThenElse::make(start_condition, body);

                    // Emit code to update line and window buffers
                    for (const HWStageEdge &edge : edges) {
                        const Stencil &stencil = edge.stencil;
                        // If the current stencil is not fully partitioned there is a stream and line buffering
                        if (!stencil.is_fully_partitioned()) {
                            Expr load_condition;
                            for (size_t i = 0; i < stencil.traverse.size(); ++i)
                                if (stencil.traverse[i] != "<NonSerial>") {
                                    Expr condition = Var(stencil.traverse[i]) >= Expr(-stencil.stencil_bounds[i] + 1) && Var(stencil.traverse[i]) < (stencil.image_bounds[i] - stencil.stencil_bounds[i] + 1);
                                    debug(3) << "LOAD CONDITION: " << condition << "\n";
                                    load_condition = load_condition.defined() ? load_condition && condition : condition;
                                }
                            //s.read()
                            Expr stream_reader = Call::make(
                                    stencil.is_param ? pad_lanes(stencil.stencil_wrapped(), stencil.type)
                                                     : stencil.type.with_lanes(stencil.stencil_wrapped()),
                                    Call::sds_stream_read,
                                    {Expr(make_stream_name(edge.producer, stencil.consumer))},
                                    Call::Intrinsic
                            );
                            //T holder = s.read()
                            Stmt stream_holder = Evaluate::make(
                                    Call::make(
                                            stencil.is_param ? pad_lanes(stencil.stencil_wrapped(), stencil.type)
                                                             : stencil.type.with_lanes(stencil.stencil_wrapped()),
                                            Call::sds_tmp_access,
                                            {Expr(edge.pure_name()), stream_reader},
                                            Call::Intrinsic));
                            if (stencil.is_single_pixel()) {
                                //A trivial condition which requires no linebuffer
                                body = Block::make(IfThenElse::make(load_condition, stream_holder), body);
                            } else {
                                Expr window_name = Expr(make_window_name(edge.producer, stencil.consumer));
                                Expr buffer_name = Expr(make_linebuffer_name(edge.producer, stencil.consumer));
                                Expr value_holder = Call::make(stencil.type, Call::sds_tmp_access,
                                                               {Expr(edge.pure_name())}, Call::Intrinsic);
                                //w.shift_pixels_left(); Then the right most column will have open space for new pixels
                                Stmt window_shift = Evaluate::make(Call::make(Type(), Call::sds_windowbuffer_update,
                                                                              {window_name, Expr("shift_pixels_left")},
                                                                              Call::Intrinsic));
                                //for (int i = 0; i < stencil_height - 1; ++i) w(i, stencil_width - 1) = l(i, current_column);
                                //Load the first (stencil_height - 1) rows of values in the linebuffer to the windowbuffer.
                                Stmt window_loader = stencil.stencil_height() - 1 != 0 ? 
                                    For::make(
                                        //If there is more than one buffer inside this stage, we need to specify the name
                                        buffer_name.as<StringImm>()->value + ".update", 0,
                                        stencil.stencil_height() - 1,
                                        ForType::Serial,
                                        DeviceAPI::Host,
                                        Evaluate::make(Call::make(
                                                stencil.type,
                                                Call::sds_windowbuffer_access,
                                                {
                                                        window_name,
                                                        Var(buffer_name.as<StringImm>()->value + ".update"),
                                                        stencil.stencil_width() - 1,
                                                        Call::make(
                                                                stencil.type,
                                                                Call::sds_linebuffer_access,
                                                                {
                                                                        buffer_name,
                                                                        Var(buffer_name.as<StringImm>()->value + ".update"),
                                                                        Var(loop->name) + stencil.stencil_width() - 1
                                                                },
                                                                Call::Intrinsic
                                                        )
                                                },
                                                Call::Intrinsic
                                        ))
                                ) : Evaluate::make(0);
                                //w(stencil_height - 1, stencil_width - 1) = holder
                                Stmt window_last_pix = Evaluate::make(Call::make(
                                        stencil.type,
                                        Call::sds_windowbuffer_access,
                                        {
                                                window_name,
                                                stencil.stencil_height() - 1,
                                                stencil.stencil_width() - 1,
                                                value_holder
                                        },
                                        Call::CallType::Intrinsic
                                ));
                                //l.shift_pixels_up(current_column);
                                //l.insert_buttom_row(current_column, holder);
                                Stmt buffer_shift = stencil.stencil_height() - 1 != 0 ? Evaluate::make(Call::make(
                                        stencil.type,
                                        Call::sds_linebuffer_update,
                                        {
                                                buffer_name,
                                                Var(loop->name) + stencil.stencil_width() - 1,
                                                value_holder
                                        },
                                        Call::CallType::Intrinsic
                                )) : Evaluate::make(0);
                                body = Block::make(IfThenElse::make(load_condition, Block::make(
                                        {stream_holder, window_shift, window_loader, window_last_pix, buffer_shift})),
                                                   body);
                            }
                        }
                    }
                } else {
                    body = mutate(loop->body);
                }

                // Find the loop_min, which is the top-left of the largest unvectorized stencil
                // We want to start the loop over the input using the largest stencil so it will be initialized
                // Smaller stencils will be initialized based on a condition in the same loop
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
                // Emit allocation of linebuffer and window in the outermost loop
                if (for_stack.empty()) {
                    vector <Stmt> buffer_allocators;
                    for (const HWStageEdge edge : edges) {
                        const Stencil &stencil = edge.stencil;
                        if (!stencil.is_fully_partitioned() && !stencil.is_single_pixel()) {
                            vector <Expr> args;
                            if (stencil.stencil_height() - 1 != 0) {
                                buffer_allocators.push_back(Evaluate::make(
                                        Call::make(
                                                stencil.type.with_lanes(stencil.stencil_wrapped()),
                                                Call::sds_linebuffer_alloc,
                                                {
                                                        Expr(make_linebuffer_name(edge.producer, stencil.consumer)),
                                                        Expr(stencil.stencil_height() - 1),
                                                        Expr(stencil.sub_img_width())
                                                },
                                                Call::Intrinsic
                                        )
                                ));
                            }
                            buffer_allocators.push_back(Evaluate::make(
                                    Call::make(
                                            stencil.type.with_lanes(stencil.stencil_wrapped()),
                                            Call::sds_windowbuffer_alloc,
                                            {
                                                    Expr(make_window_name(edge.producer, stencil.consumer)),
                                                    Expr(stencil.stencil_height()),
                                                    Expr(stencil.stencil_width())
                                            },
                                            Call::Intrinsic
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
        Type output_type;
        bool is_last_stage;

        InjectStreamConsumer(const vector <HWStageEdge> &edges, Type output_type, bool is_last_stage)
            : edges(edges), 
              scan_level(edges.front().stencil.scan_level),
              consumer(edges.front().stencil.consumer),
              output_type(output_type),
              is_last_stage(is_last_stage)
        {
        }
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
            if (call->is_intrinsic(Call::sds_tmp_access)) {
                string holder_name = call->args[0].as<StringImm>()->value;
                if (holder_type.find(holder_name) == holder_type.end()) {
                    holder_type[holder_name] = call->type;
                } else {
                    internal_assert(holder_type[holder_name] == call->type)
                            << "Holder different type detected! Abort!\n"
                            << call->args[0].as<StringImm>()->value << ": "
                            << call->type << " != " << holder_type[holder_name] << "\n";
                }
            } else {
                IRVisitor::visit(call);
            }
        }

        map <string, Type> holder_type;
    };

    /* Replace input/output parameters read/write by memory access */
    struct HolderReplacer : IRMutator {
        using IRMutator::visit;

        void visit(const Call *call) {
            if (call->is_intrinsic(Call::sds_stream_write)) {
                const string &s = call->args[0].as<StringImm>()->value;
                if (s == params.back().name) {
                    internal_assert(is_write_back);
                    expr = Call::make(params.back().type, Call::sds_stream_write,
                                      {Expr(s), write_back_index, call->args[1]}, Call::Intrinsic);
                } else {
                    IRMutator::visit(call);
                }
                /*} else if (call->is_intrinsic(Call::sds_stream_read)) {
                    const string &s = call->args[0].as<StringImm>()->value;
                    for (size_t i = 0; i < params.size() - 1; ++i) {
                        if (params[i].name == s) {
                            expr = call;
                            return ;
                        }
                    }
                    IRMutator::visit(call);
                } */
            }else {
                IRMutator::visit(call);
            }
        }

        const vector <HWParam> &params;
        Expr write_back_index;
        bool is_write_back;

        HolderReplacer(const vector <HWParam> &params, const vector <string> &traverse, const map<string, int> &extents)
                : params(params) {
            int stride = 1;
            is_write_back = true;
            for (size_t i = 0; i < traverse.size(); ++i) {
                write_back_index = !write_back_index.defined() ? Var(traverse[i]) * stride :
                                   write_back_index + Var(traverse[i]) * stride;
                if (extents.find(traverse[i]) == extents.end()) {
                    is_write_back = false;
                } else {
                    stride *= extents.find(traverse[i])->second;
                }
            }
        }
    };

    /* Find all the register accesses and put a bunch of register allocations in front of them. */
    struct OffloadLower : public IRMutator {
        using IRMutator::visit;

        void visit(const For *loop) {
            for (auto i : output_traverse) {
                if (i == loop->name) {
                    internal_assert(is_const(simplify(loop->min))) << loop->min << "\n";
                    internal_assert(is_const(simplify(loop->extent))) << loop->extent << "\n";
                    extents[loop->name] = (int) *as_const_int(simplify(loop->min + loop->extent));
                }
            }
            if (loop->for_type == ForType::SDSPipeline) {
                debug(3) << "Lowering " << loop->name << "...\n";
                HolderFinder finder;
                loop->body.accept(&finder);
                Stmt body = HolderReplacer(params, output_traverse, extents).mutate(loop->body);
                //If the final stage is also the only stage of the offloaded computation, we do not need to redefine the
                //tmp holder. Thus, just do not allocate it here!
                finder.holder_type.erase(params.back().name);
                for (const pair <string, Type> holder : finder.holder_type) {
                    body = Block::make(Evaluate::make(
                            Call::make(holder.second, Call::sds_tmp_alloc, {holder.first}, Call::Intrinsic)), body);
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
        map<string, int> extents;

        OffloadLower(const vector <HWParam> &params, const vector <string> &traverse_loop) : params(params),
                                                                                             output_traverse(
                                                                                                     traverse_loop) {
        }
    };

    // Search Call nodes for a call with the same name
    // Then duplicate the call except with different arguments
    // The new call is stored in 'res'
    // TODO: This is extremely hacky. This is used in generating glue logic to duplicate
    // a load to the input image
    struct MakeTheSameCall : public IRVisitor {
        using IRVisitor::visit;
        void visit(const Call *call) {
            if (!res.defined() && call->name == name) {
                res = Call::make(call->type, call->name, args, call->call_type,
                                 call->func, call->value_index, call->image, call->param);
            } else {
                IRVisitor::visit(call);
            }
        }

        const string &name;
        const vector <Expr> &args;
        Expr res;

        MakeTheSameCall(const string &name, const vector <Expr> &args) : name(name), args(args) {}
    };

    struct GetOutputVectorization : public IRVisitor {
        using IRVisitor::visit;

        void visit(const For *loop) {
            for_stack.push_back(loop);
            IRVisitor::visit(loop);
            for_stack.pop_back();
        }

        void visit(const Provide *provide) {
            if (provide->name == func) {
                internal_assert(is_vectorized.empty());
                has_vectorization = false;
                for (auto i : provide->args) {
                    bool res = true;
                    for (auto j : for_stack) {
                        if (expr_uses_var(i, j->name) && j->for_type == ForType::Serial) {
                            res = false;
                            break;
                        }
                    }
                    is_vectorized.push_back(res);
                    has_vectorization |= res;
                }
            }
            IRVisitor::visit(provide);
        }

        const string &func;
        vector<const For *> for_stack;
        vector<bool> is_vectorized;
        bool has_vectorization;
        GetOutputVectorization(const string &s) : func(s) {}
    };

    struct OffloadAnnotator : public IRMutator {
        using IRMutator::visit;

        // Keep track of lets to perform constant propagation
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

        // ---------------------------------------------------------------------
        // Main offload analysis pass
        // ---------------------------------------------------------------------
        void visit(const For *op) {
            bool is_offload = offload_level.match(op->name);

            // Get the bounds of this For node
            // The bounds are min and min+extent-1
            Interval interval(simplify(mutate(expand_expr(op->min, lets)), false, bounds),
                              simplify(expand_expr(mutate(op->min + op->extent - 1), lets), false, bounds));
            bounds.push(op->name, interval);

            if (is_offload) {
                Stmt new_body;
                Stmt unpruned = op->body;

                // This mutator gets rid of zero-extent loops and asserts that the loops are constant bound
                new_body = OffloadPruner(lets, bounds).mutate(op->body);

                // Output bounds inference
                Box output_box = box_provided(new_body, offload_level.func());
                vector<int> output_extent;
                debug(3) << "The output of offloaded body is: ";
                Type output_type = type_of_call_or_provide(new_body, offload_level.func(), true);
                for (size_t i = 0; i < output_box.size(); ++i) {
                    Expr extent = simplify(expand_expr(output_box[i].max - output_box[i].min + 1, lets), false, bounds);
                    internal_assert(is_const(extent));
                    debug(3) << "[" << output_box[i].min << ", " << extent << "]";
                    output_extent.push_back((int) *as_const_int(extent));
                }
                debug(3) << "\n";

                // Find inputs to the hardware function and do bounds inference
                // 'images' maps function stage names and input parameter names (e.g. 'f.s0.')
                // to their bounding box
                map <string, Box> images = offload_inputs(new_body, offload_level.func());
                map <string, Box> stage_bounds;

                vector <HWParam> hw_param;                            // arguments to the hardware function
                map <string, vector<HWStageEdge>> consume_graph;      // cosumer -> producer graph
                map <string, vector<string>> traverse_collection;     // non-unrolled loops of each stage
                set <string> input_names;                             // set of input parameter names
                set <string> hsh;                                     // set of elements which have been enqueued
                for (const pair <string, Box> &i : images) {
                    input_names.insert(i.first);
                    hsh.insert(i.first);
                }

                // -----------------------------------------------------------
                // Main analysis loop
                // For every producer, analyze the stencil of consumers
                // -----------------------------------------------------------
                map<string, vector<int>> producing_rate;
                vector <Stmt> data_duplicators;
                while (!images.empty()) {
                    // input is the current producer in the BFS search
                    pair <string, Box> input = *images.begin();
                    bool is_input_param = input_names.find(input.first) != input_names.end();

                    // This code tries to address #2 way of writing convolutions
                    //   conv(x,y) = 0;
                    //   conv(x,y) += ...;
                    // If conv is written in this way, stage0 initializes the buffer,
                    // and has no consumers. stage1 cosumes stage 0
                    debug(3) << "Try analyzing " << input.first << "\n";
                    if (!is_input_param) {
                        // Get stage0 by doing stage1 - 1
                        int stage = get_stage(input.first) - 1;
                        if (stage >= 0) {
                            // Construct string for stage0 and push it to images queue
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

                    // Pop images queue
                    images.erase(images.begin());

                    // 'expanded' is the bounding box with expr simplified bounds
                    Box expanded = input.second;
                    // Get the integer extents in each dimension and assert they are constant
                    vector<int> extents;
                    for (size_t i = 0; i < expanded.size(); ++i) {
                        expanded[i].min = simplify(expand_expr(expanded[i].min, lets), false, bounds);
                        expanded[i].max = simplify(expand_expr(expanded[i].max, lets), false, bounds);
                        Expr extent = simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds);
                        internal_assert(is_const(extent))
                                << "All stage or input inside offloaded body supposed to have fixed bound!\n"
                                << (simplify(expand_expr(expanded[i].max - expanded[i].min + 1, lets), false, bounds))
                                << "\n";
                        extents.push_back(*as_const_int(extent));
                    }
                    stage_bounds[input.first] = expanded;
                    // analyze_stencil returns a list of Stencils which are producer-consumer pairs
                    // that have stencil computation pattern
                    // 'stencil_list' is a list of the consumers of the current producer
                    vector <Stencil> stencil_list = analyze_stencil(
                            new_body,
                            input.first,
                            offload_level.func(),
                            env,                      // env here is a list of offloaded stages
                            is_input_param,
                            traverse_collection,
                            producing_rate
                    );

                    Type type = type_of_call_or_provide(new_body, input.first, is_input_param);
                    bool partitioned = false;
                    // push all the stencils onto the image queue
                    for (Stencil &stencil : stencil_list) {
                        stencil.type = type;
                        if (hsh.find(stencil.consumer) == hsh.end()) {
                            hsh.insert(input.first);
                            images[stencil.consumer] = box_of_stage(new_body, stencil.consumer);
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
                        if (stencil.is_fully_partitioned() && !partitioned) {
                            //TODO: later `maybe' intermediate partitioning will be supported
                            internal_assert(is_input_param) << "Right now only input partitioning supported!\n";
                            partitioned = true;
                        }
                    }

                    // ---------------------------------------------------------
                    // process input argument to the HW function
                    // ---------------------------------------------------------
                    if (is_input_param) {
                        /* The most proper producing rate of a parameter could be inferred by the compiler, but before
                         * inferring we need to determine if it is tuneable first. `Tunealbe' means it is consumed by all
                         * the consumers in the same consuming rate and access pattern.
                         * */
                        Stmt dd_stmt;
                        if (partitioned) {
                            int image_stride = 1;
                            Expr image_index = 0;
                            for (size_t i = 0; i < extents.size(); ++i) {
                                image_index += Var(make_iter_of_distributor(input.first, i)) * image_stride;
                                image_stride *= extents[i];
                            }
                            // Create stream reads from the input parameter to an internal buffer
                            dd_stmt = Evaluate::make(
                                    Call::make(type, Call::sds_stream_write,
                                               {"buffered$$" + input.first, image_index,
                                                Call::make(type, Call::sds_stream_read,
                                                           {Expr(input.first), image_index},
                                                           Call::CallType::Intrinsic)},
                                               Call::CallType::Intrinsic
                                    )
                            );
                            // Create loop nests to enclose the stream reads, loop nests loop over the input param
                            for (size_t i = 0; i < input.second.size(); ++i) {
                                dd_stmt = For::make(make_iter_of_distributor(input.first, i), Expr(0), extents[i],
                                                    i == 0 ? ForType::SDSPipeline : ForType::Serial, DeviceAPI::Host,
                                                    dd_stmt);
                            }
                            debug(3) << input.first << " is being partitioned!\n";
                            //internal_assert(sub_input.size() == 2) << "Right now only 2d is supported...\n";
                            producing_rate[input.first] = vector<int>(input.second.size(), 1);
                            hw_param.push_back(HWParam(type, input.first, extents));

                            //distributor maker
                            vector <Expr> expr_extents;
                            for (size_t i = 0; i < extents.size(); ++i) {
                                expr_extents.push_back(extents[i]);
                            }
                            // allocate internal buffer
                            new_body = Block::make(dd_stmt, new_body);
                            new_body = Allocate::make("buffered$$" + input.first, type, expr_extents, const_true(),
                                                      new_body);
                            debug(3) << input.first << " partitioned!\n";
                        } else {
                            // merging between a single producer sending to multiple consumers that
                            // have the same traversal pattern
                            if (can_be_merged(stencil_list)) {
                                const Stencil &stencil = stencil_list[0];
                                type = pad_lanes(stencil.stencil_wrapped(), type);
                                producing_rate[input.first] = stencil.stencil_bounds;
                            } else {
                                producing_rate[input.first] = vector<int>(input.second.size(), 1);
                            }
                            // -------------------------------------------------
                            // Create distributor
                            // A distributor sends pixels from an input argument to all consumers
                            // it accesses the argument sequentially,
                            // then for each consumer, it checks if the current pixel is in its
                            // required region, if so send the pixel
                            // -------------------------------------------------
                            vector<Stmt> distributors;
                            for (auto &stencil : stencil_list) {
                                stencil.type = type;
                                distributors.push_back(make_distributor(type, input.first, stencil));
                            }
                            dd_stmt = Block::make(distributors);
                            const Stencil &stencil = *stencil_list.begin();
                            bool innermost = true;
                            int image_stride = 1;
                            Expr image_index = 0;
                            for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                                if (!stencil.is_vectorized_dim(i)) {
                                    image_index += Var(make_iter_of_distributor(input.first, i)) * image_stride;
                                    image_stride *= extents[i];
                                }
                            }
                            // Call::sds_tmp_access is a read/write to a temporary variable
                            // Call::sts_tmp_alloc allocates a writable temporary variable
                            dd_stmt = Block::make(
                                    Evaluate::make(
                                            Call::make(type, Call::sds_tmp_access,
                                                       {input.first, Call::make(type, Call::sds_stream_read, {Expr(input.first), image_index}, Call::CallType::Intrinsic)},
                                                       Call::CallType::Intrinsic
                                            )
                                    ),
                                    dd_stmt
                            );
                            for (size_t i = 0; i < stencil.image_mins.size(); ++i) {
                                if (!stencil.is_vectorized_dim(i)) {
                                    dd_stmt = For::make(make_iter_of_distributor(input.first, i), 0, extents[i],
                                                        innermost ? ForType::SDSPipeline : ForType::Serial,
                                                        DeviceAPI::Host, dd_stmt);
                                    innermost = false;
                                }
                            }
                            vector <Stmt> stream_allocs;
                            for (const Stencil &stencil : stencil_list) {
                                Expr stream_alloc =
                                        Call::make(type, Call::sds_stream_alloc,
                                                   {Expr(make_stream_name(input.first, stencil.consumer)),
                                                         get_stream_depth(input.first, stencil.consumer, env)},
                                                   Call::CallType::Intrinsic);
                                stream_allocs.push_back(Evaluate::make(stream_alloc));
                            }
                            stream_allocs.push_back(dd_stmt);
                            dd_stmt = Block::make(stream_allocs);
                            vector<int> vectorized_extents;
                            for (size_t i = 0; i < extents.size(); ++i) {
                                if (!stencil.is_vectorized_dim(i)) {
                                    vectorized_extents.push_back(extents[i]);
                                } else {
                                    vectorized_extents.push_back(1);
                                }
                            }
                            hw_param.push_back(HWParam(type, input.first, vectorized_extents));
                            new_body = Block::make(dd_stmt, new_body);
                        }
                        //debug(3) << "Distributor:\n" << dd_stmt << "\n";
                        debug(3) << input.first << " as a parameter added...\n";

                        // -----------------------------------------------------
                        // Glue logic to duplicate the data on the software side
                        // The structure of the glue logic is below, for an input image
                        // named 'input'
                        //   for (dup.input.y = ...) {
                        //     for (dup.input.x = ...) {
                        //       tmp.range(vectorized_index) = input[input_idxs];
                        //       tmp.range(vectorized_index) = input[input_idxs];
                        //       tmp.range(vectorized_index) = input[input_idxs];
                        //       dup__input[array_index] = tmp;
                        //     }
                        //   }
                        // Currently we only tried this for a single vectorized dim
                        // which is the innermost dim
                        // -----------------------------------------------------
                        {
                            const Stencil &stencil = *stencil_list.begin();
                            // box is the subregion of arr which we are going to send to HW
                            Box box = box_required(unpruned, input.first);
                            vector <Expr> input_idxs;
                            // populate input_idxs and add the top-left corner of the input subregion index
                            for (size_t j = 0; j < box.size(); ++j) {
                                input_idxs.push_back(Var("dup." + input.first + "." + std::to_string(j)) + box[j].min);
                            }
                            // Finds any call (load) to input and duplicate it with new input_idxs
                            // TODO: replace this with creating a new call
                            MakeTheSameCall maker(input.first, input_idxs);
                            unpruned.accept(&maker);
                            internal_assert(maker.res.defined());

                            // vectorized_index is used in tmp.range(*) = ...
                            // array_index is used in dup__input[*] = tmp
                            int vectorized_stride = 1, array_stride = 1;
                            Expr vectorized_index = 0, array_index = 0;
                            bool has_vectorized_dim = false;
                            for (size_t i = 0; i < stencil.stencil_mins.size(); ++i) {
                                if (stencil.is_vectorized_dim(i)) {
                                    internal_assert(extents[i] == stencil.stencil_bounds[i]);
                                    vectorized_index +=
                                            Var("dup." + input.first + "." + std::to_string(i)) * vectorized_stride;
                                    // stencil_bounds is the size of the subregion of the input
                                    vectorized_stride *= stencil.stencil_bounds[i];
                                    has_vectorized_dim = true;
                                } else {
                                    array_index +=
                                            Var("dup." + input.first + "." + std::to_string(i)) * array_stride;
                                    array_stride *= extents[i];
                                }
                            }

                            // Generate the store to tmp, called 'duplicator'
                            // This will be wrapped in an unrolled loop over c, the innermost dimension
                            Stmt duplicator;
                            if (has_vectorized_dim) {
                                // sds_bit_range is the .range() call
                                duplicator = Evaluate::make(Call::make(type.with_lanes(1), Call::sds_bit_range,
                                                                       {Call::make(type, Call::sds_tmp_access,
                                                                                   {"dup$$" + input.first},
                                                                                   Call::CallType::Intrinsic),
                                                                        vectorized_index, maker.res},
                                                                       Call::CallType::Intrinsic));
                            } else {
                                duplicator = Evaluate::make(Call::make(type, Call::sds_tmp_access,
                                                                       {"dup$$" + input.first, maker.res},
                                                                       Call::CallType::Intrinsic));
                            }

                            // Wrap the duplicator in a loop nest
                            // The loop nest is always REORDERED (y, x, c) s.t. the vectorized dim c is the
                            // innermost loop. This reordering occurs regardless of user scheduling
                            //
                            // This code wraps the innermost (vectorized) loop
                            for (size_t i = 0; i < stencil.stencil_mins.size(); ++i) {
                                if (stencil.is_vectorized_dim(i)) {
                                    duplicator = For::make(
                                            "dup." + input.first + "." + std::to_string(i),
                                            0,
                                            stencil.stencil_bounds[i],
                                            ForType::Unrolled,
                                            DeviceAPI::Host,
                                            duplicator
                                    );
                                }
                            }

                            duplicator = Block::make({
                              // Inject allocation of tmp right before the innermomst (vectorized) loop
                                                      Evaluate::make(Call::make(type, Call::sds_tmp_alloc,
                                                                                {Expr("dup$$" + input.first)},
                                                                                 Call::CallType::Intrinsic)),
                                                      duplicator,
                              // Inject write of tmp to the dup__input array after the innermost (vectorized) loop
                                                      Evaluate::make(Call::make(type, Call::sds_stream_write,
                                                                                {Expr("dup$$" + input.first),
                                                                                 array_index,
                                                                                 Call::make(type,
                                                                                           Call::sds_tmp_access,
                                                                                           {"dup$$" + input.first},
                                                                                           Call::Intrinsic)
                                                                                 },
                                                                                Call::CallType::Intrinsic))
                                                     }
                            );

                            // Wrap the outer (y,x) loops
                            vector<Expr> expr_extents;
                            for (size_t i = 0; i < stencil.stencil_mins.size(); ++i) {
                                if (!stencil.is_vectorized_dim(i)) {
                                    duplicator = For::make(
                                            "dup." + input.first + "." + std::to_string(i),
                                            0,
                                            extents[i],
                                            ForType::Serial,
                                            DeviceAPI::Host,
                                            duplicator
                                    );
                                }
                                expr_extents.push_back(extents[i] / (stencil.is_vectorized_dim(i) ? stencil.stencil_bounds[i] : 1));
                            }
                            debug(3) << "Input duplicator:\n" << duplicator << "\n";
                            duplicator = Allocate::make("dup$$" + input.first, type, expr_extents, const_true(), duplicator);

                            // Push the data duplicator to a list, there's a duplicator for each HW input param
                            data_duplicators.push_back(duplicator);
                        }
                    } else {
                        // This code injects streams between different stages in the HW block
                        debug(3) << input.first << " stencil analysis done...\n";
                        if (!partitioned) {
                            internal_assert(traverse_collection.find(input.first) != traverse_collection.end())
                                    << "The producer " << input.first << " should have been analyzed...\n";
                            internal_assert(producing_rate.find(input.first) != producing_rate.end())
                                    << input.first << "'s producing rate note inferred!\n";
                            const vector<int> &rate = producing_rate[input.first];
                            int vectorized_lanes = 1;
                            debug(3) << input.first << "\n";
                            for (auto i : rate) {
                                vectorized_lanes *= i;
                                debug(3) << "[" << i << "]";
                            }
                            debug(3) << "\n";
                            type = type.with_lanes(vectorized_lanes);
                            for (Stencil &stencil : stencil_list) {
                                internal_assert(rate.size() == stencil.stencil_bounds.size());
                                for (size_t i = 0; i < rate.size(); ++i) {
                                    internal_assert(stencil.stencil_bounds[i] % rate[i] == 0);
                                }
                                stencil.type = type;
                                // Inject allocation and writes to the stream
                                StreamAllocInjector sai(type, input.first, stencil, traverse_collection[input.first], env);
                                new_body = sai.mutate(new_body);
                            }
                            debug(3) << "Stencil streams inject!!\n";
                        } else {
                            // We don't support partitioning of intermediate result arrays (due to laziness)
                            // We support partitioning of the input parameters after copying them to a
                            // partitioned array
                            internal_assert(false) << "Partitioning of intermediate result arrays is not supported!\n";
                        }
                    }
                    // Gather all the producers at the consumer side.
                    // 'consume_graph' gives the producers for a consumer
                    for (Stencil &stencil : stencil_list) {
                        consume_graph[stencil.consumer].push_back(HWStageEdge(input.first, extents, stencil));
                    }
                }

                // To support pure producers. This feature is not well tested
                for (const string &stage : hsh) {
                    // Check if a stage consumes nothing and is not an input
                    if (consume_graph.find(stage) == consume_graph.end() &&
                        input_names.find(stage) == input_names.end()) {
                        new_body = PureProducer(stage, traverse_collection[stage].front()).mutate(new_body);
                    }
                }

                
                for (const pair <string, vector<HWStageEdge>> node : consume_graph) {
                    internal_assert(!node.second.empty());
                    // HWStageEdge associates a stencil with its producer
                    // The code below emits debug info about each stencil and its producer
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
                        for (const int &stride : stencil.stencil_stride) {
                            debug(3) << "[" << stride << "]";
                        }
                        debug(3) << "\nName of traverse loop(s):\n";
                        for (const string &loop_name : stencil.traverse) {
                            debug(3) << loop_name << "\n";
                        }
                        debug(3) << "\n";

                    }
                    {
                        // rate[i] is R if dim i is vectorized with extent R, otherwise it is 1
                        const vector<int> &rate = producing_rate[offload_level.func() + ".s0."];
                        int lanes = 1;
                        for (size_t i = 0; i < output_extent.size(); ++i) {
                            internal_assert(output_extent[i] % rate[i] == 0);
                            lanes *= rate[i];
                        }
                        new_body = InjectStreamConsumer(node.second, pad_lanes(lanes, output_type),
                                                        offload_level.func() == strip_stage(node.first)).mutate(new_body);
                    }
                }
                {
                    internal_assert(producing_rate.find(offload_level.func() + ".s0.") != producing_rate.end());
                    vector<int> output_vectorized_extent;
                    const vector<int> &rate = producing_rate[offload_level.func() + ".s0."];
                    internal_assert(output_extent.size() == rate.size());
                    int lanes = 1;
                    debug(3) << "Output producing rate: ";
                    for (size_t i = 0; i < output_extent.size(); ++i) {
                        internal_assert(output_extent[i] % rate[i] == 0);
                        output_vectorized_extent.push_back(output_extent[i] / rate[i]);
                        lanes *= rate[i];
                        debug(3) << "[" << rate[i] << "]";
                    }
                    debug(3) << "\n";
                    output_type = pad_lanes(lanes, output_type);
                    hw_param.push_back(HWParam(output_type, offload_level.func(), output_vectorized_extent));
                    internal_assert(
                            traverse_collection.find(offload_level.func() + ".s0.") != traverse_collection.end())
                            << "Traverse loop of out put not found?!\n";
                }
                new_body = OffloadLower(hw_param, traverse_collection[offload_level.func() + ".s0."]).mutate(new_body);
                new_body = Offload::make(offload_level.func(), hw_param, new_body);

                Stmt data_write_back;
                {
                    Box box = box_provided(unpruned, offload_level.func());
                    vector<Expr> extents;
                    const vector<int> &sizes(output_extent);
                    const vector<int> &rate(producing_rate[offload_level.func() + ".s0."]);
                    /*for (size_t i = 0; i < output_extent.size(); ++i) {
                        extents.push_back(output_extent[i]);
                    }*/
                    //Buffer<> out_buffer(output_type, sizes, "dup$$" + offload_level.func());
                    Expr array_index = 0, vectorized_index = 0;
                    int array_stride = 1, vectorized_stride= 1;
                    GetOutputVectorization checker(offload_level.func());
                    unpruned.accept(&checker);
                    debug(3) << checker.is_vectorized.size() << " bits:";
                    for (size_t i = 0; i < box.size(); ++i) {
                        internal_assert(sizes[i] % rate[i] == 0);
                        if (checker.is_vectorized[i]) {
                            vectorized_index += vectorized_stride * Var("dup$$" + offload_func.name() + "." + std::to_string(i));
                            vectorized_stride*= sizes[i];
                            extents.push_back(sizes[i] / rate[i]);
                        } else {
                            array_index += array_stride * Var("dup$$" + offload_func.name() + "." + std::to_string(i));
                            array_stride*= sizes[i];
                            extents.push_back(sizes[i]);
                        }
                        debug(3) << checker.is_vectorized[i];
                    }
                    debug(3) << "\n";

                    vector<Expr> provide_args/*, call_args*/;
                    for (size_t i = 0; i < box.size(); ++i) {
                        provide_args.push_back(Var("dup$$" + offload_func.name() + "." + std::to_string(i)) + box[i].min);
                        //call_args.push_back(Var("dup$$" + offload_func.name() + "." + std::to_string(i)));
                    }
                    Expr value = Call::make(output_type, Call::sds_stream_read, {Expr("dup$$" + offload_level.func()), array_index}, Call::CallType::Intrinsic);
                    if (checker.has_vectorization) {
                        value = Call::make(output_type.with_lanes(1), Call::sds_bit_range, {value, vectorized_index},
                                           Call::CallType::Intrinsic);
                        //debug(3) << "Vectorized output:\n" << value << "\n";
                    }
                    Stmt write_back = Provide::make(offload_func.name(), {value}, provide_args);
                    for (size_t i = 0; i < box.size(); ++i) {
                        write_back = For::make("dup$$" + offload_func.name() + "." + std::to_string(i), 0, sizes[i], checker.is_vectorized[i] ? ForType::Unrolled : ForType::Serial, DeviceAPI::Host, write_back);
                    }
                    int stride = 1;
                    for (size_t j = 0; j < box.size(); ++j) {
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".min." + std::to_string(j), 0,
                                                   write_back);
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".extent." + std::to_string(j),
                                                   sizes[j], write_back);
                        write_back = LetStmt::make("dup$$" + offload_func.name() + ".stride." + std::to_string(j),
                                                   stride,
                                                   write_back);
                        stride *= sizes[j];
                    }
                    debug(3) << "Write it back: "
                             << write_back << "\n";
                    data_write_back = Allocate::make("dup$$" + offload_func.name(), output_type, extents, const_true(),
                                                     write_back);
                }

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
                    debug(3) << "Duplicate " << allocate->name << "\n";
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
        // f.offload({g,h},xo);
        // offload_stages contains g,h (may be empty)
        // offload_level is the variable "f.*.xo"

        for (const pair <string, Function> &function : env) {
            const vector <string> &offloads(function.second.schedule().offloaded_stages());
            if (function.second.schedule().offload_level().func() != "") {
                /*Extract the stages offloaded to FPGA logic. Only purely defined functions allowed to be blocks. */
                map<string, Function> sub_env;
                for (auto i : offloads) {
                    internal_assert(env.find(i)->second.updates().size() == 0)
                            << "Only purely defined functions allowed to be blocks!\n";
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
