#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include "Token.h"

class Lexer {
    public:
        std::map<std::string, Token*> keywords;
        
        Lexer(std::string code);

        Token* get_next_token();
        void error();

    private:
        int pos;
        std::string code;
        char current_char;

        void skip_whitespace();
        void advance();
        
        char peek();
        Token* number();
        Token* string();

        Token* handle_identifiers();

        void create_keywords();
};

#endif