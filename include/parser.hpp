#ifndef __KALEIDOSCOPE_INCLUDE_PARSER_HPP__
#define __KALEIDOSCOPE_INCLUDE_PARSER_HPP__
#include "lexer.hpp"
#include "codegen.hpp"
#include <vector>
#include <memory>
#include <map>

// The basic Expr abstract syntax tree
class ExprAST {
public:
    virtual ~ExprAST(); 
    // llvm::Value is a class used to represent a SSA register in LLVM
    virtual llvm::Value *codegen() = 0;
};

// Expression class for the digit
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val);
    llvm::Value *codegen() override;
};

// Expression class for a variable
class VariableExprAST : public ExprAST {
    std::string Name;

public: 
    VariableExprAST(const std::string &Name); 
    llvm::Value *codegen() override;
};

// Expression class for a  binary operator
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS);
    llvm::Value *codegen() override;
};

// Expression class for a function call
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args);
    llvm::Value *codegen() override;
};

// Expression class for the function call, for instance functionname(id*)
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args);
    const std::string &getName(); 
    llvm::Function *codegen();
};

// Expression class for the function, contains body and name.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body);
    llvm::Function *codegen();
};

namespace parser {
    extern int CurTok;
    int getNextToken();
    int GetTokPrecedence();
    extern std::map<char, int> BinOpPrecedence;
    std::unique_ptr<ExprAST> NumberExpr();
    std::unique_ptr<ExprAST> ParenExpr();
    std::unique_ptr<ExprAST> IdentifierExpr();
    std::unique_ptr<ExprAST> Expression();
    std::unique_ptr<ExprAST> Parse();
    std::unique_ptr<ExprAST> BinOpRHS(int, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeAST> Prototype();
    std::unique_ptr<PrototypeAST> Extern();
    std::unique_ptr<FunctionAST> Definition();
    std::unique_ptr<FunctionAST> TopLevelExpr();
    void HandleDef();
    void HandleTLE();
    void HandleExtern();
    void MainLoop();
}
#endif //__KALEIDOSCOPE_INCLUDE_PARSER_HPP__
