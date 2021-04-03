#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <map>
#include "../lexer/Token.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "Memory.h"
#include "SemanticAnalyzer.h"
#include "../utils/Values.h"
#include "../utils/Error.h"

class Interpreter {
    public:
        Interpreter();
        
        MemoryValue* evaluate(std::string path);

        Memory* memory_block;
    
    private:
        SemanticAnalyzer* semantic_analyzer;

        MemoryValue* visit(AST* node);
        MemoryValue* visit_binary_op(BinaryOperator* op);
        MemoryValue* visit_compound(Compound* comp);
        MemoryValue* visit_assign(Assign* assign);
        MemoryValue* visit_variable(Variable* var);
        MemoryValue* visit_no_operator(NoOperator* no_op);
        MemoryValue* visit_var_declaration(VariableDeclaration* decl);
        MemoryValue* visit_if_condition(IfCondition* cond);
        MemoryValue* visit_print(Print* print);
        MemoryValue* visit_array_access(ArrayAccess* access);
        MemoryValue* visit_function_call(FunctionCall* func_call);
        MemoryValue* visit_return(Return* ret);
        MemoryValue* visit_while_loop(WhileLoop* while_loop);
        MemoryValue* visit_object_dive(ObjectDive* dive);
        
        Array* visit_array_init(ArrayInit* array_init);
        Function* visit_function_init(FunctionInit* func_init);
        Object* visit_import(Import* import);

        SingularMemoryValue* visit_unary_op(UnaryOperator* op);
        SingularMemoryValue* visit_value(Value* val);
        SingularMemoryValue* visit_compare(Compare* c);
        SingularMemoryValue* visit_double_condition(DoubleCondition* cond);
        SingularMemoryValue* visit_negation(Negation* neg);
        SingularMemoryValue* visit_cast_value(CastValue* cast);

        void enter_new_memory_block();
        void leave_memory_block();

        void type_mismatch_error(Token* token);
        void value_error(Token* token);
};

#endif