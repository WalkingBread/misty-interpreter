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
        int scope_level;
        SymbolTable* enclosing_scope;

        std::map<std::string, Symbol*> symbols;

        SymbolTable(int scope_level, SymbolTable* enclosing_scope);

        std::string str();

        void define(Symbol* symbol);
        Symbol* lookup(std::string name);
};

#endif