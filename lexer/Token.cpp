#include <string>
#include "Token.h"

Token::Token(TokenType type, std::string value) {
    this->type = type;
    this->value = value;
}

bool Token::type_of(TokenType type) {
    if(this->type == type) {
        return true;
    }
    return false;
}