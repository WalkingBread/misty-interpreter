#include "Symbol.h"

SymbolTable::SymbolTable(int scope_level, SymbolTable* enclosing_scope) {
    this->scope_level = scope_level;
    this->enclosing_scope = enclosing_scope;
}

void SymbolTable::define(Symbol* symbol) {
    symbols[symbol->name] = symbol;
}

Symbol* SymbolTable::lookup(std::string name) {
    if(symbols.find(name) != symbols.end()) {
        return symbols.find(name)->second;
    }

    if(enclosing_scope != NULL) {
        return enclosing_scope->lookup(name);
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