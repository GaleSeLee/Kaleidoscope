#ifndef __KALEIDOSCOPE_INCLUDE_CODEGEN_HPP__
#define __KALEIDOSCOPE_INCLUDE_CODEGEN_HPP__

namespace codegen {

extern LLVMContext TheContext;
extern IRBuilder<> Builder(TheContext);
extern std::unique_ptr<Module> TheModule;
// Namedvalues map keeps track of which values are defined in the current scope.
extern std::map<std::string, Value *> NamedValues;

}
// Function
Value *LogErrorV(const char *Str);

#endif // __KALE
