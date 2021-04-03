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
    OBJECT,
    NONE
};

class MemoryValue {
    public:
        Type type;

        MemoryValue(Type type) {
            this->type = type;
        }

        virtual ~MemoryValue() = 0;
        
        virtual std::string str() = 0;
};

class SingularMemoryValue : public MemoryValue {
    public:
        std::string value;

        std::string str() override;

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

        std::string str() override;

        ~Array() override {}
};

class Function : public MemoryValue {
    public:
        FunctionInit* func;

        Function(FunctionInit* func)
        : MemoryValue(Type::FUNCTION) {
            this->func = func;
        }

        std::string str() override;

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

        MemoryValue* get(std::string name, bool only_this_block);
};

class Object : public MemoryValue {
    public:
        Memory* object_memory;

        Object(Memory* memory)
        : MemoryValue(Type::OBJECT) {
            this->object_memory = memory;
        }

        std::string str() override;

        ~Object() override {}
};

#endif