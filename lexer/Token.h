#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType { 
    FLOAT = 1,
    PLUS = 2,
    MINUS = 3,
    DIV = 4,
    INT_DIV = 5,
    MULT = 6,
    MODULO = 7,
    L_PAREN = 8,
    R_PAREN = 9,
    L_CURLY = 10,
    R_CURLY = 11,
    IDENTIFIER = 12,
    VARIABLE_DECL = 13,
    COMMA = 14,
    ASSIGN = 15,
    SEMICOLON = 16,
    END_OF_FILE = 17,
    STRING = 18,
    BOOLEAN = 19,
    COMPARE = 20
};

class Token {
    public:
        TokenType type;
        std::string value;
        
        Token(TokenType type, std::string value);

        bool type_of(TokenType type);
};

#endif