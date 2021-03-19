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
    PRINT = 29
};


/*class TokenType {
    public:
        std::string str;

        TokenType(std::string str) {
            this->str = str;
        }

};

const TokenType 
        FLOAT("FLOAT"), 
        PLUS("+"),
        MINUS("-"),
        DIV("/"),
        INT_DIV("//"),
        MULT("*"),
        MODULO("%"),
        L_PAREN("("),
        R_PAREN(")"),
        L_CURLY("{"),
        R_CURLY("}"),
        L_SQUARED("["),
        R_SQUARED("]"),
        IDENTIFIER("ID"),
        VARIABLE_DECL("VAR_DECL"),
        COMMA(","),
        ASSIGN("="),
        SEMICOLON(";"),
        STRING("STRING"),
        BOOLEAN("BOOL"),
        EQUALS("=="),
        NOT_EQUALS("!="),
        NOT("!"),
        AND("&&"),
        OR("||");*/


class Token {
    public:
        TokenType type;
        std::string value;
        
        Token(TokenType type, std::string value);

        bool type_of(TokenType type);
};

#endif