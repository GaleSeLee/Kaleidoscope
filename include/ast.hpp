#ifndef __KALEIDOSCOPE_INCLUDE_ASC_HPP__
#define __KALEIDOSCOPE_INCLUDE_ASC_HPP__
#include "lexer.hpp"

class ExprAST {
public:
    virtual ~ExprAST() {}
}

class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};
#endif //__KALEIDOSCOPE_INCLUDE_AST_HPP__
