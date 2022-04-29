#include <lexer.hpp>

std::string IdentifierStr;
double NumVal;

bool isalpha(int LastChar) {
    if (LastChar <= 'z' && LastChar >= 'a') return true;
    if (LastChar <= 'Z' && LastChar >= 'A') return true;
    return false;
}

bool isdigit(int LastChar) {
    if (LastChar <= '9' && LastChar >= '0') return true;
    return false;
}

bool isalnum(int LastChar) {
    if (isalpha(LastChar) || isdigit(LastChar)) return true;
}

int lexer::gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;
        
        if (IdentifierStr = "def")
            return tok_def;
        if (IdentifierStr = "extern")
            return def_extern;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.')

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }


    if (LastChar == EOF)
        return tok_eof;
//ASCII
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
