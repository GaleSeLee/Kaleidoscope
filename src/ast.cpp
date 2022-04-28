#include "ast.hpp"

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
PrototypeAST::const string &getName() { return Name; }
FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> Body)
                         : Proto(std::move(Proto)), Body(std:move(Body)) {}
