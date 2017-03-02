#include <iostream>
#include <limits>

#include "CodeGen_SDS.h"
#include "CodeGen_Internal.h"
#include "Substitute.h"
#include "IROperator.h"
#include "Param.h"
#include "Var.h"
#include "Lerp.h"
#include "Simplify.h"

namespace Halide {
namespace Internal {

using std::ostream;
using std::endl;
using std::string;
using std::vector;
using std::ostringstream;
using std::map;

namespace {
const string buffer_t_definition =
    "#ifndef HALIDE_ATTRIBUTE_ALIGN\n"
    "  #ifdef _MSC_VER\n"
    "    #define HALIDE_ATTRIBUTE_ALIGN(x) __declspec(align(x))\n"
    "  #else\n"
    "    #define HALIDE_ATTRIBUTE_ALIGN(x) __attribute__((aligned(x)))\n"
    "  #endif\n"
    "#endif\n"
    "#ifndef BUFFER_T_DEFINED\n"
    "#define BUFFER_T_DEFINED\n"
    "#include <stdbool.h>\n"
    "#include <stdint.h>\n"
    "typedef struct buffer_t {\n"
    "    uint64_t dev;\n"
    "    uint8_t* host;\n"
    "    int32_t extent[4];\n"
    "    int32_t stride[4];\n"
    "    int32_t min[4];\n"
    "    int32_t elem_size;\n"
    "    HALIDE_ATTRIBUTE_ALIGN(1) bool host_dirty;\n"
    "    HALIDE_ATTRIBUTE_ALIGN(1) bool dev_dirty;\n"
    "    HALIDE_ATTRIBUTE_ALIGN(1) uint8_t _padding[10 - sizeof(void *)];\n"
    "} buffer_t;\n"
    "#endif\n";

const string top_headers =
    "#include <iostream>\n"
    "#include <math.h>\n"
    "#include <float.h>\n"
    "#include <assert.h>\n"
    "#include <string.h>\n"
    "#include <stdio.h>\n"
    "#include <stdint.h>\n";

const string hardware_headers =
    "#include \"hls_video.h\"\n"
    "#include \"hls_stream.h\"\n"
    "#include <iostream>\n"
    "#include <math.h>\n"
    "#include <float.h>\n"
    "#include <assert.h>\n"
    "#include <string.h>\n"
    "#include <stdio.h>\n"
    "#include <stdint.h>\n";

const string extern_globals =
    "extern \"C\" {\n"
    "void *halide_malloc(void *ctx, size_t);\n"
    "void halide_free(void *ctx, void *ptr);\n"
    "void *halide_print(void *ctx, const void *str);\n"
    "void *halide_error(void *ctx, const void *str);\n"
    "int halide_debug_to_file(void *ctx, const char *filename, int, struct buffer_t *buf);\n"
    "int halide_start_clock(void *ctx);\n"
    "int64_t halide_current_time_ns(void *ctx);\n"
    "void halide_profiler_pipeline_end(void *, void *);\n"
    "}\n"
    "\n";

const string internal_globals =
    "#ifdef _WIN32\n"
    "float roundf(float);\n"
    "double round(double);\n"
    "#else\n"
    "inline float asinh_f32(float x) {return asinhf(x);}\n"
    "inline float acosh_f32(float x) {return acoshf(x);}\n"
    "inline float atanh_f32(float x) {return atanhf(x);}\n"
    "inline double asinh_f64(double x) {return asinh(x);}\n"
    "inline double acosh_f64(double x) {return acosh(x);}\n"
    "inline double atanh_f64(double x) {return atanh(x);}\n"
    "#endif\n"
    "inline float sqrt_f32(float x) {return sqrtf(x);}\n"
    "inline float sin_f32(float x) {return sinf(x);}\n"
    "inline float asin_f32(float x) {return asinf(x);}\n"
    "inline float cos_f32(float x) {return cosf(x);}\n"
    "inline float acos_f32(float x) {return acosf(x);}\n"
    "inline float tan_f32(float x) {return tanf(x);}\n"
    "inline float atan_f32(float x) {return atanf(x);}\n"
    "inline float sinh_f32(float x) {return sinhf(x);}\n"
    "inline float cosh_f32(float x) {return coshf(x);}\n"
    "inline float tanh_f32(float x) {return tanhf(x);}\n"
    "inline float hypot_f32(float x, float y) {return hypotf(x, y);}\n"
    "inline float exp_f32(float x) {return expf(x);}\n"
    "inline float log_f32(float x) {return logf(x);}\n"
    "inline float pow_f32(float x, float y) {return powf(x, y);}\n"
    "inline float floor_f32(float x) {return floorf(x);}\n"
    "inline float ceil_f32(float x) {return ceilf(x);}\n"
    "inline float round_f32(float x) {return roundf(x);}\n"
    "\n"
    "inline double sqrt_f64(double x) {return sqrt(x);}\n"
    "inline double sin_f64(double x) {return sin(x);}\n"
    "inline double asin_f64(double x) {return asin(x);}\n"
    "inline double cos_f64(double x) {return cos(x);}\n"
    "inline double acos_f64(double x) {return acos(x);}\n"
    "inline double tan_f64(double x) {return tan(x);}\n"
    "inline double atan_f64(double x) {return atan(x);}\n"
    "inline double sinh_f64(double x) {return sinh(x);}\n"
    "inline double cosh_f64(double x) {return cosh(x);}\n"
    "inline double tanh_f64(double x) {return tanh(x);}\n"
    "inline double hypot_f64(double x, double y) {return hypot(x, y);}\n"
    "inline double exp_f64(double x) {return exp(x);}\n"
    "inline double log_f64(double x) {return log(x);}\n"
    "inline double pow_f64(double x, double y) {return pow(x, y);}\n"
    "inline double floor_f64(double x) {return floor(x);}\n"
    "inline double ceil_f64(double x) {return ceil(x);}\n"
    "inline double round_f64(double x) {return round(x);}\n"
    "\n"
    "inline float nan_f32() {return NAN;}\n"
    "inline float neg_inf_f32() {return -INFINITY;}\n"
    "inline float inf_f32() {return INFINITY;}\n"
    "inline bool is_nan_f32(float x) {return x != x;}\n"
    "inline bool is_nan_f64(double x) {return x != x;}\n"
    "template<typename A, typename B> A reinterpret(B b) {A a; memcpy(&a, &b, sizeof(a)); return a;}\n"
    "inline float float_from_bits(uint32_t bits) {return reinterpret<float, uint32_t>(bits);}\n"
    "\n"
    "template<typename T> T max(T a, T b) {if (a > b) return a; return b;}\n"
    "template<typename T> T min(T a, T b) {if (a < b) return a; return b;}\n"
    "\n"
    "inline uint8_t *_halide_buffer_get_host(const buffer_t *buf) {\n"
    " return buf->host;\n"
    "}\n"
    "inline int _halide_buffer_get_min(const buffer_t *buf, int d) {\n"
    " return buf->min[d];\n"
    "}\n"
    "inline int _halide_buffer_get_max(const buffer_t *buf, int d) {\n"
    " return buf->min[d] + buf->extent[d] - 1;\n"
    "}\n"
    "inline int _halide_buffer_set_dev_dirty(buffer_t *buf, bool val) {\n"
    " buf->dev_dirty = val;\n"
    " return 0;\n"
    "}\n"
    "inline int _halide_buffer_set_host_dirty(buffer_t *buf, bool val) {\n"
    " buf->host_dirty = val;\n"
    " return 0;\n"
    "}\n"
    "inline buffer_t *_halide_buffer_init(buffer_t *dst, void *host, uint64_t dev, int /*type_code*/, int type_bits, int dimensions, \n"
    "                                     const void *min, const void *extent, const void *stride, bool host_dirty, bool dev_dirty) {\n"
    " dst->host = (uint8_t *)host;\n"
    " dst->dev = dev;\n"
    " dst->elem_size = (type_bits + 7) / 8;\n"
    " dst->host_dirty = host_dirty;\n"
    " dst->dev_dirty = dev_dirty;\n"
    " for (int i = 0; i < dimensions; i++) {\n"
    "  dst->min[i] = ((const int *)min)[i];\n"
    "  dst->extent[i] = ((const int *)extent)[i];\n"
    "  dst->stride[i] = ((const int *)stride)[i];\n"
    " }\n"
    " for (int i = dimensions; i < 4; i++) {\n"
    "  dst->min[i] = 0;\n"
    "  dst->extent[i] = 0;\n"
    "  dst->stride[i] = 0;\n"
    " }\n"
    " return dst;\n"
    "}\n"
    "\n";
}

CodeGen_SDS::CodeGen_SDS(ostream &s, OutputKind output_kind, const std::string &guard) : IRPrinter(s), id("$$ BAD ID $$"), output_kind(output_kind), extern_c_open(false) {
    if (is_header()) {
        // If it's a header, emit an include guard.
        stream << "#ifndef HALIDE_" << print_name(guard) << '\n'
               << "#define HALIDE_" << print_name(guard) << '\n';
    }

    if (!is_header()) {
        stream << (is_hardware() ? hardware_headers : top_headers);
    }

    // Throw in a definition of a buffer_t
    stream << buffer_t_definition;

    // halide_filter_metadata_t just gets a forward declaration
    // (include HalideRuntime.h for the full goodness)
    stream << "struct halide_filter_metadata_t;\n";

    if (!is_header()) {
        if (!is_hardware()) {
            stream << extern_globals;
        }
        stream << internal_globals;
    }

    // Throw in a default (empty) definition of HALIDE_FUNCTION_ATTRS
    // (some hosts may define this to e.g. __attribute__((warn_unused_result)))
    stream << "#ifndef HALIDE_FUNCTION_ATTRS\n";
    stream << "#define HALIDE_FUNCTION_ATTRS\n";
    stream << "#endif\n";
}

CodeGen_SDS::~CodeGen_SDS() {
    switch_to_c_or_c_plus_plus(COrCPlusPlus::Default);
    if (is_header()) {
        stream << "#endif\n";
    }
}

namespace {
string type_to_c_type(Type type, bool include_space, bool c_plus_plus = true) {
    bool needs_space = true;
    ostringstream oss;
    user_assert(type.lanes() == 1) << "Can't use vector types when compiling to C (yet)\n";
    if (type.is_float()) {
        if (type.bits() == 32) {
            oss << "float";
        } else if (type.bits() == 64) {
            oss << "double";
        } else {
            user_error << "Can't represent a float with this many bits in C: " << type << "\n";
        }

    } else if (type.is_handle()) {
        needs_space = false;

        // If there is no type info or is generating C (not C++) and
        // the type is a class or in an inner scope, just use void *.
        if (type.handle_type == NULL ||
            (!c_plus_plus &&
             (!type.handle_type->namespaces.empty() ||
              !type.handle_type->enclosing_types.empty() ||
              type.handle_type->inner_name.cpp_type_type == halide_cplusplus_type_name::Class))) {
            oss << "void *";
        } else {
            if (type.handle_type->inner_name.cpp_type_type == halide_cplusplus_type_name::Struct) {
                oss << "struct ";
            } else if (type.handle_type->inner_name.cpp_type_type == halide_cplusplus_type_name::Class) {
                oss << "class ";
            }
            if (!type.handle_type->namespaces.empty() ||
                !type.handle_type->enclosing_types.empty()) {
                oss << "::";
                for (size_t i = 0; i < type.handle_type->namespaces.size(); i++) {
                    oss << type.handle_type->namespaces[i] << "::";
                }
                for (size_t i = 0; i < type.handle_type->enclosing_types.size(); i++) {
                    oss << type.handle_type->enclosing_types[i].name << "::";
                }
            }
            oss << type.handle_type->inner_name.name;
            if (type.handle_type->reference_type == halide_handle_cplusplus_type::LValueReference) {
                oss << " &";
            } else if (type.handle_type->reference_type == halide_handle_cplusplus_type::LValueReference) {
                oss << " &&";
            }
            for (auto modifier : type.handle_type->cpp_type_modifiers) {
                if (modifier & halide_handle_cplusplus_type::Const) {
                    oss << " const";
                }
                if (modifier & halide_handle_cplusplus_type::Volatile) {
                    oss << " volatile";
                }
                if (modifier & halide_handle_cplusplus_type::Restrict) {
                    oss << " restrict";
                }
                if (modifier & halide_handle_cplusplus_type::Pointer) {
                    oss << " *";
                }
            }
        }
    } else {
        switch (type.bits()) {
        case 1:
            oss << "bool";
            break;
        case 8: case 16: case 32: case 64:
            if (type.is_uint()) oss << 'u';
            oss << "int" << type.bits() << "_t";
            break;
        default:
            user_error << "Can't represent an integer with this many bits in C: " << type << "\n";
        }
    }
    if (include_space && needs_space)
        oss << " ";
    return oss.str();
}
}

void CodeGen_SDS::switch_to_c_or_c_plus_plus(COrCPlusPlus mode) {
    if (extern_c_open && mode != COrCPlusPlus::C) {
        stream << "\n#ifdef __cplusplus\n";
        stream << "}  // extern \"C\"\n";
        stream << "#endif\n";
        extern_c_open = false;
    } else if (!extern_c_open && mode == COrCPlusPlus::C) {
        stream << "#ifdef __cplusplus\n";
        stream << "extern \"C\" {\n";
        stream << "#endif\n";
        extern_c_open = true;
    }
}

string CodeGen_SDS::print_type(Type type, AppendSpaceIfNeeded space_option) {
    return type_to_c_type(type, space_option == AppendSpace);
}

string CodeGen_SDS::print_reinterpret(Type type, Expr e) {
    ostringstream oss;
    oss << "reinterpret<" << print_type(type) << ">(" << print_expr(e) << ")";
    return oss.str();
}

string CodeGen_SDS::print_name(const string &name) {
    ostringstream oss;

    // Prefix an underscore to avoid reserved words (e.g. a variable named "while")
    if (isalpha(name[0])) {
        oss << '_';
    }

    for (size_t i = 0; i < name.size(); i++) {
        if (name[i] == '.') {
            oss << '_';
        } else if (name[i] == '$') {
            oss << "__";
        } else if (name[i] != '_' && !isalnum(name[i])) {
            oss << "___";
        }
        else oss << name[i];
    }
    return oss.str();
}

namespace {
class ExternCallPrototypes : public IRGraphVisitor {
    struct NamespaceOrCall {
        const Call *call; // nullptr if this is a subnamespace
        std::map<string, NamespaceOrCall> names;
        NamespaceOrCall(const Call *call = nullptr) : call(call) { }
    };
    std::map<string, NamespaceOrCall> c_plus_plus_externs;
    std::map<string, const Call *> c_externs;
    std::set<std::string> &emitted;

    using IRGraphVisitor::visit;

    void visit(const Call *op) {
        IRGraphVisitor::visit(op);

        if (!emitted.count(op->name)) {
            if (op->call_type == Call::Extern) {
                c_externs.insert({op->name, op});
            } else if (op->call_type == Call::ExternCPlusPlus) {
                std::vector<std::string> namespaces;
                std::string name = extract_namespaces(op->name, namespaces);
                std::map<string, NamespaceOrCall> *namespace_map(&c_plus_plus_externs);
                for (const auto &ns : namespaces) {
                    auto insertion = namespace_map->insert({ns, NamespaceOrCall()});
                    namespace_map = &insertion.first->second.names;
                }
                namespace_map->insert({name, NamespaceOrCall(op)});
            }
            emitted.insert(op->name);
        }
    }

    void emit_function_decl(ostream &stream, const Call *op, const std::string &name) {
        //FIXME: Halide internal bug?!
        if (op->name == "_halide_buffer_init") {
            return ;
        }
        stream << type_to_c_type(op->type, true) << " " << name << "(";
        if (function_takes_user_context(name)) {
            stream << "void *";
            if (!op->args.empty()) {
                stream << ", ";
            }
        }
        for (size_t i = 0; i < op->args.size(); i++) {
            if (i > 0) {
                stream << ", ";
            }
            if (op->args[i].as<StringImm>()) {
                stream << "const char *";
            } else {
              stream << type_to_c_type(op->args[i].type(), true);
            }
        }
        stream << ");\n";
    }

    void emit_namespace_or_call(ostream &stream, const NamespaceOrCall &ns_or_call, const std::string &name) {
        if (ns_or_call.call == nullptr) {
            stream << "namespace " << name << " {\n";
            for (const auto &ns_or_call_inner : ns_or_call.names) {
                emit_namespace_or_call(stream, ns_or_call_inner.second, ns_or_call_inner.first);
            }
            stream << "} // namespace " << name << "\n";
        } else {
            emit_function_decl(stream, ns_or_call.call, name);
        }
    }

public:
  ExternCallPrototypes(std::set<string> &emitted) : emitted(emitted) {
        size_t j = 0;
        // Make sure we don't catch calls that are already in the global declarations
        for (size_t i = 0; i < extern_globals.size(); i++) {
            char c = extern_globals[i];
            if (c == '(' && i > j+1) {
                // Could be the end of a function_name.
                emitted.insert(extern_globals.substr(j+1, i-j-1));
            }

            if (('A' <= c && c <= 'Z') ||
                ('a' <= c && c <= 'z') ||
                c == '_' ||
                ('0' <= c && c <= '9')) {
                // Could be part of a function name.
            } else {
                j = i;
            }

        }
    }

    bool has_c_declarations() {
        return !c_externs.empty();
    }

    bool has_c_plus_plus_declarations() {
        return !c_plus_plus_externs.empty();
    }

    void emit_c_declarations(ostream &stream) {
        for (const auto &call : c_externs) {
            emit_function_decl(stream, call.second, call.first);
        }
        stream << "\n";
    }

    void emit_c_plus_plus_declarations(ostream &stream) {
        for (const auto &ns_or_call : c_plus_plus_externs) {
            emit_namespace_or_call(stream, ns_or_call.second, ns_or_call.first);
        }
        stream << "\n";
    }
};

    struct OffloadFinder : IRVisitor {
		using IRVisitor::visit;
        void visit(const Offload *offload) {
            res.insert(offload->name);
        }
    public:
        std::set<string> res;
    };


}

void CodeGen_SDS::compile(const Module &input) {
    OffloadFinder finder;
    for (const auto &f : input.functions()) {
        f.body.accept(&finder);
    }
    stream << "\n";
    stream << "//offloaded function modules...\n";
    for (const auto &s : finder.res) {
        stream << "#include \"" << s << ".h\"\n";
    }
    stream << "\n";
    for (const auto &b : input.buffers()) {
        compile(b);
    }
    for (const auto &f : input.functions()) {
        compile(f);
    }
}

void CodeGen_SDS::compile(const LoweredFunc &f) {
    // Don't put non-external function declarations in headers.
    if (is_header() && f.linkage != LoweredFunc::External) {
        return;
    }

    internal_assert(emitted.count(f.name) == 0)
        << "Function '" << f.name << "'  has already been emitted.\n";
    emitted.insert(f.name);

    const std::vector<LoweredArgument> &args = f.args;

    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].type.handle_type != NULL) {
            if (!args[i].type.handle_type->namespaces.empty()) {
                if (args[i].type.handle_type->inner_name.cpp_type_type != halide_cplusplus_type_name::Simple) {
                    for (size_t ns = 0; ns < args[i].type.handle_type->namespaces.size(); ns++ ) {
                        for (size_t indent = 0; indent < ns; indent++) {
                           stream << "    ";
                        }
                        stream << indent << "namespace " << args[i].type.handle_type->namespaces[ns] << " {\n";
                    }
                    for (size_t indent = 0; indent < args[i].type.handle_type->namespaces.size(); indent++) {
                        stream << "    ";
                    }
                    if (args[i].type.handle_type->inner_name.cpp_type_type != halide_cplusplus_type_name::Struct) {
                        stream << "struct " << args[i].type.handle_type->inner_name.name << ";\n";
                    } else {
                        stream << "class " << args[i].type.handle_type->inner_name.name << ";\n";
                    }
                    for (size_t ns = 0; ns < args[i].type.handle_type->namespaces.size(); ns++ ) {
                        for (size_t indent = 0; indent < ns; indent++) {
                           stream << "    ";
                        }
                        stream << indent << "}\n";
                    }
                }
            }
        }
    }

    have_user_context = false;
    for (size_t i = 0; i < args.size(); i++) {
        // TODO: check that its type is void *?
        have_user_context |= (args[i].name == "__user_context");
    }

    // Emit prototypes for any extern calls used.
    if (!is_header()) {
        stream << "\n";
        ExternCallPrototypes e(emitted);
        f.body.accept(&e);

        if (e.has_c_plus_plus_declarations()) {
            switch_to_c_or_c_plus_plus(COrCPlusPlus::CPlusPlus);
            e.emit_c_plus_plus_declarations(stream);
        }

        if (e.has_c_declarations()) {
            switch_to_c_or_c_plus_plus(COrCPlusPlus::C);
            e.emit_c_declarations(stream);
        }
    }

    switch_to_c_or_c_plus_plus(COrCPlusPlus::Default);
    stream << "\n";

    std::vector<std::string> namespaces;
    std::string simple_name = extract_namespaces(f.name, namespaces);

    if (!namespaces.empty()) {
        const char *separator = "";
        for (const auto &ns : namespaces) {
            stream << separator << "namespace " << ns << " {";
            separator = " ";
        }
        stream << "\n\n";
    }

    // Emit the function prototype
    if (f.linkage != LoweredFunc::External) {
        // If the function isn't public, mark it static.
        stream << "static ";
    }
    stream << "int " << simple_name << "(";
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].is_buffer()) {
            stream << "buffer_t *"
                   << print_name(args[i].name)
                   << "_buffer";
        } else {
            stream << print_type(args[i].type, AppendSpace)
                   << print_name(args[i].name);
        }

        if (i < args.size()-1) stream << ", ";
    }

    if (is_header()) {
        stream << ") HALIDE_FUNCTION_ATTRS;\n";
    } else {
        stream << ") HALIDE_FUNCTION_ATTRS {\n";
        indent += 1;

        // Unpack the buffer_t's
        for (size_t i = 0; i < args.size(); i++) {
            if (args[i].is_buffer()) {
                push_buffer(args[i].type, args[i].name);
            }
        }
        // Emit the body
        print(f.body);

        // Return success.
        do_indent();
        stream << "return 0;\n";

        indent -= 1;
        stream << "}\n";

        // Done with the buffer_t's, pop the associated symbols.
        for (size_t i = 0; i < args.size(); i++) {
            if (args[i].is_buffer()) {
                pop_buffer(args[i].name);
            }
        }
    }

    if (is_header()) {
        // If this is a header and we are here, we know this is an externally visible Func, so
        // declare the argv function.
        stream << "int " << simple_name << "_argv(void **args) HALIDE_FUNCTION_ATTRS;\n";

        // And also the metadata.
        stream << "// Result is never null and points to constant static data\n";
        stream << "const struct halide_filter_metadata_t *" << simple_name << "_metadata() HALIDE_FUNCTION_ATTRS;\n";
    }

    // Close namespaces here as metadata must be outside them
    if (!namespaces.empty()) {
        stream << "\n";
        for (size_t i = 0; i < namespaces.size(); i++) {
            stream << "}";
        }
        stream << " // Close namespaces ";
        const char *separator = "";
        for (const auto &ns : namespaces) {
            stream << separator << ns;
            separator = "::";
        }

        stream << "\n\n";
    }
}

void CodeGen_SDS::compile(const Buffer<> &buffer) {
    // Don't define buffers in headers.
    if (is_header()) {
        return;
    }

    string name = print_name(buffer.name());
    buffer_t b = *(buffer.raw_buffer());

    // Figure out the offset of the last pixel.
    size_t num_elems = 1;
    for (int d = 0; b.extent[d]; d++) {
        num_elems += b.stride[d] * (b.extent[d] - 1);
    }

    // Emit the data
    stream << "static uint8_t " << name << "_data[] __attribute__ ((aligned (32))) = {";
    for (size_t i = 0; i < num_elems * b.elem_size; i++) {
        if (i > 0) stream << ", ";
        stream << (int)(b.host[i]);
    }
    stream << "};\n";

    // Emit the buffer_t
    user_assert(b.host) << "Can't embed image: " << buffer.name() << " because it has a null host pointer\n";
    user_assert(!b.dev_dirty) << "Can't embed image: " << buffer.name() << "because it has a dirty device pointer\n";
    stream << "static buffer_t " << name << "_buffer = {"
           << "0, " // dev
           << "&" << name << "_data[0], " // host
           << "{" << b.extent[0] << ", " << b.extent[1] << ", " << b.extent[2] << ", " << b.extent[3] << "}, "
           << "{" << b.stride[0] << ", " << b.stride[1] << ", " << b.stride[2] << ", " << b.stride[3] << "}, "
           << "{" << b.min[0] << ", " << b.min[1] << ", " << b.min[2] << ", " << b.min[3] << "}, "
           << b.elem_size << ", "
           << "0, " // host_dirty
           << "0};\n"; //dev_dirty

    // Make a global pointer to it
    stream << "static buffer_t *" << name << " = &" << name << "_buffer;\n";
}

void CodeGen_SDS::push_buffer(Type t, const std::string &buffer_name) {
    string name = print_name(buffer_name);
    string buf_name = name + "_buffer";
    string type = print_type(t);
    do_indent();
    stream << type
           << " *"
           << name
           << " = ("
           << type
           << " *)("
           << buf_name
           << "->host);\n";
    Allocation alloc;
    alloc.type = t;
    allocations.push(buffer_name, alloc);
    do_indent();
    stream << "(void)" << name << ";\n";

    do_indent();
    stream << "uint8_t * "
           << name << "_host = "
           << buf_name << "->host;\n";
    do_indent();
    stream << "(void)" << name << "_host;\n";

    do_indent();
    stream << "const uint64_t "
           << name << "_dev = "
           << buf_name << "->dev;\n";
    do_indent();
    stream << "(void)" << name << "_dev;\n";

    for (int j = 0; j < 4; j++) {
        do_indent();
        stream << "const int32_t "
               << name
               << "_min_" << j << " = "
               << buf_name
               << "->min[" << j << "];\n";
        // emit a void cast to suppress "unused variable" warnings
        do_indent();
        stream << "(void)" << name << "_min_" << j << ";\n";
    }
    for (int j = 0; j < 4; j++) {
        do_indent();
        stream << "const int32_t "
               << name
               << "_extent_" << j << " = "
               << buf_name
               << "->extent[" << j << "];\n";
        do_indent();
        stream << "(void)" << name << "_extent_" << j << ";\n";
    }
    for (int j = 0; j < 4; j++) {
        do_indent();
        stream << "const int32_t "
               << name
               << "_stride_" << j << " = "
               << buf_name
               << "->stride[" << j << "];\n";
        do_indent();
        stream << "(void)" << name << "_stride_" << j << ";\n";
    }
    do_indent();
    stream << "const int32_t "
           << name
           << "_elem_size = "
           << buf_name
           << "->elem_size;\n";
    do_indent();
    stream << "(void)" << name << "_elem_size;\n";
}

void CodeGen_SDS::pop_buffer(const std::string &buffer_name) {
    //debug(3) << "pop_buffer : " << buffer_name << "\n";
    allocations.pop(buffer_name);
}

string CodeGen_SDS::print_expr(Expr e) {
    id = "$$ BAD ID $$";
    e.accept(this);
    return id;
}

void CodeGen_SDS::print_stmt(Stmt s) {
    s.accept(this);
}

string CodeGen_SDS::print_assignment(Type t, const std::string &rhs) {

    map<string, string>::iterator cached = cache.find(rhs);

    if (cached == cache.end()) {
        id = unique_name('_');
        do_indent();
        stream << print_type(t, AppendSpace) << id << " = " << rhs << ";\n";
        cache[rhs] = id;
    } else {
        id = cached->second;
    }
    return id;
}

void CodeGen_SDS::open_scope() {
    cache.clear();
    do_indent();
    indent++;
    stream << "{\n";
}

void CodeGen_SDS::close_scope(const std::string &comment) {
    cache.clear();
    indent--;
    do_indent();
    if (!comment.empty()) {
        stream << "} // " << comment << "\n";
    } else {
        stream << "}\n";
    }
}

void CodeGen_SDS::visit(const Variable *op) {
    id = print_name(op->name);
}

void CodeGen_SDS::visit(const Cast *op) {
    print_assignment(op->type, "(" + print_type(op->type) + ")(" + print_expr(op->value) + ")");
}

void CodeGen_SDS::visit_binop(Type t, Expr a, Expr b, const char * op) {
    string sa = print_expr(a);
    string sb = print_expr(b);
    print_assignment(t, sa + " " + op + " " + sb);
}

void CodeGen_SDS::visit(const Add *op) {
    visit_binop(op->type, op->a, op->b, "+");
}

void CodeGen_SDS::visit(const Sub *op) {
    visit_binop(op->type, op->a, op->b, "-");
}

void CodeGen_SDS::visit(const Mul *op) {
    visit_binop(op->type, op->a, op->b, "*");
}

void CodeGen_SDS::visit(const Div *op) {
    int bits;
    if (is_const_power_of_two_integer(op->b, &bits)) {
        ostringstream oss;
        oss << print_expr(op->a) << " >> " << bits;
        print_assignment(op->type, oss.str());
    } else if (op->type.is_int()) {
        print_expr(lower_euclidean_div(op->a, op->b));
    } else {
        visit_binop(op->type, op->a, op->b, "/");
    }
}

void CodeGen_SDS::visit(const Mod *op) {
    int bits;
    if (is_const_power_of_two_integer(op->b, &bits)) {
        ostringstream oss;
        oss << print_expr(op->a) << " & " << ((1 << bits)-1);
        print_assignment(op->type, oss.str());
    } else if (op->type.is_int()) {
        print_expr(lower_euclidean_mod(op->a, op->b));
    } else {
        visit_binop(op->type, op->a, op->b, "%");
    }
}

void CodeGen_SDS::visit(const Max *op) {
    print_expr(Call::make(op->type, "max", {op->a, op->b}, Call::Extern));
}

void CodeGen_SDS::visit(const Min *op) {
    print_expr(Call::make(op->type, "min", {op->a, op->b}, Call::Extern));
}

void CodeGen_SDS::visit(const EQ *op) {
    visit_binop(op->type, op->a, op->b, "==");
}

void CodeGen_SDS::visit(const NE *op) {
    visit_binop(op->type, op->a, op->b, "!=");
}

void CodeGen_SDS::visit(const LT *op) {
    visit_binop(op->type, op->a, op->b, "<");
}

void CodeGen_SDS::visit(const LE *op) {
    visit_binop(op->type, op->a, op->b, "<=");
}

void CodeGen_SDS::visit(const GT *op) {
    visit_binop(op->type, op->a, op->b, ">");
}

void CodeGen_SDS::visit(const GE *op) {
    visit_binop(op->type, op->a, op->b, ">=");
}

void CodeGen_SDS::visit(const Or *op) {
    visit_binop(op->type, op->a, op->b, "||");
}

void CodeGen_SDS::visit(const And *op) {
    visit_binop(op->type, op->a, op->b, "&&");
}

void CodeGen_SDS::visit(const Not *op) {
    print_assignment(op->type, "!(" + print_expr(op->a) + ")");
}

void CodeGen_SDS::visit(const IntImm *op) {
    if (op->type == Int(32)) {
        id = std::to_string(op->value);
    } else {
        print_assignment(op->type, "(" + print_type(op->type) + ")(" + std::to_string(op->value) + ")");
    }
}

void CodeGen_SDS::visit(const UIntImm *op) {
    print_assignment(op->type, "(" + print_type(op->type) + ")(" + std::to_string(op->value) + ")");
}

void CodeGen_SDS::visit(const StringImm *op) {
    ostringstream oss;
    oss << Expr(op);
    id = oss.str();
}

// NaN is the only float/double for which this is true... and
// surprisingly, there doesn't seem to be a portable isnan function
// (dsharlet).
template <typename T>
static bool isnan(T x) { return x != x; }

template <typename T>
static bool isinf(T x)
{
    return std::numeric_limits<T>::has_infinity && (
        x == std::numeric_limits<T>::infinity() ||
        x == -std::numeric_limits<T>::infinity());
}

void CodeGen_SDS::visit(const FloatImm *op) {
    if (isnan(op->value)) {
        id = "nan_f32()";
    } else if (isinf(op->value)) {
        if (op->value > 0) {
            id = "inf_f32()";
        } else {
            id = "neg_inf_f32()";
        }
    } else {
        // Write the constant as reinterpreted uint to avoid any bits lost in conversion.
        union {
            uint32_t as_uint;
            float as_float;
        } u;
        u.as_float = op->value;

        ostringstream oss;
        oss << "float_from_bits(" << u.as_uint << " /* " << u.as_float << " */)";
        id = oss.str();
    }
}

void CodeGen_SDS::visit(const Call *op) {

    internal_assert(op->call_type == Call::Extern ||
                    op->call_type == Call::ExternCPlusPlus ||
                    op->call_type == Call::PureExtern ||
                    op->call_type == Call::Intrinsic ||
                    op->call_type == Call::PureIntrinsic)
            << "Can only codegen extern calls and intrinsics\n";

    ostringstream rhs;

    if (op->is_intrinsic(Call::sds_windowbuffer_alloc)) {
        internal_assert(op->args.size() == 3);
        internal_assert(is_const(op->args[1]));
        internal_assert(is_const(op->args[2]));
        internal_assert(op->args[0].as<StringImm>());
        do_indent();
        stream << "hls::Window<"
               << *as_const_int(op->args[1])
               << ", "
               << *as_const_int(op->args[2])
               << ", "
               << print_type(op->type) << "> "
               << print_name(op->args[0].as<StringImm>()->value)
               << ";\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::sds_linebuffer_alloc)) {
        internal_assert(op->args.size() == 3);
        internal_assert(is_const(op->args[1]));
        internal_assert(is_const(op->args[2]));
        internal_assert(op->args[0].as<StringImm>());
        do_indent();
        stream << "hls::LineBuffer<"
               << *as_const_int(op->args[1])
               << ", "
               << *as_const_int(op->args[2])
               << ", "
               << print_type(op->type) << "> "
               << print_name(op->args[0].as<StringImm>()->value)
               << ";\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::sds_stream_alloc)) {
        internal_assert(op->args.size() == 1 && op->args[0].as<StringImm>());
        do_indent();
        stream << "hls::stream<" << print_type(op->type) << "> "
               << print_name(op->args[0].as<StringImm>()->value) << ";\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::sds_linebuffer_access) || op->is_intrinsic(Call::sds_windowbuffer_access)) {
        internal_assert(op->args.size() == 4 || op->args.size() == 3);
        internal_assert(op->args[0].as<StringImm>());
        if (op->args.size() == 4) {
            string x = print_expr(op->args[1]), y = print_expr(op->args[2]), value = print_expr(op->args[3]);
            do_indent();
            stream << print_name(op->args[0].as<StringImm>()->value)
                   << "(" << x << ", " << y << ")"
                   << " = "
                   << value << ";\n";
            id = "0";
        } else if (op->args.size() == 3) {
            rhs << print_name(op->args[0].as<StringImm>()->value)
                << "(" << print_expr(op->args[1]) << ", " << print_expr(op->args[2]) << ")";
            //print_assignment(op->type, rhs.str());
            id = rhs.str();
        }
        return ;
    } else if (op->is_intrinsic(Call::sds_stream_write)) {
        internal_assert(op->args.size() == 2);
        internal_assert(op->args[0].as<StringImm>());
        string value = print_expr(op->args[1]);
        do_indent();
        stream << print_name(op->args[0].as<StringImm>()->value)
               << ".write(" << value << ");\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::sds_stream_read)) {
        internal_assert(op->args.size() == 1);
        internal_assert(op->args[0].as<StringImm>());
        rhs << print_name(op->args[0].as<StringImm>()->value) << ".read()";
        print_assignment(op->type, rhs.str());
        return ;
    } else if (op->is_intrinsic(Call::sds_linebuffer_update)) {
        internal_assert(op->args.size() == 3);
        internal_assert(op->args[0].as<StringImm>());
        string column = print_expr(op->args[1]);
        string value = print_expr(op->args[2]);
        do_indent();
        stream << print_name(op->args[0].as<StringImm>()->value)
               << ".shift_up(" << column << ");\n";
               //<< ".shift_pixels_up(" << column << ");\n";
        do_indent();
        stream << print_name(op->args[0].as<StringImm>()->value)
               << ".insert_top(" << value << ", " << column << ");\n";
               //<< ".insert_bottom_row(" << value << ", " << column << ");\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::sds_windowbuffer_update)) {
        internal_assert(op->args.size() == 2);
        internal_assert(op->args[0].as<StringImm>());
        do_indent();
        stream << print_name(op->args[0].as<StringImm>()->value)
               << ".shift_left();\n";
               //<< ".shift_pixels_left();\n";
        id = "0";
        return ;
    } else if (op->is_intrinsic(Call::debug_to_file)) {
        internal_assert(op->args.size() == 3);
        const StringImm *string_imm = op->args[0].as<StringImm>();
        internal_assert(string_imm);
        string filename = string_imm->value;
        string typecode = print_expr(op->args[1]);
        string buffer = print_name(print_expr(op->args[2]));

        rhs << "halide_debug_to_file(";
        rhs << (have_user_context ? "__user_context_" : "nullptr");
        rhs << ", \"" + filename + "\", " + typecode;
        rhs << ", (struct buffer_t *)" << buffer;
        rhs << ")";
    } else if (op->is_intrinsic(Call::bitwise_and)) {
        internal_assert(op->args.size() == 2);
        string a0 = print_expr(op->args[0]);
        string a1 = print_expr(op->args[1]);
        rhs << a0 << " & " << a1;
    } else if (op->is_intrinsic(Call::bitwise_xor)) {
        internal_assert(op->args.size() == 2);
        string a0 = print_expr(op->args[0]);
        string a1 = print_expr(op->args[1]);
        rhs << a0 << " ^ " << a1;
    } else if (op->is_intrinsic(Call::bitwise_or)) {
        internal_assert(op->args.size() == 2);
        string a0 = print_expr(op->args[0]);
        string a1 = print_expr(op->args[1]);
        rhs << a0 << " | " << a1;
    } else if (op->is_intrinsic(Call::bitwise_not)) {
        internal_assert(op->args.size() == 1);
        rhs << "~" << print_expr(op->args[0]);
    } else if (op->is_intrinsic(Call::reinterpret)) {
        internal_assert(op->args.size() == 1);
        rhs << print_reinterpret(op->type, op->args[0]);
    } else if (op->is_intrinsic(Call::shift_left)) {
        internal_assert(op->args.size() == 2);
        string a0 = print_expr(op->args[0]);
        string a1 = print_expr(op->args[1]);
        rhs << a0 << " << " << a1;
    } else if (op->is_intrinsic(Call::shift_right)) {
        internal_assert(op->args.size() == 2);
        string a0 = print_expr(op->args[0]);
        string a1 = print_expr(op->args[1]);
        rhs << a0 << " >> " << a1;
    } else if (op->is_intrinsic(Call::lerp)) {
        internal_assert(op->args.size() == 3);
        Expr e = lower_lerp(op->args[0], op->args[1], op->args[2]);
        rhs << print_expr(e);
    } else if (op->is_intrinsic(Call::absd)) {
        internal_assert(op->args.size() == 2);
        Expr a = op->args[0];
        Expr b = op->args[1];
        Expr e = select(a < b, b - a, a - b);
        rhs << print_expr(e);
    } else if (op->is_intrinsic(Call::address_of)) {
        const Load *l = op->args[0].as<Load>();
        internal_assert(op->args.size() == 1 && l);
        rhs << "(("
            << print_type(l->type.element_of()) // index is in elements, not vectors.
            << " *)"
            << print_name(l->name)
            << " + "
            << print_expr(l->index)
            << ")";
    } else if (op->is_intrinsic(Call::return_second)) {
        internal_assert(op->args.size() == 2);
        string arg0 = print_expr(op->args[0]);
        string arg1 = print_expr(op->args[1]);
        rhs << "(" << arg0 << ", " << arg1 << ")";
    } else if (op->is_intrinsic(Call::if_then_else)) {
        internal_assert(op->args.size() == 3);

        string result_id = unique_name('_');

        do_indent();
        stream << print_type(op->args[1].type(), AppendSpace)
               << result_id << ";\n";

        string cond_id = print_expr(op->args[0]);

        do_indent();
        stream << "if (" << cond_id << ")\n";
        open_scope();
        string true_case = print_expr(op->args[1]);
        do_indent();
        stream << result_id << " = " << true_case << ";\n";
        close_scope("if " + cond_id);
        do_indent();
        stream << "else\n";
        open_scope();
        string false_case = print_expr(op->args[2]);
        do_indent();
        stream << result_id << " = " << false_case << ";\n";
        close_scope("if " + cond_id + " else");

        rhs << result_id;
    } else if (op->is_intrinsic(Call::abs)) {
        internal_assert(op->args.size() == 1);
        Expr a0 = op->args[0];
        rhs << print_expr(cast(op->type, select(a0 > 0, a0, -a0)));
    } else if (op->is_intrinsic(Call::memoize_expr)) {
        internal_assert(op->args.size() >= 1);
        string arg = print_expr(op->args[0]);
        rhs << "(" << arg << ")";
    } else if (op->is_intrinsic(Call::alloca)) {
        internal_assert(op->args.size() == 1);
        internal_assert(op->type.is_handle());
        if (op->type == type_of<struct buffer_t *>() &&
            is_const(op->args[0], (int) sizeof(buffer_t))) {
            do_indent();
            string buf_name = unique_name('b');
            stream << "buffer_t " << buf_name << ";";
            rhs << "&" << buf_name;
        } else {
            // Make a stack of uint64_ts
            string size = print_expr(simplify((op->args[0] + 7) / 8));
            do_indent();
            string array_name = unique_name('a');
            stream << "uint64_t " << array_name << "[" << size << "];";
            rhs << "(" << print_type(op->type) << ")(&" << array_name << ")";
        }
    } else if (op->is_intrinsic(Call::copy_memory)) {
        internal_assert(op->args.size() == 3);
        string dest = print_expr(op->args[0]);
        string src = print_expr(op->args[1]);
        string size = print_expr(op->args[2]);
        rhs << "memcpy(" << dest << ", " << src << ", " << size << ")";
    } else if (op->is_intrinsic(Call::make_struct)) {
        if (op->args.empty()) {
            rhs << "NULL";
        } else {
            // Emit a line something like:
            // struct {const int f_0, const char f_1, const int f_2} foo = {3, 'c', 4};

            // Get the args
            vector <string> values;
            for (size_t i = 0; i < op->args.size(); i++) {
                values.push_back(print_expr(op->args[i]));
            }
            do_indent();
            stream << "struct {";
            // List the types.
            for (size_t i = 0; i < op->args.size(); i++) {
                stream << "const " << print_type(op->args[i].type()) << " f_" << i << "; ";
            }
            string struct_name = unique_name('s');
            stream << "}  " << struct_name << " = {";
            // List the values.
            for (size_t i = 0; i < op->args.size(); i++) {
                if (i > 0) stream << ", ";
                stream << values[i];
            }
            stream << "};\n";
            // Return a pointer to it.
            rhs << "(&" << struct_name << ")";
        }
    } else if (op->is_intrinsic(Call::stringify)) {
        // Rewrite to an snprintf
        vector <string> printf_args;
        string format_string = "";
        for (size_t i = 0; i < op->args.size(); i++) {
            Type t = op->args[i].type();
            printf_args.push_back(print_expr(op->args[i]));
            if (t.is_int()) {
                format_string += "%lld";
                printf_args[i] = "(long long)(" + printf_args[i] + ")";
            } else if (t.is_uint()) {
                format_string += "%llu";
                printf_args[i] = "(long long unsigned)(" + printf_args[i] + ")";
            } else if (t.is_float()) {
                if (t.bits() == 32) {
                    format_string += "%f";
                } else {
                    format_string += "%e";
                }
            } else if (op->args[i].as<StringImm>()) {
                format_string += "%s";
            } else {
                internal_assert(t.is_handle());
                format_string += "%p";
            }

        }
        string buf_name = unique_name('b');
        do_indent();
        stream << "char " << buf_name << "[1024];\n";
        do_indent();
        stream << "snprintf(" << buf_name << ", 1024, \"" << format_string << "\"";
        for (size_t i = 0; i < printf_args.size(); i++) {
            stream << ", " << printf_args[i];
        }
        stream << ");\n";
        rhs << buf_name;

    } else if (op->is_intrinsic(Call::register_destructor)) {
        internal_assert(op->args.size() == 2);
        const StringImm *fn = op->args[0].as<StringImm>();
        internal_assert(fn);
        string arg = print_expr(op->args[1]);

        string call =
                fn->value + "(" +
                (have_user_context ? "__user_context_, " : "nullptr, ")
                + "arg);";

        do_indent();
        // Make a struct on the stack that calls the given function as a destructor
        string struct_name = unique_name('s');
        string instance_name = unique_name('d');
        stream << "struct " << struct_name << "{ "
               << "void *arg; "
               << struct_name << "(void *a) : arg((void *)a) {} "
               << "~" << struct_name << "() {" << call << "}"
               << "} " << instance_name << "(" << arg << ");\n";
        rhs << print_expr(0);
    } else if (op->is_intrinsic(Call::div_round_to_zero)) {
        rhs << print_expr(op->args[0]) << " / " << print_expr(op->args[1]);
    } else if (op->is_intrinsic(Call::mod_round_to_zero)) {
        rhs << print_expr(op->args[0]) << " % " << print_expr(op->args[1]);
    } else if (op->is_intrinsic(Call::signed_integer_overflow)) {
        user_error << "Signed integer overflow occurred during constant-folding. Signed"
                " integer overflow for int32 and int64 is undefined behavior in"
                " Halide.\n";
    } else if (op->is_intrinsic(Call::indeterminate_expression)) {
        user_error << "Indeterminate expression occurred during constant-folding.\n";
    } else if (op->call_type == Call::Intrinsic ||
               op->call_type == Call::PureIntrinsic) {
        // TODO: other intrinsics
        internal_error << "Unhandled intrinsic in C backend: " << op->name << '\n';
    } else {
        // Generic calls
        vector <string> args(op->args.size());
        for (size_t i = 0; i < op->args.size(); i++) {
            args[i] = print_expr(op->args[i]);
        }
        rhs << op->name << "(";

        if (function_takes_user_context(op->name)) {
            rhs << (have_user_context ? "__user_context_, " : "nullptr, ");
        }

        for (size_t i = 0; i < op->args.size(); i++) {
            if (i > 0) rhs << ", ";
            rhs << args[i];
        }
        rhs << ")";
    }

    print_assignment(op->type, rhs.str());
}

void CodeGen_SDS::visit(const Load *op) {

    Type t = op->type;
    bool type_cast_needed =
            !allocations.contains(op->name) ||
            allocations.get(op->name).type != t;

    ostringstream rhs;
    if (type_cast_needed) {
        rhs << "(("
            << print_type(op->type)
            << " *)"
            << print_name(op->name)
            << ")";
    } else {
        rhs << print_name(op->name);
    }
    rhs << "["
        << print_expr(op->index)
        << "]";

    print_assignment(op->type, rhs.str());
}

void CodeGen_SDS::visit(const Store *op) {

    Type t = op->value.type();

    bool type_cast_needed =
        t.is_handle() ||
        !allocations.contains(op->name) ||
        allocations.get(op->name).type != t;

    string id_index = print_expr(op->index);
    string id_value = print_expr(op->value);
    do_indent();

    if (type_cast_needed) {
        stream << "((const "
               << print_type(t)
               << " *)"
               << print_name(op->name)
               << ")";
    } else {
        stream << print_name(op->name);
    }
    stream << "["
           << id_index
           << "] = "
           << id_value
           << ";\n";

    cache.clear();
}

void CodeGen_SDS::visit(const Let *op) {
    string id_value = print_expr(op->value);
    Expr new_var = Variable::make(op->value.type(), id_value);
    Expr body = substitute(op->name, new_var, op->body);
    print_expr(body);
}

void CodeGen_SDS::visit(const Select *op) {
    ostringstream rhs;
    string true_val = print_expr(op->true_value);
    string false_val = print_expr(op->false_value);
    string cond = print_expr(op->condition);
    rhs << "(" << print_type(op->type) << ")"
        << "(" << cond
        << " ? " << true_val
        << " : " << false_val
        << ")";
    print_assignment(op->type, rhs.str());
}

void CodeGen_SDS::visit(const LetStmt *op) {
    string id_value = print_expr(op->value);
    Expr new_var = Variable::make(op->value.type(), id_value);
    Stmt body = substitute(op->name, new_var, op->body);
    body.accept(this);
}

void CodeGen_SDS::visit(const AssertStmt *op) {
    string id_cond = print_expr(op->condition);

    do_indent();
    // Halide asserts have different semantics to C asserts.  They're
    // supposed to clean up and make the containing function return
    // -1, so we can't use the C version of assert. Instead we convert
    // to an if statement.

    stream << "if (!" << id_cond << ") ";
    open_scope();
    string id_msg = print_expr(op->message);
    do_indent();
    stream << "return " << id_msg << ";\n";
    close_scope("");
}

void CodeGen_SDS::visit(const ProducerConsumer *op) {
    do_indent();
    if (op->is_producer) {
        stream << "// produce " << op->name << '\n';
    } else {
        stream << "// consume " << op->name << '\n';
    }
    print_stmt(op->body);
}

void CodeGen_SDS::visit(const Offload* offload) {
    std::ofstream header_file(offload->name + ".h");
    CodeGen_SDS cg1(header_file, SDSHardwareHeader, offload->name);
    cg1.compile(offload);

    std::ofstream function_file(offload->name + ".cpp");
    CodeGen_SDS cg2(function_file, SDSHardwareImplement, offload->name);
    cg2.compile(offload);

    do_indent();
    stream << offload->name << "(";
    for (size_t i = 0; i < offload->param.size(); ++i) {
        stream << print_name("dup$$" + offload->param[i].name);
        if (i != offload->param.size() - 1) {
            stream << ", ";
        }
    }
    stream << ");\n";
}

void CodeGen_SDS::compile(const Offload *offload) {
    stream << "void " << offload->name << "(\n";
    indent += 1;
    for (size_t i = 0; i < offload->param.size(); ++i) {
        do_indent();
        stream << print_type(offload->param[i].type, AppendSpace) << print_name(offload->param[i].name) << "[";
        for (size_t j = 0; j < offload->param[i].sub.size(); ++j) {
            stream << offload->param[i].sub[j].extent;
            if (j != offload->param[i].sub.size() - 1) {
                stream << " * ";
            }
        }
        if (i != offload->param.size() - 1) {
            stream << "],\n";
        } else {
            stream << "]";
        }
        if (!is_header()) {
            Allocation allocation;
            allocation.type = offload->param[i].type;
            allocation.free_function = "";
            allocations.push(offload->param[i].name, allocation);
        }
    }
    indent -= 1;

    if (is_header()) {
        stream << ");\n";
        return;
    }

    stream << ") {\n";
    indent += 1;
    do_indent(); stream << "#pragma HLS dataflow\n";

    // Emit the body
    print(offload->body);

    // Return success.
    //do_indent();
    //stream << "return 0;\n";

    indent -= 1;
    stream << "}\n";
    for (size_t i = 0; i < offload->param.size(); ++i) {
        if (!is_header()) {
            allocations.pop(offload->param[i].name);
        }
    }
}

void CodeGen_SDS::visit(const For *op) {
    if (op->for_type == ForType::Parallel) {
        internal_assert(false) << "There's no parallel in SDSoC!\n";
    } else {
        internal_assert(op->for_type == ForType::Serial || op->for_type == ForType::SDSPipeline)
            << "Can only emit serial or parallel for loops to C\n";
    }

    string id_min = print_expr(op->min);
    string id_extent = print_expr(op->extent);

    stream << print_name(op->name) << ":\n";
    do_indent();
    stream << "for (int "
           << print_name(op->name)
           << " = " << id_min
           << "; "
           << print_name(op->name)
           << " < " << id_min
           << " + " << id_extent
           << "; "
           << print_name(op->name)
           << "++)\n";
    if (op->for_type == ForType::SDSPipeline) {
        do_indent();
        stream << "#pragma HLS pipeline II=1\n";
        do_indent();
        stream << "#pragma HLS loop_flatten off\n";
    }

    open_scope();
    op->body.accept(this);
    close_scope("for " + print_name(op->name));

}

void CodeGen_SDS::visit(const Provide *op) {
    internal_error << "Cannot emit Provide statements as C\n";
}

void CodeGen_SDS::visit(const Allocate *op) {
    open_scope();

    // For sizes less than 8k, do a stack allocation
    bool on_stack = false;
    int32_t constant_size;
    string size_id;
    if (op->new_expr.defined()) {
        Allocation alloc;
        alloc.type = op->type;
        alloc.free_function = op->free_function;
        allocations.push(op->name, alloc);
        heap_allocations.push(op->name, 0);
        stream << print_type(op->type) << "*" << print_name(op->name) << " = (" << print_expr(op->new_expr) << ");\n";
    } else {
        constant_size = op->constant_allocation_size();
        if (constant_size > 0) {
            int64_t stack_bytes = constant_size * op->type.bytes();

            if (stack_bytes > ((int64_t(1) << 31) - 1)) {
                user_error << "Total size for allocation "
                           << op->name << " is constant but exceeds 2^31 - 1.\n";
            } else {
                size_id = print_expr(Expr(static_cast<int32_t>(constant_size)));
                if (can_allocation_fit_on_stack(stack_bytes)) {
                    on_stack = true;
                }
            }
        } else {
            // Check that the allocation is not scalar (if it were scalar
            // it would have constant size).
            internal_assert(op->extents.size() > 0);

            size_id = print_assignment(Int(64), print_expr(op->extents[0]));

            for (size_t i = 1; i < op->extents.size(); i++) {
                // Make the code a little less cluttered for two-dimensional case
                string new_size_id_rhs;
                string next_extent = print_expr(op->extents[i]);
                if (i > 1) {
                    new_size_id_rhs =  "(" + size_id + " > ((int64_t(1) << 31) - 1)) ? " + size_id + " : (" + size_id + " * " + next_extent + ")";
                } else {
                    new_size_id_rhs = size_id + " * " + next_extent;
                }
                size_id = print_assignment(Int(64), new_size_id_rhs);
            }
            do_indent();
            stream << "if ((" << size_id << " > ((int64_t(1) << 31) - 1)) || ((" << size_id <<
              " * sizeof(" << print_type(op->type) << ")) > ((int64_t(1) << 31) - 1)))\n";
            open_scope();
            do_indent();
            stream << "halide_error("
                   << (have_user_context ? "__user_context_" : "nullptr")
                   << ", \"32-bit signed overflow computing size of allocation "
                   << op->name << "\\n\");\n";
            do_indent();
            stream << "return -1;\n";
            close_scope("overflow test " + op->name);
        }

        // Check the condition to see if this allocation should actually be created.
        // If the allocation is on the stack, the only condition we can respect is
        // unconditional false (otherwise a non-constant-sized array declaration
        // will be generated).
        if (!on_stack || is_zero(op->condition)) {
            Expr conditional_size = Select::make(op->condition,
                                                 Var(size_id),
                                                 Expr(static_cast<int32_t>(0)));
            conditional_size = simplify(conditional_size);
            size_id = print_assignment(Int(64), print_expr(conditional_size));
        }

        Allocation alloc;
        alloc.type = op->type;
        allocations.push(op->name, alloc);

        do_indent();
        stream << print_type(op->type) << ' ';

        if (on_stack) {
            stream << print_name(op->name)
                   << "[" << size_id << "];\n";
            if (is_hardware()) {
                do_indent();
                stream << "#pragma HLS array_partition variable=" << print_name(op->name) << " complete dim=0\n";
            }
        } else {
            //if (is_hardware()) {
            //    internal_assert(false) << op->name << " is too large?!\n";
            //}
            stream << "*"
                   << print_name(op->name)
                   << " = ("
                   << print_type(op->type)
                   << " *)halide_malloc("
                   << (have_user_context ? "__user_context_" : "nullptr")
                   << ", sizeof("
                   << print_type(op->type)
                   << ")*" << size_id << ");\n";
            heap_allocations.push(op->name, 0);
        }
    }

    op->body.accept(this);

    // Should have been freed internally
    //FIXME: fuck this shit!
    //internal_assert(!allocations.contains(op->name));

    close_scope("alloc " + print_name(op->name));
}

void CodeGen_SDS::visit(const Free *op) {
    if (heap_allocations.contains(op->name)) {
        string free_function = allocations.get(op->name).free_function;
        if (free_function.empty()) {
            free_function = "halide_free";
        }

        do_indent();
        stream << free_function << "("
               << (have_user_context ? "__user_context_, " : "nullptr, ")
               << print_name(op->name)
               << ");\n";
        heap_allocations.pop(op->name);
    }
    allocations.pop(op->name);

    /*if (allocations.contains(op->name)) {
        debug(3) << "Still " << op->name << "!\n";
        debug(3) << allocations.get(op->name).free_function << "\n";
    } else {
        debug(3) << "None " << op->name << "\n";
    }*/
}

void CodeGen_SDS::visit(const Realize *op) {
    internal_error << "Cannot emit realize statements to C\n";
}

void CodeGen_SDS::visit(const IfThenElse *op) {
    string cond_id = print_expr(op->condition);

    do_indent();
    stream << "if (" << cond_id << ")\n";
    open_scope();
    op->then_case.accept(this);
    close_scope("if " + cond_id);

    if (op->else_case.defined()) {
        do_indent();
        stream << "else\n";
        open_scope();
        op->else_case.accept(this);
        close_scope("if " + cond_id + " else");
    }
}

void CodeGen_SDS::visit(const Evaluate *op) {
    if (is_const(op->value)) return;
    string id = print_expr(op->value);
    do_indent();
    stream << "(void)" << id << ";\n";
}

void CodeGen_SDS::visit(const Shuffle *op) {
    internal_error << "Cannot emit vector code to C\n";
}

void CodeGen_SDS::test() {
    //TODO: Untested...
    return ;
}

}
}
