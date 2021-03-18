#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <map>

class Symbol {
    public:
        std::string name;

        Symbol(std::string name) {
            this->name = name;
        }

};

class SymbolTable {
    public:
        std::map<std::string, Symbol*> symbols;

        std::string str();

        void define(Symbol* symbol);
        Symbol* lookup(std::string name);
};

#endif