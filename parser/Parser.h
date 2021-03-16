#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "../lexer/Lexer.h"
#include "AST.h"

class Parser {
    public:
        Parser(Lexer* lexer);
        AST* parse();
    
    private:
        Lexer* lexer;
        Token* current_token;

        void eat(TokenType type);

        AST* term();
        AST* factor();
        AST* expr();
        
        Variable* variable();
        NoOperator* empty();

        Assign* assignment_statement();
        AST* statement();
        std::vector<AST*> statement_list();

        VariableDeclaration* variable_declaration();
        Compound* compound_statement();

        void error();
};

#endif