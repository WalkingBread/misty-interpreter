#include "Symbol.h"

void SymbolTable::define(Symbol* symbol) {
    symbols[symbol->name] = symbol;
}

Symbol* SymbolTable::lookup(std::string name) {
    if(symbols.find(name) != symbols.end()) {
        return symbols.find(name)->second;
    }
    return NULL;
}

std::string SymbolTable::str() {
    std::string result = "Symbols: \n";

    std::map<std::string, Symbol*>::iterator it;

    for(it = symbols.begin(); it != symbols.end(); it++) {
        result += it->second->name;
        result += "\n";
    }

    return result;
}