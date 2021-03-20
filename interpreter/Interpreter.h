#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <map>
#include "../lexer/Token.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "Memory.h"
#include "SymbolTableBuilder.h"
#include "../utils/Values.h"

class Interpreter {
    public:
        Interpreter(std::string code);
        
        void evaluate();

        Memory* global_memory;
    
    private:
        Lexer* lexer;
        Parser* parser;
        SymbolTableBuilder* symbol_table_builder;

        MemoryValue* visit(AST* node);
        MemoryValue* visit_binary_op(BinaryOperator* op);
        MemoryValue* visit_unary_op(UnaryOperator* op);
        MemoryValue* visit_value(Value* val);
        MemoryValue* visit_compare(Compare* c);
        MemoryValue* visit_compound(Compound* comp);
        MemoryValue* visit_assign(Assign* assign);
        MemoryValue* visit_variable(Variable* var);
        MemoryValue* visit_no_operator(NoOperator* no_op);
        MemoryValue* visit_double_condition(DoubleCondition* cond);
        MemoryValue* visit_negation(Negation* neg);
        MemoryValue* visit_var_declaration(VariableDeclaration* decl);
        MemoryValue* visit_if_condition(IfCondition* cond);
        MemoryValue* visit_print(Print* print);

        void type_mismatch_error();
};

#endif