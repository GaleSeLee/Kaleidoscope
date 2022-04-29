#ifndef __KALEIDOSCOPE_INCLUDE_LEXER_HPP__
#define __KALEIDOSCOPE_INCLUDE_LEXER_HPP__
#include<iostream>
#include<cstring>
#include<stdlib>

namespace lexer {
enum Token {
    tok_eof = -1;
    tok_def = -2;
    tok_extern = -3;
    tok_identifier = -4;
    tok_number = -5;
}

int gettok();
}
#endif //__KALEDOSCOPE_INCLUDE_LEXER_HPP__
