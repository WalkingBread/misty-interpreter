#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "lexer/Token.h"
#include "lexer/Lexer.h"

class Interpreter {
    public:
        Interpreter(std::string code);

        int expr();
    
    private:
        Lexer* lexer;
        Token* current_token;

        void eat(TokenType type);

        int term();
};

#endif