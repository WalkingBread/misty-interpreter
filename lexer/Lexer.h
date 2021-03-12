#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "Token.h"

class Lexer {
    public:
        Lexer(std::string code);

        Token* get_next_token();
        void error();

    private:
        int pos;
        std::string code;
        char current_char;

        void skip_whitespace();
        void advance();

        int integer();
};

#endif