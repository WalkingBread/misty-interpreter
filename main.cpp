#include <iostream>
#include <string>
#include "interpreter/Interpreter.h"

int main(int argc, char** argv) {
    std::string path = argv[1];

    Interpreter* interpreter = new Interpreter();

    interpreter->evaluate(path);
    //std::cout << interpreter->memory_block->str();

    return 0;
}