#include "Lexer.h"

#include <iostream>

Lexer::Lexer(std::string code) {
    this->code = code;

    pos = 0;
    current_char = code[pos];
}

void Lexer::error() {
    std::cout << "Invalid syntax" << std::endl;
    exit(0);
}

void Lexer::advance() {
    pos++;
    if(pos > code.length() - 1) {
        current_char = NULL;
    } else {
        current_char = code[pos];
    }
}

void Lexer::skip_whitespace() {
    while(current_char != NULL && current_char == ' ') {
        advance();
    }
}

int Lexer::integer() {
    std::string result = "";

    while(current_char != NULL && isdigit(current_char)) {
        result += current_char;
        advance();
    }
    return std::stoi(result);
}

Token* Lexer::get_next_token() {

    while(current_char != NULL) {

        if(current_char == ' ') {
            skip_whitespace();
        }

        if(isdigit(current_char)) {
            std::string value = std::to_string(integer());

            return new Token(TokenType::INTEGER, value);
        }

        if(current_char == '+') {
            advance();
            return new Token(TokenType::PLUS, "+");
        }

        if(current_char == '-') {
            advance();
            return new Token(TokenType::MINUS, "-");
        }

        if(current_char == '*') {
            advance();
            return new Token(TokenType::MULT, "*");
        }

        if(current_char == '/') {
            advance();
            return new Token(TokenType::DIV, "/");
        }

        if(current_char == '(') {
            advance();
            return new Token(TokenType::LPAREN, "(");
        }

        if(current_char == ')') {
            advance();
            return new Token(TokenType::RPAREN, ")");
        }

        error();
    }

    return new Token(TokenType::END_OF_FILE, "");
}