#include "Token.h"
#include <iostream>

Token::Token(TokenType type, std::string value, int line, int column, std::string file) {
    this->type = type;
    this->value = value;
    this->line = line;
    this->column = column;
    this->file = file;
}

Token::Token(TokenType type, std::string value) {
    this->type = type;
    this->value = value;
    this->line = 0;
    this->column = 0;
    this->file = "";
}

bool Token::type_of(TokenType type) {
    if(this->type == type) {
        return true;
    }
    return false;
}