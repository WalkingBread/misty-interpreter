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

        Error(std::string error_type, std::string file_path, int line, int column, std::string message) {
            this->error_type = error_type;
            this->file_path = file_path;
            this->line = line;
            this->column = column;
            this->error_message = message;
        }

        Error(std::string file_path, int line, int column, std::string message) {
            this->error_type = "Error: ";
            this->file_path = file_path;
            this->line = line;
            this->column = column;
            this->error_message = message;
        }

        void cast() {
            std::cout << error_type << "In file: " << file_path << " line " << line << ", column " << column << ": " << error_message << std::endl;
            exit(0);
        }
};

class NameError : public Error {
    public:
        NameError(std::string file_path, int line, int column, std::string message) 
        : Error("NameError: ", file_path, line, column, message) {}
};

class SyntaxError : public Error {
    public:
        SyntaxError(std::string file_path, int line, int column, std::string message) 
        : Error("SyntaxError: ", file_path, line, column, message) {}
};

class ValueError : public Error {
    public:
        ValueError(std::string file_path, int line, int column, std::string message) 
        : Error("ValueError: ", file_path, line, column, message) {}
};

#endif