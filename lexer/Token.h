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
    COLON,
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
    CAST_INT,
    CAST_STRING,
    CAST_FLOAT,
    CAST_BOOL,
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
    LESS_OR_EQ,
    CLASS, 
    AS, 
    IMPORT,
    BUILT_IN_LIB
};

class Token {
    public:
        TokenType type;
        std::string value;

        int line;
        int column;

        std::string file;

        Token(TokenType type, std::string value, int line, int column, std::string file);
        Token(TokenType type, std::string value);

        bool type_of(TokenType type);
};

#endif