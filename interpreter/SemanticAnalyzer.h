#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Symbol.h"
#include "../parser/AST.h"
#include "../utils/Error.h"
#include <iostream>

class SemanticAnalyzer {
    public:

        SymbolTable* current_scope;

        SemanticAnalyzer() {
            current_scope = NULL;
        }

        void visit(AST* node);

    private:
        void enter_new_scope();
        void leave_scope();

        void visit_binary_op(BinaryOperator* op);
        void visit_unary_op(UnaryOperator* op);
        void visit_value(Value* val);
        void visit_compare(Compare* c);
        void visit_compound(Compound* comp);
        void visit_assign(Assign* assign);
        void visit_variable(Variable* var);
        void visit_no_operator(NoOperator* no_op);
        void visit_double_condition(DoubleCondition* cond);
        void visit_negation(Negation* neg);
        void visit_var_declaration(VariableDeclaration* decl);
        void visit_if_condition(IfCondition* cond);
        void visit_print(Print* print);
        void visit_array_init(ArrayInit* array_init);
        void visit_array_access(ArrayAccess* access);
        void visit_function_init(FunctionInit* func_init);
        void visit_function_call(FunctionCall* func_call);
        void visit_return(Return* ret);
        void visit_while_loop(WhileLoop* while_loop);

        void name_error(Token* token);
};

#endif