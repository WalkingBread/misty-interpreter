#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>
#include "../lexer/Token.h"

class Error {
    public:
        std::string error_type;
        std::string file_path;
        std::string error_message;

        int line, column;

        Error(std::string error_type, int line, int column, std::string message) {
            this->error_type = error_type;
            this->line = line;
            this->column = column;
            this->error_message = message;
        }

        Error(int line, int column, std::string message) {
            this->error_type = "Error: ";
            this->line = line;
            this->column = column;
            this->error_message = message;
        }

        void cast() {
            std::cout << error_type << "line " << line << ", column " << column << ": " << error_message << std::endl;
            exit(0);
        }
};

class NameError : public Error {
    public:
        NameError(int line, int column, std::string message) 
        : Error("NameError: ", line, column, message) {}
};

class SyntaxError : public Error {
    public:
        SyntaxError(int line, int column, std::string message) 
        : Error("SyntaxError: ", line, column, message) {}
};

class ValueError : public Error {
    public:
        ValueError(int line, int column, std::string message) 
        : Error("ValueError: ", line, column, message) {}
};

#endif