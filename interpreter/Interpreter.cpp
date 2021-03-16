#include <iostream>
#include "Interpreter.h"

Interpreter::Interpreter(std::string code) {
    lexer = new Lexer(code);
    parser = new Parser(lexer);
}

std::string Interpreter::evaluate() {
    AST* tree = parser->parse();
    return tree->visit(&global_scope);
}

