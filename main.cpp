#include <iostream>
#include <fstream>
#include <string>

#include "lexer/Token.h"
#include "interpreter/Interpreter.h"

int main(int argc, char** argv) {
    std::string path = argv[1];

    std::string content = "";
    std::ifstream input_file(path);

    std::string line;
    while(std::getline(input_file, line)) {
        content += line + '\n'; 
    }

    Interpreter interpreter(content);

    interpreter.evaluate();
    std::cout << interpreter.memory_block->str();

    return 0;
}