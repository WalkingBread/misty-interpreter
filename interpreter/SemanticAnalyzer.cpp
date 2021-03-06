#include "SemanticAnalyzer.h"

void SemanticAnalyzer::visit(AST* node) {
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

    } else if(IfCondition* ast = dynamic_cast<IfCondition*>(node)) {
        visit_if_condition(ast);

    } else if(Print* ast = dynamic_cast<Print*>(node)) {
        visit_print(ast);

    } else if(ArrayInit* ast = dynamic_cast<ArrayInit*>(node)) {
        visit_array_init(ast);

    } else if(ArrayAccess* ast = dynamic_cast<ArrayAccess*>(node)) {
        visit_array_access(ast);

    } else if(FunctionInit* ast = dynamic_cast<FunctionInit*>(node)) {
        visit_function_init(ast);

    } else if(FunctionCall* ast = dynamic_cast<FunctionCall*>(node)) {
        visit_function_call(ast);

    } else if(Return* ast = dynamic_cast<Return*>(node)) {
        visit_return(ast);

    } else if(WhileLoop* ast = dynamic_cast<WhileLoop*>(node)) {
        visit_while_loop(ast);
    
    } else if(CastValue* ast = dynamic_cast<CastValue*>(node)) {
        visit_cast_value(ast);
        
    } else if(Import* ast = dynamic_cast<Import*>(node)) {
        visit_import(ast);

    } else if(ObjectDive* ast = dynamic_cast<ObjectDive*>(node)) {
        visit_object_dive(ast);

    } else {
        std::string message = "Unknown AST branch.";
        int line = node->token->line;
        int column = node->token->column;
        std::string file_path = node->token->file;
        Error(file_path, line, column, message).cast();
    }
}

void SemanticAnalyzer::enter_new_scope() {
    int scope_level = current_scope->scope_level + 1;
    current_scope = new SymbolTable(scope_level, current_scope);
}

void SemanticAnalyzer::leave_scope() {
    current_scope = current_scope->enclosing_scope;
}

void SemanticAnalyzer::name_error(Token* token) {
    std::string message = "Variable " + token->value + " has not been declared.";
    int line = token->line;
    int column = token->column;
    std::string file_path = token->file;

    NameError(file_path, line, column, message).cast();
}

void SemanticAnalyzer::visit_binary_op(BinaryOperator* op) {
    visit(op->left);
    visit(op->right);
}

void SemanticAnalyzer::visit_unary_op(UnaryOperator* op) {
    visit(op->expr);
}

void SemanticAnalyzer::visit_value(Value* val) {}

void SemanticAnalyzer::visit_compare(Compare* c) {
    for(AST* node : c->comparables) {
        visit(node);
    }
}

void SemanticAnalyzer::visit_compound(Compound* comp) {
    if(current_scope == NULL) {
        current_scope = new SymbolTable(1, NULL);
    }

    for(AST* node : comp->children) {
        visit(node);
    }
}

void SemanticAnalyzer::visit_assign(Assign* assign) {
    AST* left = assign->left;
    if(Variable* var = dynamic_cast<Variable*>(left)) {
        std::string var_name = var->value;

        Symbol* var_symbol = current_scope->lookup(var_name, false);

        if(var_symbol == NULL) {
            name_error(var->token);
        }
    } else if(ArrayAccess* arr_acc = dynamic_cast<ArrayAccess*>(left)) {
        visit_array_access(arr_acc);
    }

    visit(assign->right);
}

void SemanticAnalyzer::visit_variable(Variable* var) {
    std::string var_name = var->value;
    Symbol* var_symbol = current_scope->lookup(var_name, false);

    if(var_symbol == NULL) {
        name_error(var->token);
    }
}

void SemanticAnalyzer::visit_no_operator(NoOperator* no_op) {}

void SemanticAnalyzer::visit_double_condition(DoubleCondition* cond) {
    visit(cond->left);
    visit(cond->right);
}

void SemanticAnalyzer::visit_negation(Negation* neg) {
    visit(neg->statement);
}

void SemanticAnalyzer::visit_var_declaration(VariableDeclaration* decl) {
    for(Variable* var : decl->variables) {
        std::string name = var->value;

        if(current_scope->lookup(name, true) != NULL) {
            std::string message = "Variable "  + name + " has already been declared.";
            int line = var->token->line;
            int column = var->token->column;
            std::string file_path;

            NameError(file_path, line, column, message).cast();
        }

        Symbol* symbol = new Symbol(name);

        current_scope->define(symbol);
    }
}

void SemanticAnalyzer::visit_if_condition(IfCondition* cond) {
    visit(cond->condition);

    enter_new_scope();
    visit(cond->statement);
    leave_scope();
}

void SemanticAnalyzer::visit_print(Print* print) {
    visit(print->printable);
}

void SemanticAnalyzer::visit_array_init(ArrayInit* array_init) {
    for(AST* node : array_init->elements) {
        visit(node);
    }
}

void SemanticAnalyzer::visit_array_access(ArrayAccess* access) {
    visit(access->array);
    visit(access->index);
}

void SemanticAnalyzer::visit_function_init(FunctionInit* func_init) {
    Symbol* func_symbol = new Symbol(func_init->func_name);
    current_scope->define(func_symbol);

    enter_new_scope();

    visit(func_init->params);
    visit(func_init->block);

    leave_scope();
}

void SemanticAnalyzer::visit_function_call(FunctionCall* func_call) {
    visit(func_call->function);
    for(AST* param : func_call->params) {
        visit(param);
    }
}

void SemanticAnalyzer::visit_return(Return* ret) {
    visit(ret->returnable);
}

void SemanticAnalyzer::visit_while_loop(WhileLoop* while_loop) {
    visit(while_loop->condition);

    enter_new_scope();
    visit(while_loop->statement);
    leave_scope();
}

void SemanticAnalyzer::visit_cast_value(CastValue* cast) {
    visit(cast->value);
}

void SemanticAnalyzer::visit_import(Import* import) {
    Symbol* import_name = new Symbol(import->name);
    current_scope->define(import_name);
}

void SemanticAnalyzer::visit_object_dive(ObjectDive* dive) {
    visit(dive->parent);
    visit(dive->child);
}