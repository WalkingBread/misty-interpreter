#ifndef SYMBOL_TABLE_BUILDER_H
#define SYMBOL_TABLE_BUILDER_H

#include "Symbol.h"
#include "../parser/AST.h"
#include <iostream>

class SymbolTableBuilder {
    public:

        SymbolTable* table;

        SymbolTableBuilder() {
            table = new SymbolTable();
        }

        void visit(AST* node);

    private:
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

        void name_error(std::string name);
};

#endif