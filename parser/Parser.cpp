#include <iostream>
#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    current_token = lexer->get_next_token();
}

AST* Parser::term() {
    AST* node = factor();

    while(current_token->type_of(TokenType::MULT) || current_token->type_of(TokenType::DIV)) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinOp(node, token, factor());
    }

    return node;
}

AST* Parser::factor() {
    Token* token = current_token;

    if(token->type_of(TokenType::INTEGER)) {
        eat(TokenType::INTEGER);
        return new Num(token);
        
    } else if(token->type_of(TokenType::LPAREN)) {
        eat(TokenType::LPAREN);
        AST* node = expr();
        eat(TokenType::RPAREN);
        return node;
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
    while(current_token->type_of(TokenType::PLUS) || current_token->type_of(TokenType::MINUS)) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinOp(node, token, term());
    }

    return node;
}

