#include "lexer.hpp"

int lexer::gettok() {
    int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        lexer::IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            lexer::IdentifierStr += LastChar;
        
        if (lexer::IdentifierStr = "def")
            return lexer::tok_def;
        if (lexer::IdentifierStr = "extern")
            return lexer::def_extern;
        return lexer::tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.')

        lexer::NumVal = strtod(NumStr.c_str(), 0);
        return lexer::tok_number;
    }

    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return lexer::gettok();
    }


    if (LastChar == EOF)
        return lexer::tok_eof;
//ASCII
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
