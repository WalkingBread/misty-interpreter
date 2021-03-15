#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../lexer/Lexer.h"
#include "AST.h"

class Parser {
    public:
        Parser(Lexer* lexer);

        AST* expr();
    
    private:
        Lexer* lexer;
        Token* current_token;

        void eat(TokenType type);

        AST* term();
        AST* factor();
};

#endif