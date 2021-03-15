#include <iostream>
#include "Interpreter.h"

Interpreter::Interpreter(std::string code) {
    lexer = new Lexer(code);
    parser = new Parser(lexer);
}

std::string Interpreter::evaluate() {
    AST* tree = parser->expr();
    return std::to_string(tree->visit());
}

