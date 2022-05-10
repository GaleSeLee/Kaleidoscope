#include "codegen.hpp"

std::unique_ptr<llvm::LLVMContext> codegen::TheContext = std::make_unique<llvm::LLVMContext>();
std::unique_ptr<llvm::IRBuilder<>> codegen::Builder = std::make_unique<llvm::IRBuilder<>>(*codegen::TheContext);
std::unique_ptr<llvm::Module> codegen::TheModule = std::make_unique<llvm::Module>("My first jit", *codegen::TheContext);
std::map<std::string, llvm::Value *> codegen::NamedValues;

llvm::Value *LogErrorV(const char *Str) {
    LogErrorV(Str);
    return nullptr;
}

llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*codegen::TheContext, llvm::APFloat(this->Val));
}

// Get the value of variable from NamedValues 
llvm::Value *VariableExprAST::codegen() {
    llvm::Value *V = codegen::NamedValues[this->Name];
    if (!V)
        LogErrorV("Unknown variable name");
    return V;
}

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = this->LHS->codegen();
    llvm::Value *R = this->RHS->codegen();
    if (!L || !R)
        return nullptr;

// Create the LLVM instrution
// IRBuilder knows where to insert the newly ceratd instruction
// coder just decide which instruction need to be created.
// Provide a name for the generated instrution
    switch (this->Op) {
    // LLVM will automatically provide each one with an unique numeric suffix.
    case '+':
        return codegen::Builder->CreateFAdd(L, R, "addtmp");
        // LLVM instruction are constrained by strict rules: the Left and Right operands of an add instruction must have the same type
    case '-':
        return codegen::Builder->CreateFSub(L, R, "subtmp");
    case '*':
        return codegen::Builder->CreateFMul(L, R, "multmp");
    case '<':
        L = codegen::Builder->CreateFCmpULT(L, R, "cmptmp");
    default:
        return LogErrorV("invalid binary operator");
    }
}

llvm::Value *CallExprAST::codegen() {
    llvm::Function *CalleeF = codegen::TheModule->getFunction(this->Callee);
    if (!CalleeF)
        return LogErrorV("Unknow function referenced");

    if (CalleeF->arg_size() != this->Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value *> ArgsV;
    for (int ii = 0, ee = this->Args.size(); ii != ee; ++ii) {
        ArgsV.push_back(Args[ii]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return codegen::Builder->CreateCall(CalleeF, ArgsV, "calltmp");
} 
        
llvm::Function *PrototypeAST::codegen() {
    // Make the function type : double(double,double) etc. 
    std::vector<llvm::Type*> Doubles(this->Args.size(),
                               llvm::Type::getDoubleTy (*codegen::TheContext));
    llvm::FunctionType *FT  = 
        llvm::FunctionType::get(llvm::Type::getDoubleTy(*codegen::TheContext), Doubles, false);
    // IR Function corresponding to the Prototype
    // Externallinkage means that the function may be outside the module.
    // The Name is registered in "The llvm::Module" symbol table.
    llvm::Function *F = 
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, codegen::TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(this->Args[Idx++]);

    return F;
}

llvm::Function *FunctionAST::codegen() {
    auto test = Proto->getName();
    llvm::Function *TheFunction = codegen::TheModule->getFunction(Proto->getName());


    if (!TheFunction)
        TheFunction = this->Proto->codegen();

    if (!TheFunction)
        return nullptr;

    if (!TheFunction->empty())
        return (llvm::Function*)LogErrorV("Function cannot be redefined.");

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*codegen::TheContext, "entry", TheFunction);
    codegen::Builder->SetInsertPoint(BB);

    codegen::NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        codegen::NamedValues[std::string(Arg.getName())] = &Arg;

    if (llvm::Value *RetVal = Body->codegen()) {
        codegen::Builder->CreateRet(RetVal);
        llvm::verifyFunction(*TheFunction);
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}
