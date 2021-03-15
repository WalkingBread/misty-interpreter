#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "../lexer/Token.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"

class Interpreter {
    public:
        Interpreter(std::string code);
        
        std::string evaluate();
    
    private:
        Lexer* lexer;
        Parser* parser;
};

#endif