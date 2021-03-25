#include <iostream>
#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    current_token = lexer->get_next_token();
}

void Parser::error(Token* token) {
    std::string message = "Unexpected token: " + token->value;
    int line = token->line;
    int column = token->column;
    SyntaxError(line, column, message).cast();
}

std::vector<AST*> Parser::collection(TokenType ending) {
    std::vector<AST*> collection;
    if(!current_token->type_of(ending)) {
        AST* element = expr();
        collection.push_back(element);

        while(current_token->type_of(TokenType::COMMA)) {
            eat(TokenType::COMMA);

            element = expr();
            collection.push_back(element);
        }
    }
    eat(ending);

    return collection;
}

Compound* Parser::compound_statement() {
    eat(TokenType::L_CURLY);
    std::vector<AST*> nodes = statement_list();

    eat(TokenType::R_CURLY);

    Compound* root = new Compound();
    root->children = nodes;

    return root;
}

std::vector<AST*> Parser::statement_list() {
    AST* node = statement();

    std::vector<AST*> nodes = {node};

    while(current_token->type_of(TokenType::SEMICOLON)) {
        eat(current_token->type);
        nodes.push_back(statement());
    } 

    return nodes;
}

Variable* Parser::variable() {
    Variable* node = new Variable(current_token);
    eat(TokenType::IDENTIFIER);

    return node;
}

VariableDeclaration* Parser::standard_variable_declaration() {
    Variable* var = new Variable(current_token);
    eat(TokenType::IDENTIFIER);

    std::vector<Variable*> variables = { var };

    while(current_token->type_of(TokenType::COMMA)) {
        eat(TokenType::COMMA);
        Variable* var = variable();
        variables.push_back(var);
    }

    return new VariableDeclaration(variables);
}

VariableDeclaration* Parser::variable_declaration() {
    VariableDeclaration* var_decl = standard_variable_declaration();

    if(current_token->type_of(TokenType::ASSIGN)) {
        eat(TokenType::ASSIGN);

        Variable* left = var_decl->variables.at(0);
        AST* right = expr();
        Assign* assignment = new Assign(left, new Token(TokenType::ASSIGN, "="), right);

        var_decl->assignments.push_back(assignment);

        int i = 1;
        while(current_token->type_of(TokenType::COMMA)) {
            eat(TokenType::COMMA);
            if(i > var_decl->variables.size()) {
                error(current_token);
            }

            Variable* left = var_decl->variables.at(i);
            AST* right = expr();
            Assign* assignment = new Assign(left, new Token(TokenType::ASSIGN, "="), right);

            var_decl->assignments.push_back(assignment);
            i++;
        }
    }
    return var_decl;
}

NoOperator* Parser::empty() {
    return new NoOperator();
}

AST* Parser::identifier_statement() {
    Variable* left = variable();
    Token* token = current_token;

    if(token->type_of(TokenType::L_PAREN)) {
        return function_call(left);
    }

    eat(TokenType::ASSIGN);
    AST* right = expr();

    return new Assign(left, token, right);
}

IfCondition* Parser::if_statement() {
    eat(TokenType::IF);
    eat(TokenType::L_PAREN);
    AST* condition = expr();
    eat(TokenType::R_PAREN);

    Compound* statement = compound_statement();
    return new IfCondition(condition, statement);
}

IfCondition* Parser::else_statement() {
    eat(TokenType::ELSE);
    if(current_token->type_of(TokenType::IF)) {
        return if_statement();

    } else if(current_token->type_of(TokenType::L_CURLY)) {
        AST* condition = new Value(new Token(TokenType::BOOLEAN, Values::TRUE));

        return new IfCondition(condition, compound_statement());
    }
}

Print* Parser::print_statement() {
    eat(TokenType::PRINT);
    eat(TokenType::L_PAREN);
    AST* printable = expr();
    eat(TokenType::R_PAREN);

    return new Print(printable);
}

FunctionInit* Parser::function_init_statement() {
    eat(TokenType::FUNCTION);
    std::string func_name = current_token->value;
    eat(TokenType::IDENTIFIER);
    eat(TokenType::L_PAREN);

    VariableDeclaration* params = NULL;

    if(current_token->type_of(TokenType::IDENTIFIER)) {
        params = standard_variable_declaration();
    }

    eat(TokenType::R_PAREN);

    Compound* block = compound_statement();

    return new FunctionInit(func_name, params, block);
}

FunctionCall* Parser::function_call(AST* function) {
    eat(TokenType::L_PAREN);

    std::vector<AST*> params = collection(TokenType::R_PAREN);
    FunctionCall* func_call = new FunctionCall(function, params);

    while(current_token->type_of(TokenType::L_PAREN)) {
        eat(TokenType::L_PAREN);

        std::vector<AST*> params = collection(TokenType::R_PAREN);
        func_call = new FunctionCall(func_call, params);
    }

    return func_call;
}

AST* Parser::statement() {
    AST* node;

    switch(current_token->type) {
        case TokenType::VARIABLE_DECL:
        {
            eat(TokenType::VARIABLE_DECL);
            node = variable_declaration();
            break;
        }
        case TokenType::IDENTIFIER:
            node = identifier_statement();
            break;

        case TokenType::IF:
        {
            IfCondition* cond = if_statement();
            while(current_token->type_of(TokenType::ELSE)) {
                cond->elses.push_back(else_statement());
            }

            node = cond;
            break;
        }

        case TokenType::PRINT:
            node = print_statement();
            break;

        case TokenType::FUNCTION:
            node = function_init_statement();
            break;

        default:
            node = empty();
    }

    return node;
}

AST* Parser::term() {
    AST* node = factor();

    while(current_token->type_of(TokenType::MULT) || 
          current_token->type_of(TokenType::DIV) || 
          current_token->type_of(TokenType::INT_DIV) ||
          current_token->type_of(TokenType::MODULO)
    ) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinaryOperator(node, token, factor());
    }

    return node;
}

ArrayAccess* Parser::array_access(AST* array) {
    eat(TokenType::L_SQUARED);
    AST* index = expr();
    eat(TokenType::R_SQUARED);

    ArrayAccess* access = new ArrayAccess(array, index);

    while(current_token->type_of(TokenType::L_SQUARED)) {
        eat(TokenType::L_SQUARED);
        AST* index = expr();
        eat(TokenType::R_SQUARED);
        access = new ArrayAccess(access, index);
    }

    return access;
}

ArrayInit* Parser::array_init() {
    eat(TokenType::L_SQUARED);
    std::vector<AST*> elements = collection(TokenType::R_SQUARED);

    return new ArrayInit(elements);
}

AST* Parser::factor() {
    Token* token = current_token;

    switch(token->type) {
        case TokenType::PLUS:
        {
            eat(TokenType::PLUS);
            AST* expr = factor();
            return new UnaryOperator(token, expr);
        }
        
        case TokenType::MINUS:
        {
            eat(TokenType::MINUS);
            return new UnaryOperator(token, factor());
        }

        case TokenType::FLOAT:
        {
            eat(TokenType::FLOAT);
            return new Value(token);
        }

        case TokenType::NOT:
        {
            eat(TokenType::NOT);
            return new Negation(token, factor());
        }

        case TokenType::STRING:
        {
            eat(TokenType::STRING);
            return new Value(token);
        }

        case TokenType::BOOLEAN:
        {
            eat(TokenType::BOOLEAN);
            return new Value(token);
        }
        case TokenType::NONE:
        {
            eat(TokenType::NONE);
            return new Value(token);
        }
        case TokenType::L_PAREN:
        {
            eat(TokenType::L_PAREN);
            AST* node = expr();
            eat(TokenType::R_PAREN);
            return node;
        }
        case TokenType::L_SQUARED:
        {
            AST* node = array_init();
            if(current_token->type_of(TokenType::L_SQUARED)) {
                node = array_access(node);
            }
            return node;
        }
        
        default:
            AST* node = variable();
            if(current_token->type_of(TokenType::L_SQUARED)) {
                node = array_access(node);

            } else if(current_token->type_of(TokenType::L_PAREN)) {
                node = function_call(node);
            }
            return node;
    }
    
}

AST* Parser::eq_not_eq() {
    AST* node = sub_add();

    if(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
        std::vector<AST*> comparables = { node };
        std::vector<Token*> operators;

        while(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
            Token* op = current_token;
            eat(current_token->type);

            comparables.push_back(factor());
            operators.push_back(op);
        }
        node = new Compare(comparables, operators);
    }
    return node;
}

AST* Parser::sub_add() {
    AST* node = term();

    while(current_token->type_of(TokenType::PLUS) || current_token->type_of(TokenType::MINUS)) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinaryOperator(node, token, term());
    }

    return node;
}

AST* Parser::expr() {
    AST* node = eq_not_eq();

    while(current_token->type_of(TokenType::AND) || current_token->type_of(TokenType::OR)) {
        Token* op = current_token;
        eat(current_token->type);

        node = new DoubleCondition(node, op, expr());
    }

    return node;
}

void Parser::eat(TokenType type) {
    if(current_token->type == type) {
        current_token = lexer->get_next_token();
    } else {
        error(current_token);
    }
}

AST* Parser::parse() {
    Compound* program = new Compound();
    program->children = statement_list();

    if(!current_token->type_of(TokenType::END_OF_FILE)) {
        error(current_token);
    }

    return program;
}

