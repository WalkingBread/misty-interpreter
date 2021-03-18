#include "SymbolTableBuilder.h"

void SymbolTableBuilder::visit(AST* node) {
    if(BinaryOperator* ast = dynamic_cast<BinaryOperator*>(node)) {
        visit_binary_op(ast);

    } else if(UnaryOperator* ast = dynamic_cast<UnaryOperator*>(node)) {
        visit_unary_op(ast);

    } else if(Value* ast = dynamic_cast<Value*>(node)) {
         visit_value(ast);

    } else if(Compare* ast = dynamic_cast<Compare*>(node)) {
        visit_compare(ast);

    } else if(Compound* ast = dynamic_cast<Compound*>(node)) {
        visit_compound(ast);

    } else if(Assign* ast = dynamic_cast<Assign*>(node)) {
        visit_assign(ast);

    } else if(Variable* ast = dynamic_cast<Variable*>(node)) {
        visit_variable(ast);

    } else if(NoOperator* ast = dynamic_cast<NoOperator*>(node)) {
        visit_no_operator(ast);
        
    } else if(DoubleCondition* ast = dynamic_cast<DoubleCondition*>(node)) {
        visit_double_condition(ast);
        
    } else if(Negation* ast = dynamic_cast<Negation*>(node)) {
        visit_negation(ast);
        
    } else if(VariableDeclaration* ast = dynamic_cast<VariableDeclaration*>(node)) {
        visit_var_declaration(ast);
        
    } 
}

void SymbolTableBuilder::name_error(std::string name) {
    std::cout << "NameError: Symbol " << name << " has not been declared." << std::endl;  
    exit(0);
}

void SymbolTableBuilder::visit_binary_op(BinaryOperator* op) {
    visit(op->left);
    visit(op->right);
}

void SymbolTableBuilder::visit_unary_op(UnaryOperator* op) {
    visit(op->expr);
}

void SymbolTableBuilder::visit_value(Value* val) {}

void SymbolTableBuilder::visit_compare(Compare* c) {
    for(AST* node : c->comparables) {
        visit(node);
    }
}

void SymbolTableBuilder::visit_compound(Compound* comp) {
    for(AST* node : comp->children) {
        visit(node);
    }
}

void SymbolTableBuilder::visit_assign(Assign* assign) {
    Variable* var = assign->left;
    std::string var_name = var->value;

    Symbol* var_symbol =  table->lookup(var_name);
    if(var->token)

    if(var_symbol == NULL) {
        name_error(var_name);
    }

    visit(assign->right);
}

void SymbolTableBuilder::visit_variable(Variable* var) {
    std::string var_name = var->value;
    Symbol* var_symbol = table->lookup(var_name);

    if(var_symbol == NULL) {
        name_error(var_name);
    }
}

void SymbolTableBuilder::visit_no_operator(NoOperator* no_op) {}

void SymbolTableBuilder::visit_double_condition(DoubleCondition* cond) {
    visit(cond->left);
    visit(cond->right);
}

void SymbolTableBuilder::visit_negation(Negation* neg) {
    visit(neg->statement);
}

void SymbolTableBuilder::visit_var_declaration(VariableDeclaration* decl) {
    for(Variable* var : decl->variables) {
        std::string name = var->value;
        Symbol* symbol = new Symbol(name);

        table->define(symbol);
    }
}
