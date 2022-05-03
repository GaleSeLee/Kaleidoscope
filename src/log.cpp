#ifndef __KALEIDOSCOPE_INCLUDE_LOG_HPP__
#define __KALEIDOSCOPE_INCLUDE_LOG_HPP__
#include "log.hpp"
#include "codegen.hpp"
std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}
#endif // __KALEIDOSCOPE_INCLUDE_LOG_HPP__
