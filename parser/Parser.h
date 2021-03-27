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

        bool inside_func;

        void eat(TokenType type);

        std::vector<AST*> collection(TokenType ending);

        AST* sub_add();
        AST* eq_not_eq();
        AST* term();
        AST* factor();
        AST* expr();
        
        Variable* variable();
        NoOperator* empty();

        AST* identifier_statement();
        AST* statement();
        std::vector<AST*> statement_list();

        VariableDeclaration* standard_variable_declaration();
        VariableDeclaration* variable_declaration();
        IfCondition* if_statement();
        IfCondition* else_statement();
        Compound* compound_statement();
        Print* print_statement();
        Return* return_statement();

        ArrayInit* array_init();
        ArrayAccess* array_access(AST* array);

        FunctionInit* function_init_statement();
        FunctionCall* function_call(AST* function);

        void error(Token* token);
};

#endif