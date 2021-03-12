#include <string>
#include <iostream>
#include "Interpreter.h"
#include "lexer/Lexer.h"

Interpreter::Interpreter(std::string code) {
    lexer = new Lexer(code);
}

int Interpreter::term() {
    Token* token = current_token;
    eat(TokenType::INTEGER);
    return std::stoi(token->value);
}

void Interpreter::eat(TokenType type) {
    if(current_token->type == type) {
        current_token = lexer->get_next_token();
    } else {
        lexer->error();
    }
}

int Interpreter::expr() {
    current_token = lexer->get_next_token();

    int result = term();
    while(current_token->type_of(TokenType::PLUS) || current_token->type_of(TokenType::MINUS)) {

        if(current_token->type_of(TokenType::PLUS)) {
            eat(TokenType::PLUS);
            result += term();
        } else if(current_token->type_of(TokenType::MINUS)) {
            eat(TokenType::MINUS);
            result -= term();
        }
    }

    return result;
}

