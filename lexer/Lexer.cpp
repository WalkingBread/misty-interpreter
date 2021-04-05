#include "Lexer.h"

#include <iostream>

Lexer::Lexer(std::string path) {
    this->path = path;

    std::ifstream input_file(path);

    std::string line;
    while(std::getline(input_file, line)) {
        code += line + '\n'; 
    }

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
    keywords["int"] = TokenType::CAST_INT;
    keywords["string"] = TokenType::CAST_STRING;
    keywords["float"] = TokenType::CAST_FLOAT;
    keywords["bool"] = TokenType::CAST_BOOL;
    keywords["func"] = TokenType::FUNCTION;
    keywords["return"] = TokenType::RETURN;
    keywords["for"] = TokenType::FOR;
    keywords["while"] = TokenType::WHILE;
    keywords["class"] = TokenType::CLASS;
    keywords["as"] = TokenType::AS;
    keywords["import"] = TokenType::IMPORT;

    keywords[";"] = TokenType::SEMICOLON;
    keywords[":"] = TokenType::COLON;
    keywords[","] = TokenType::COMMA;
    keywords["("] = TokenType::L_PAREN;
    keywords[")"] = TokenType::R_PAREN;
    keywords["["] = TokenType::L_SQUARED;
    keywords["]"] = TokenType::R_SQUARED;
    keywords["{"] = TokenType::L_CURLY;
    keywords["}"] = TokenType::R_CURLY;
    keywords["-"] = TokenType::MINUS;
    keywords["+"] = TokenType::PLUS;
    keywords["/"] = TokenType::DIV;
    keywords["//"] = TokenType::INT_DIV;
    keywords["*"] = TokenType::MULT;
    keywords["%"] = TokenType::MODULO;
    keywords["="] = TokenType::ASSIGN;
    keywords["=="] = TokenType::EQUALS;
    keywords["!="] = TokenType::NOT_EQUALS;
    keywords[">="] = TokenType::MORE_OR_EQ;
    keywords["<="] = TokenType::LESS_OR_EQ;
    keywords[">"] = TokenType::MORE;
    keywords["<"] = TokenType::LESS;
    keywords["&&"] = TokenType::AND;
    keywords["||"] = TokenType::OR;
    keywords["!"] = TokenType::NOT;
}

Token* Lexer::create_token(TokenType type, std::string value) {
    return new Token(type, value, line, column, path);
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
            SyntaxError(path, line, column, message).cast();
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

Token* Lexer::handle_build_in_lib() {
    std::string lib_name = "";
    advance();

    while(current_char != NULL && !isspace(current_char)) {
        lib_name += current_char;
        advance();
    }

    return create_token(TokenType::BUILT_IN_LIB, lib_name);
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

        if(current_char == '$') {
            return handle_build_in_lib();
        }

        std::string single {current_char};
        std::string peeked {current_char, peek()};

        if(keywords.find(peeked) != keywords.end()) {
            advance();
            advance();

            std::map<std::string, TokenType>::iterator it = keywords.find(peeked);
            return create_token(it->second, it->first);
        }

        if(keywords.find(single) != keywords.end()) {
            advance();

            std::map<std::string, TokenType>::iterator it = keywords.find(single);
            return create_token(it->second, it->first);
        }

        std::string message = "Unidentified token: " + current_char;
        SyntaxError(path, line, column, message).cast();
    }

    return create_token(TokenType::END_OF_FILE, "");
}