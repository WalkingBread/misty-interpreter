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
    EQUALS = 20,
    NOT_EQUALS = 21,
    NOT = 22,
    AND = 23,
    OR = 24,
    NONE = 25,
    IF = 26,
    ELSE = 27,
    CAST = 28,
    PRINT = 29,
    FUNCTION = 30,
    L_SQUARED = 31,
    R_SQUARED = 32,
    RETURN = 33,
    WHILE = 34,
    FOR = 35,
    MORE = 36,
    LESS = 37,
    MORE_OR_EQ = 38,
    LESS_OR_EQ = 39
};

class Token {
    public:
        TokenType type;
        std::string value;

        int line;
        int column;
        
        Token(TokenType type, std::string value, int line, int column);
        Token(TokenType type, std::string value);

        bool type_of(TokenType type);
};

#endif