#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>
#include "../lexer/Token.h"

class Error {
    public:
        std::string error_type;
        Token* token;
        std::string file_path;
        std::string error_message;

        Error(std::string error_type, std::string file_path, Token* token, std::string message) {
            this->error_type = error_type;
            this->file_path = file_path;
            this->token = token;
            this->error_message = message;
        }

        void cast() {
            std::cout << error_message << std::endl;
            exit(0);
        }
};

class NameError : public Error {
    public:

};

class SyntaxError : public Error {
    public:

};

#endif