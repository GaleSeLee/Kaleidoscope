#ifndef __KALEIDOSCOPE_INCLUDE_PARSER_HPP__
#define __KALEIDOSCOPE_INCLUDE_PARSER_HPP__
#include "lexer.hpp"
#include <vector>
#include <memory>

// The basic Expr abstract syntax tree
class ExprAST {
public:
    virtual ~ExprAST(); 
}

// Expression class for the digit
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val);
};

// Expression class for a variable
class VariableExprAST : public ExprAST {
    std::string Name;

public: 
    VariableExprAST(const std::string &Name); 
};

// Expression class for a  binary operator
class BinaryExprAST : public ExprAST {
    char op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS);
}

// Expression class for a function call
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args);
};

// Expression class for the function call, for instance functionname(id*)
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args);
    const std::string &getName(); 
};

// Expression class for the function, contains body and name.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body);
};

namespace parser {
    int CurTok;
    int getNextToken();
    int GetTokPrecedence();
    std::map<char, int> BinOpPrecedence;
    BinOpPrecedence['<'] = 10;
    BinOpPrecedence['+'] = 20;
    BinOpPrecedence['-'] = 30;
    BinOpPrecedence['*'] = 40;
    std::unique_ptr<ExprAST> NumberExpr();
    std::unique_ptr<ExprAST> ParenExpr();
    std::unique_ptr<ExprAST> IdentifierExpr();
    std::unique_ptr<ExprAST> Expression();
    std::unique_ptr<ExprAST> Parse();
    std::unique_ptr<ExprAST> BinOpRHS(int, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeAST> Prototype();
    std::unique_ptr<FunctionAST> Definition();
    std::unique_ptr<FunctionAST> TopLevelExpr();
    void HandleDefinition();
    void HandleTopLevelExpression();
    void MainLoop();
}
#endif //__KALEIDOSCOPE_INCLUDE_PARSER_HPP__
