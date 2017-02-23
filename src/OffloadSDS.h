#ifndef OFFLOAD_SDS_H
#define OFFLOAD_SDS_H

#include "Bounds.h"
#include "ExprUsesVar.h"
#include "IR.h"
#include "IRVisitor.h"
#include "IRMutator.h"
#include "Simplify.h"
#include "Substitute.h"
#include "Var.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

namespace Halide {
namespace Internal {

using std::vector;
using std::string;
using std::map;

/** Offload all of the functions specified by user to FPGA logic. */
Stmt offload_functions(Stmt s,
                      const vector<Function> &outputs,
                      const map<string, Function> &env);

}
}

#endif
