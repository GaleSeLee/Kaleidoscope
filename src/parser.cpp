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

    if (CurTok != ')')
        return LogError("Expected ')'");

    parser::getNextToken(); //eat )
    return V;
}

std::unique_ptr<ExprAST> parser::IdentifierExpr() {
    std::string IdName = lexer::IdentifierStr;

    parser::getNextToken();
    // simple variable or call
    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);
    
    parser::getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')') {
        while (1) {
            if (auto Arg = parser::Expression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            
            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            parser::getNextToken();
        }
    }
    parser::getNextToken();
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}




