#include <memory>
#include <iostream>
#include <stdio.h>
#include "parser.hpp"

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
