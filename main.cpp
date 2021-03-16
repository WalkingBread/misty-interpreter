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
        content += line;
    }

    Interpreter interpreter(content);

    interpreter.evaluate();
    std::cout << interpreter.global_scope.find("x")->second << std::endl;
    std::cout << interpreter.global_scope.find("z")->second << std::endl;

    return 0;
}