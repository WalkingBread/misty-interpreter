#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType { 
    FLOAT,
    PLUS,
    MINUS,
    DIV,
    INT_DIV,
    MULT,
    MODULO,
    L_PAREN,
    R_PAREN,
    L_CURLY,
    R_CURLY,
    IDENTIFIER,
    VARIABLE_DECL,
    COMMA,
    ASSIGN,
    SEMICOLON,
    END_OF_FILE,
    STRING,
    BOOLEAN,
    EQUALS,
    NOT_EQUALS,
    NOT,
    AND,
    OR,
    NONE,
    IF,
    ELSE,
    CAST,
    PRINT,
    FUNCTION,
    L_SQUARED,
    R_SQUARED,
    RETURN,
    WHILE,
    FOR,
    MORE,
    LESS,
    MORE_OR_EQ,
    LESS_OR_EQ
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