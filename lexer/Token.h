#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType { 
    INTEGER,
    PLUS,
    MINUS,
    END_OF_FILE 
};

class Token {
    public:
        TokenType type;
        std::string value;
        
        Token(TokenType type, std::string value);

        bool type_of(TokenType type);

};

#endif