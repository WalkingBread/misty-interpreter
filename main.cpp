#include <iostream>
#include <fstream>
#include <string>

#include "lexer/Token.h"
#include "Interpreter.h"

int main(int argc, char** argv) {
    std::string path = argv[1];

    std::string content = "";
    std::ifstream input_file(path);

    std::string line;
    while(std::getline(input_file, line)) {
        content += line;
    }

    Interpreter interpreter(content);

    std::cout << interpreter.expr();

    return 0;
}