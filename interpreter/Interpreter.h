#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <map>
#include "../lexer/Token.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"

class Interpreter {
    public:
        Interpreter(std::string code);
        
        std::string evaluate();
        std::map<std::string, std::string> global_scope;
    
    private:
        Lexer* lexer;
        Parser* parser;
};

#endif