#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "../lexer/Lexer.h"
#include "../utils/Values.h"
#include "../utils/Error.h"
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
        IfCondition* if_statement();
        IfCondition* else_statement();
        Compound* compound_statement();
        Print* print_statement();

        ArrayInit* array_init();

        void error(Token* token);
};

#endif