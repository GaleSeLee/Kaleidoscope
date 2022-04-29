#include "parser.hpp"
#include "log.hpp"

ExprAST::~ExprAST(){}
NumberExprAST::NumberExprAST(double Val) : Val(Val) {}
VariableExprAST::VariableExprAST(const std::string &Name) : Name(Name) {}
BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, 
                             std::unique_ptr<ExprAST> RHS)
                             : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
CallExprAST::CallExprAST(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprAST>> Args)
                         : Callee(Callee), Args(std::move(Args)) {}
PrototypeAST::PrototypeAST(const std::string &name, std::vector<std::string> Args
                           : Name(name), Args(std::move(Args)) {}
const string & PrototypeAST::getName() { return Name; }
FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> Body)
                         : Proto(std::move(Proto)), Body(std:move(Body)) {}

int parser::getNextToken() {
    return parser::CurTok = lexer::gettok();
}

std::unique_ptr<ExprAST> parser::NumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    parser::getNextToken();
    return std::move(Result);
}

// One question : How could I identify the string is a ParenExpr or an CallExpr
// Is there a previous pretreanment before enter the ParenExpr or IdentifierExpr ?
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

std::unique_ptr<ExprAST> parser::IdentifierExpr() {
    std::string IdName = lexer::IdentifierStr;

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

// Binary Expression Parsing
int parser::GetTokPrecedence() {
    if (!isascii(parser::CurTok))
        return -1;
    
    int TokPrec = BinOpPrecedence[CurTok];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

std::unique_ptr<ExprAST> parser::Expression() {
    auto LHS = parser::Parse();
    if (!LHS)
        return nullptr;

    return BinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprASC> parser::BinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
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

std::unique_ptr<PrototypeAST> parser::Prototype() {
    if (parser::CurTok != lexer::IdentifierStr)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = lexer::IdentifierStr;
    parser::getNextToken();

    if (parser::CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (parser::getNextToken() == lexer::tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (parser::CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    parser::getNextToken();
    
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

int main() {
}

