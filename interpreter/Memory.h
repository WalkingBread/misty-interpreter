#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "../parser/AST.h"

enum class Type {
    FLOAT,
    STRING,
    BOOLEAN,
    ARRAY,
    FUNCTION,
    NONE
};

class MemoryValue {
    public:
        Type type;

        MemoryValue(Type type) {
            this->type = type;
        }

        virtual ~MemoryValue() = 0;
};

class SingularMemoryValue : public MemoryValue {
    public:
        std::string value;

        SingularMemoryValue(std::string value, Type type)
        : MemoryValue(type) {
            this->value = value;
        }

        ~SingularMemoryValue() override {}
};

class Array : public MemoryValue {
    public:
        std::vector<MemoryValue*> elements;

        Array(std::vector<MemoryValue*> elements)
        : MemoryValue(Type::ARRAY) {
            this->elements = elements;
        }

        Array()
        : MemoryValue(Type::ARRAY) {}

        std::string str();

        ~Array() override {}
};

class Function : public MemoryValue {
    public:
        FunctionInit* func;

        Function(FunctionInit* func)
        : MemoryValue(Type::FUNCTION) {
            this->func = func;
        }

        ~Function() override {}
};

class Memory {
    public:
        std::map<std::string, MemoryValue*> values;
        Memory* enclosing_memory_block;
        int memory_level;

        Memory(int memory_level, Memory* enclosing_memory_block);

        std::string str();

        void put(std::string name, MemoryValue* val);

        MemoryValue* get(std::string name);
};

#endif