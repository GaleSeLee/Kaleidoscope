#ifndef __KALEIDOSCOPE_INCLUDE_LOG_HPP__
#define __KALEIDOSCOPE_INCLUDE_LOG_HPP__
#include <memory>
#include <iostream>
#include <stdio.h>
#include "parser.hpp"

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
#endif //__KALEIDOSCOPE_INCLUDE_LOG_HPP__
