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
    eat(TokenType::R_CURLY);

    Compound* root = new Compound();
    root->children = nodes;

    return root;
}

std::vector<AST*> Parser::statement_list() {
    AST* node = statement();

    std::vector<AST*> nodes = {node};

    while(current_token->type_of(TokenType::SEMICOLON)) {
        eat(TokenType::SEMICOLON);
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

AST* Parser::statement() {
    AST* node;

    switch(current_token->type) {
        case TokenType::L_CURLY:
            node = compound_statement();
            break;

        case TokenType::VARIABLE_DECL:
            node = variable_declaration();
            break;

        case TokenType::IDENTIFIER:
            node = assignment_statement();
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
        lexer->error();
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

