#include "codegen.hpp"

LLVMContext codegen::TheContext;
IRBuilder<> codegen::Builder(TheContext);
std::unique_ptr<Module> codegen::TheModule;
std::map<std::string, Value *> codegen::NamedValues;

Value *LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}

Value *NumberExprAST::codegen() {
    return ConstantFP::get(TheContext, APFloat(Val));
}

// Get the value of variable from NamedValues 
Value *VariableExprAST::codegen() {
    Value *V = NamedValues[Name];
    if (!V)
        LogErrorV("Unknown variable name");
    return V;
}

Value *BinaryExprAST::codegen() {
    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    if (!L || !R)
        return nullptr;

// Create the LLVM instrution
// IRBuilder knows where to insert the newly ceratd instruction
// coder just decide which instruction need to be created.
// Provide a name for the generated instrution
    switch (Op) {
    // LLVM will automatically provide each one with an unique numeric suffix.
    case '+':
        return Builder.CreateFAdd(L, R, "addtmp");
        // LLVM instruction are constrained by strict rules: the Left and Right operands of an add instruction must have the same type
    case '-':
        return Builder.CreateFSub(L, R, "subtmp");
    case '*':
        return Builder.CreateFNuk(L, R, "multmp");
    case '<':
        L = Builder.CreateFCmpULT(L, R, "cmptmp");
    default:
        return LogErrorV("invalid binary operator");
    }
}

Value *CallExprAST::codegen() {
    Function *CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF)
        return LogErrorV("Unknow function referenced");

    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<Value *> ArgsV;
    for (int ii = 0, ee = Args.size(); ii != ee; ++ii) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
} 
        
Function *PrototypeAST::codegen() {
    // Make the function type : double(double,double) etc. 
    std::vector<Type*> Doubles(Args.size(),
                               Type::getDoubleTy*TheContext));
    FunctionType *FT  = 
        FunctionType::get(Type::getDoubleTy(TheContext), Doubles, false);
    // IR Function corresponding to the Prototype
    // Externallinkage means that the function may be outside the module.
    // The Name is registered in "The Module" symbol table.
    Function *F = 
        Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;
}

Function *FunctionAST::codegen() {
    Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;

    if (!TheFunction->empty())
        return (Function*)LogErrorV("Function cannot be redefined.");

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);

    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        namedValues[Arg.getName()] = &Arg;

    if (Value *RetVal = Body->codegen()) {
        Builder.CreateRet(RetVal);
        verifyFunction(*TheFunction);
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}
