#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <map>
#include <iostream>

enum class Type {
    FLOAT,
    STRING,
    BOOLEAN,
    NONE
};

class MemoryValue {
    public:
        Type type;
        std::string value;

        MemoryValue(std::string value, Type type) {
            this->value = value;
            this->type = type;
        }
};

class Memory {
    public:
        std::map<std::string, MemoryValue*> values;

        std::string str() {
            std::string result = "Symbols: \n";

            std::map<std::string, MemoryValue*>::iterator it;

            for(it = values.begin(); it != values.end(); it++) {
                result += "Name: " + it->first + ", Value: " + it->second->value;
                result += "\n";
            }

            return result;
        }

        void put(std::string name, MemoryValue* val) {
            values[name] = val;
        }

        MemoryValue* get(std::string name) {
            if(values.find(name) != values.end()) {
                return values.find(name)->second;
            }
            return NULL;
        }
};

#endif