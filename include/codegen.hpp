#ifndef __KALEIDOSCOPE_INCLUDE_CODEGEN_HPP__
#define __KALEIDOSCOPE_INCLUDE_CODEGEN_HPP__

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"

namespace codegen {

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
// Namedvalues map keeps track of which values are defined in the current scope.
extern std::map<std::string, llvm::Value *> NamedValues;

}
// Function
llvm::Value *LogErrorV(const char *Str);

#endif // __KALE
