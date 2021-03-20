#include <iostream>
#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    current_token = lexer->get_next_token();
}

void Parser::error() {
    std::cout << "Parser: Invalid syntax." << std::endl;
    exit(0);
}

Compound* Parser::compound_statement() {
    eat(TokenType::L_CURLY);
    std::vector<AST*> nodes = statement_list();

    Compound* root = new Compound();
    root->children = nodes;

    return root;
}

std::vector<AST*> Parser::statement_list() {
    AST* node = statement();

    std::vector<AST*> nodes = {node};

    while(current_token->type_of(TokenType::SEMICOLON) || current_token->type_of(TokenType::R_CURLY)) {
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

VariableDeclaration* Parser::variable_declaration() {
    eat(TokenType::VARIABLE_DECL);
    Variable* var = new Variable(current_token);
    eat(TokenType::IDENTIFIER);

    std::vector<Variable*> variables = { var };

    while(current_token->type_of(TokenType::COMMA)) {
        eat(TokenType::COMMA);
        Variable* var = new Variable(current_token);
        eat(TokenType::IDENTIFIER);
        variables.push_back(var);
    }

    return new VariableDeclaration(variables);
}

NoOperator* Parser::empty() {
    return new NoOperator();
}

Assign* Parser::assignment_statement() {
    Variable* left = variable();
    Token* token = current_token;
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

AST* Parser::statement() {
    AST* node;

    switch(current_token->type) {
        case TokenType::VARIABLE_DECL:
            node = variable_declaration();
            break;

        case TokenType::IDENTIFIER:
            node = assignment_statement();
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

        default:
            node = empty();
    }

    return node;
}

AST* Parser::term() {
    AST* node = factor();

    if(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
        std::vector<AST*> comparables = { node };
        std::vector<Token*> operators;

        while(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
            Token* op = current_token;
            eat(current_token->type);

            comparables.push_back(factor());
            operators.push_back(op);
        }
        return new Compare(comparables, operators);
    }

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
        
        default:
            return variable();
    }
    
}

void Parser::eat(TokenType type) {
    if(current_token->type == type) {
        current_token = lexer->get_next_token();
    } else {
        int current_type = static_cast<int>(current_token->type);
        int expected_type = static_cast<int>(type);

        lexer->error("Unexpected token: Found: " + std::to_string(current_type) + " Instead of: " + std::to_string(expected_type));
    }
}

AST* Parser::expr() {
    AST* node = term();

    while(current_token->type_of(TokenType::AND) || current_token->type_of(TokenType::OR)) {
        Token* op = current_token;
        eat(current_token->type);

        node = new DoubleCondition(node, op, expr());
    }

    while(current_token->type_of(TokenType::PLUS) || current_token->type_of(TokenType::MINUS)) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinaryOperator(node, token, term());
    }

    return node;
}

AST* Parser::parse() {
    Compound* program = new Compound();
    program->children = statement_list();

    if(!current_token->type_of(TokenType::END_OF_FILE)) {
        std::cout << static_cast<int>(current_token->type) << std::endl;
        error();
    }

    return program;
}

