#include "Lexer.h"

#include <iostream>

Lexer::Lexer(std::string code) {
    this->code = code;

    pos = 0;
    current_char = code[pos];

    create_keywords();
}

void Lexer::create_keywords() {
    keywords["have"] = TokenType::VARIABLE_DECL;
    keywords["True"] = TokenType::BOOLEAN; 
    keywords["False"] = TokenType::BOOLEAN;
    keywords["not"] = TokenType::NOT;
    keywords["is"] = TokenType::EQUALS;
    keywords["isnt"] = TokenType::NOT_EQUALS;
    keywords["and"] = TokenType::AND;
    keywords["or"] = TokenType::OR;
    keywords["None"] = TokenType::NONE;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["print"] = TokenType::PRINT;
    keywords["int"] = TokenType::CAST;
    keywords["str"] = TokenType::CAST;
    keywords["float"] = TokenType::CAST;
    keywords["func"] = TokenType::FUNCTION;
}

void Lexer::error(std::string message) {
    std::cout << "Lexer: " << message << std::endl;
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

char Lexer::peek() {
    int peek_pos = pos + 1;
    if(peek_pos > code.length() - 1) {
        return NULL;
    }
    return code[peek_pos];
}

void Lexer::skip_whitespace() {
    while(current_char != NULL && current_char == ' ') {
        advance();
    }
}

Token* Lexer::number() {
    std::string result = "";

    while(current_char != NULL && isdigit(current_char)) {
        result += current_char;
        advance();
    }

    if(current_char == '.') {
        result += current_char;
        advance();

        while(current_char != NULL && isdigit(current_char)) {
            result += current_char;
            advance();
        }
    }
    
    return new Token(TokenType::FLOAT, result);
}

Token* Lexer::string() {
    std::string result = "";

    while(current_char != NULL && current_char != '\'') {
        
    }
}

Token* Lexer::handle_identifiers() {
    std::string result = "";

    while(current_char != NULL && isalnum(current_char)) {
        result += current_char;
        advance();
    }

    if(keywords.find(result) != keywords.end()) {
        std::map<std::__cxx11::string, TokenType>::iterator it = keywords.find(result);

        return new Token(it->second, it->first);
    }
    return new Token(TokenType::IDENTIFIER, result);
}


Token* Lexer::get_next_token() {

    while(current_char != NULL) {

        if(current_char == ' ') {
            skip_whitespace();
        }

        if(isdigit(current_char)) {
            return number();
        }

        if(isalpha(current_char)) {
            return handle_identifiers();
        }

        if(current_char == ',') {
            advance();
            return new Token(TokenType::COMMA, ",");
        }

        if(current_char == '&' && peek() == '&') {
            advance();
            advance();
            return new Token(TokenType::AND, "&&");
        }

        if(current_char == '|' && peek() == '|') {
            advance();
            advance();
            return new Token(TokenType::OR, "||");
        }

        if(current_char == '!' && peek() == '=') {
            advance();
            advance();
            return new Token(TokenType::NOT_EQUALS, "==");
        }

        if(current_char == '!') {
            advance();
            return new Token(TokenType::NOT, "!");
        }

        if(current_char == '=' && peek() == '=') {
            advance();
            advance();
            return new Token(TokenType::EQUALS, "==");
        }

        if(current_char == '=') {
            advance();
            return new Token(TokenType::ASSIGN, "=");
        }

        if(current_char == ';') {
            advance();
            return new Token(TokenType::SEMICOLON, ";");
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

        if(current_char == '/' && peek() == '/') {
            advance();
            advance();
            return new Token(TokenType::INT_DIV, "//");
        }

        if(current_char == '/') {
            advance();
            return new Token(TokenType::DIV, "/");
        }

        if(current_char == '%') {
            advance();
            return new Token(TokenType::MODULO, "%");
        }

        if(current_char == '(') {
            advance();
            return new Token(TokenType::L_PAREN, "(");
        }

        if(current_char == ')') {
            advance();
            return new Token(TokenType::R_PAREN, ")");
        }

        if(current_char == '{') {
            advance();
            return new Token(TokenType::L_CURLY, "{");
        }

        if(current_char == '}') {
            advance();
            return new Token(TokenType::R_CURLY, "}");
        }

        std::cout << current_char << std::endl;
        error("Unidentified token.");
    }

    return new Token(TokenType::END_OF_FILE, "");
}