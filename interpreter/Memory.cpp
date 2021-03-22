#include "Memory.h"

MemoryValue::~MemoryValue() {}

std::string Memory::str() {
    std::string result = "Symbols: \n";

    std::map<std::string, MemoryValue*>::iterator it;

    for(it = values.begin(); it != values.end(); it++) {
        result += "Name: " + it->first + ", Value: ";

        if(SingularMemoryValue* sing = dynamic_cast<SingularMemoryValue*>(it->second)) {
            result += sing->value;
        } else if(Array* arr = dynamic_cast<Array*>(it->second)) {
             result += "array";
        }

        result += "\n";
    }

    return result;
}

MemoryValue* Memory::get(std::string name) {
    if(values.find(name) != values.end()) {
        return values.find(name)->second;
    }
    return NULL;
}

void Memory::put(std::string name, MemoryValue* val) {
    values[name] = val;
}

std::string Array::str() {}
