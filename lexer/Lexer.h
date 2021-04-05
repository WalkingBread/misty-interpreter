#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include <fstream>
#include "Token.h"
#include "../utils/Error.h"

class Lexer {
    public:
        std::map<std::string, TokenType> keywords;
        
        Lexer(std::string code);

        Token* get_next_token();
        void error(std::string message);

        std::string path;

        int line;
        int column;

    private:
        int pos;
        std::string code;
        char current_char;

        void skip_whitespace();
        void advance();
        
        char peek();
        Token* number();
        Token* string();

        Token* handle_build_in_lib();
        Token* handle_identifiers();

        Token* create_token(TokenType type, std::string value);

        void create_keywords();
};

#endif