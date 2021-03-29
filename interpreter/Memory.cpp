#include "Memory.h"

MemoryValue::~MemoryValue() {}

Memory::Memory(int memory_level, Memory* enclosing_memory_block) {
    this->memory_level = memory_level;
    this->enclosing_memory_block = enclosing_memory_block;
}

std::string Memory::str() {
    std::string result = "Symbols: \n";

    std::map<std::string, MemoryValue*>::iterator it;

    for(it = values.begin(); it != values.end(); it++) {
        result += "Name: " + it->first + ", Value: ";

        if(SingularMemoryValue* sing = dynamic_cast<SingularMemoryValue*>(it->second)) {
            result += sing->value;
        } else if(Array* arr = dynamic_cast<Array*>(it->second)) {
            result += "array";
        } else if(Function* func = dynamic_cast<Function*>(it->second)) {
            result += "function";
        }

        result += "\n";
    }

    return result;
}

MemoryValue* Memory::get(std::string name, bool only_this_block) {
    if(values.find(name) != values.end()) {
        return values.find(name)->second;
    }

    if(enclosing_memory_block != NULL && !only_this_block) {
        return enclosing_memory_block->get(name, false);
    }

    return NULL;
}

void Memory::put(std::string name, MemoryValue* val) {
    Memory* scope = this;

    while(scope->get(name, true) == NULL) {
        if(scope->memory_level != 1) {
            scope = scope->enclosing_memory_block;
        } else {
            break;
        }
    }

    scope->values[name] = val;
}

std::string SingularMemoryValue::str() {
    return value;
}

std::string Function::str() {
    return "function " + func->func_name;
}

std::string Array::str() {
    std::string result = "[";
    for(int i = 0; i < elements.size(); i++) {
        MemoryValue* val = elements.at(i);
        result += val->str();

        if(i != elements.size() - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}
