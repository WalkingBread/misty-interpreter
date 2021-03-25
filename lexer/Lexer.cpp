#include "Lexer.h"

#include <iostream>

Lexer::Lexer(std::string code) {
    this->code = code;

    pos = 0;
    current_char = code[pos];

    line = 1;
    column = 1;

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
    keywords["return"] = TokenType::RETURN;
}

Token* Lexer::create_token(TokenType type, std::string value) {
    return new Token(type, value, line, column);
}

void Lexer::advance() {
    if(current_char == '\n') {
        line++;
        column = 0;
    }

    pos++;
    if(pos > code.length() - 1) {
        current_char = NULL;
    } else {
        current_char = code[pos];
        column++;
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
    while(current_char != NULL && isspace(current_char)) {
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
    
    return create_token(TokenType::FLOAT, result);
}

Token* Lexer::string() {
    std::string result = "";
    advance();

    while(current_char != '\'') {
        if(current_char == '\n') {
            std::string message = "Reached end of line while parsing string.";
            SyntaxError(line, column, message).cast();
        }
        result += current_char;
        advance();
    }
    advance();

    return create_token(TokenType::STRING, result);
}

Token* Lexer::handle_identifiers() {
    std::string result = "";

    while(current_char != NULL && (isalnum(current_char) || current_char == '_')) {
        result += current_char;
        advance();
    }

    if(keywords.find(result) != keywords.end()) {
        std::map<std::__cxx11::string, TokenType>::iterator it = keywords.find(result);

        return create_token(it->second, it->first);
    }
    return create_token(TokenType::IDENTIFIER, result);
}


Token* Lexer::get_next_token() {

    while(current_char != NULL) {

        if(isspace(current_char)) {
            skip_whitespace();

            if(current_char == NULL) {
                return create_token(TokenType::END_OF_FILE, "");
            }
        }

        if(isdigit(current_char)) {
            return number();
        }

        if(isalpha(current_char) || current_char == '_') {
            return handle_identifiers();
        }

        if(current_char == '\'') {
            return string();
        }

        if(current_char == ',') {
            advance();
            return create_token(TokenType::COMMA, ",");
        }

        if(current_char == '&' && peek() == '&') {
            advance();
            advance();
            return create_token(TokenType::AND, "&&");
        }

        if(current_char == '|' && peek() == '|') {
            advance();
            advance();
            return create_token(TokenType::OR, "||");
        }

        if(current_char == '!' && peek() == '=') {
            advance();
            advance();
            return create_token(TokenType::NOT_EQUALS, "==");
        }

        if(current_char == '!') {
            advance();
            return create_token(TokenType::NOT, "!");
        }

        if(current_char == '=' && peek() == '=') {
            advance();
            advance();
            return create_token(TokenType::EQUALS, "==");
        }

        if(current_char == '=') {
            advance();
            return create_token(TokenType::ASSIGN, "=");
        }

        if(current_char == ';') {
            advance();
            return create_token(TokenType::SEMICOLON, ";");
        }

        if(current_char == '+') {
            advance();
            return create_token(TokenType::PLUS, "+");
        }

        if(current_char == '-') {
            advance();
            return create_token(TokenType::MINUS, "-");
        }

        if(current_char == '*') {
            advance();
            return create_token(TokenType::MULT, "*");
        }

        if(current_char == '/' && peek() == '/') {
            advance();
            advance();
            return create_token(TokenType::INT_DIV, "//");
        }

        if(current_char == '/') {
            advance();
            return create_token(TokenType::DIV, "/");
        }

        if(current_char == '%') {
            advance();
            return create_token(TokenType::MODULO, "%");
        }

        if(current_char == '(') {
            advance();
            return create_token(TokenType::L_PAREN, "(");
        }

        if(current_char == ')') {
            advance();
            return create_token(TokenType::R_PAREN, ")");
        }

        if(current_char == '{') {
            advance();
            return create_token(TokenType::L_CURLY, "{");
        }

        if(current_char == '}') {
            advance();
            return create_token(TokenType::R_CURLY, "}");
        }

        if(current_char == '[') {
            advance();
            return create_token(TokenType::L_SQUARED, "[");
        } 

        if(current_char == ']') {
            advance();
            return create_token(TokenType::R_SQUARED, "]");
        }

        std::string message = "Unidentified token: " + current_char;
        SyntaxError(line, column, message).cast();
    }

    return create_token(TokenType::END_OF_FILE, "");
}