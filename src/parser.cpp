#include "parser.hpp"
#include "log.hpp"
#include "stdio.h"

int parser::CurTok;
std::map<char, int> parser::BinOpPrecedence;

ExprAST::~ExprAST(){}

NumberExprAST::NumberExprAST(double Val) : Val(Val) {}

VariableExprAST::VariableExprAST(const std::string &Name) : Name(Name) {}

BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, 
                             std::unique_ptr<ExprAST> RHS)
                             : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

CallExprAST::CallExprAST(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprAST>> Args)
                         : Callee(Callee), Args(std::move(Args)) {}

PrototypeAST::PrototypeAST(const std::string &name, std::vector<std::string> Args)
                           : Name(name), Args(std::move(Args)) {}

const std::string & PrototypeAST::getName() { return Name; }

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> Body)
                         : Proto(std::move(Proto)), Body(std::move(Body)) {}

// Get the next token from lexer
int parser::getNextToken() {
    return parser::CurTok = lexer::gettok();
}

// Gen a NumberExprAST with NumVal which is generated by lexer
std::unique_ptr<ExprAST> parser::NumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(lexer::NumVal);
    parser::getNextToken();
    return std::move(Result);
}

// Gen an Expression AST with (expression)
std::unique_ptr<ExprAST> parser::ParenExpr() {
    //eat (
    parser::getNextToken();
    auto V = parser::Expression();
    if (!V)
        return nullptr;

    if (parser::CurTok != ')')
        return LogError("Expected ')'");

    parser::getNextToken(); //eat )
    return V;
}

// Gen an Expression AST to handle variable and function call
std::unique_ptr<ExprAST> parser::IdentifierExpr() {
    std::string IdName = lexer::IdentifierStr;
    
    // get the next token to identify the variable or function call
    parser::getNextToken();

    // simple variable or call
    if (parser::CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);
    
    parser::getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (parser::CurTok != ')') {
        while (1) {
            if (auto Arg = parser::Expression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            
            if (parser::CurTok == ')')
                break;

            if (parser::CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            parser::getNextToken();
        }
    }
    parser::getNextToken();
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// Classify the token from lexer and parser
std::unique_ptr<ExprAST> parser::Parse() {
    switch (parser::CurTok) {
    default:
        return LogError("unknown token when expecting an expression");
    case lexer::tok_identifier:
        return parser::IdentifierExpr();
    case lexer::tok_number:
        return parser::NumberExpr();
    case '(':
        return parser::ParenExpr();
    }
}

// Get the Procedence of the operation for the expression parsering
int parser::GetTokPrecedence() {
    if (!isascii(parser::CurTok))
        return -1;
    
    int TokPrec = parser::BinOpPrecedence[CurTok];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

// There is one detail that needs to be confirmed.
// Handle the expression that has been passed
std::unique_ptr<ExprAST> parser::Expression() {
    auto LHS = parser::Parse();
    if (!LHS)
        return nullptr;

    return BinOpRHS(0, std::move(LHS));
}

// handle the precedence of expression
// TODO 
std::unique_ptr<ExprAST> parser::BinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (1) {
        int TokPrec = parser::GetTokPrecedence();
        if (TokPrec < ExprPrec)
            return LHS;
        
        int BinOp = parser::CurTok;
        parser::getNextToken();

        auto RHS = parser::Parse();
        if (!RHS)
            return nullptr;

        int NextPrec = parser::GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = parser::BinOpRHS(TokPrec+1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// Prototype ::= id ( id* )  no ',' ?
std::unique_ptr<PrototypeAST> parser::Prototype() {
    if (parser::CurTok != lexer::tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = lexer::IdentifierStr;
    parser::getNextToken();

    if (parser::CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (parser::getNextToken() == lexer::tok_identifier)
        ArgNames.push_back(lexer::IdentifierStr);
    if (parser::CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    parser::getNextToken();
    
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// Definition ::= def
std::unique_ptr<FunctionAST> parser::Definition() {
    parser::getNextToken();
    auto Proto = parser::Prototype();
    if (!Proto) return nullptr;

    if (auto E = parser::Expression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

// Externexpr ::= sin cos
std::unique_ptr<PrototypeAST> parser::Extern() {
    parser::getNextToken();
    return parser::Prototype();
}

// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> parser::TopLevelExpr() {
    if (auto E = parser::Expression()) {
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void parser::HandleDef() {
    if (auto FnAST = parser::Definition()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition: \n");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    }   
    else 
        parser::getNextToken();
}

void parser::HandleTLE() {
    if (auto FnAST = parser::TopLevelExpr()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read Top-level expression: \n");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            FnIR->eraseFromParent();
        }
    }
    else 
        parser::getNextToken();
}

void parser::HandleExtern() {
    if (auto ProtoAST = parser::Extern()) {
        if (auto *FnIR = ProtoAST->codegen()) {
        fprintf(stderr, "Read extern: ");
        FnIR->print(llvm::errs());
        fprintf(stderr, "\n");
        }
    }
    else 
        parser::getNextToken();
}  

void parser::MainLoop() {
    while(1) {
        switch (parser::CurTok) {
        case lexer::tok_eof:
            return;
        case ';':
            parser::getNextToken();
            break;
        case lexer::tok_def:
            parser::HandleDef();
            break;
        case lexer::tok_extern:
            parser::HandleExtern();
            break;
        default:
            parser::HandleTLE();
            break;
        }
        
        fprintf(stderr, "ready> ");
    }
}

int main() {
    parser::BinOpPrecedence['<'] = 10;
    parser::BinOpPrecedence['+'] = 20;
    parser::BinOpPrecedence['-'] = 30;
    parser::BinOpPrecedence['*'] = 40;
    printf("ready> ");
    parser::getNextToken();
    

    parser::MainLoop();

    return 0;
}


