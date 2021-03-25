#include <iostream>
#include "Interpreter.h"

void Interpreter::type_mismatch_error(Token* token) {
    std::string message = "Type mismatch.";
    SyntaxError(token->line, token->column, message).cast();
}

Interpreter::Interpreter(std::string code) {
    lexer = new Lexer(code);
    parser = new Parser(lexer);
    memory_block = new Memory(0, NULL);
    semantic_analyzer = new SemanticAnalyzer();
}

void Interpreter::enter_new_memory_block() {
    memory_block = new Memory(memory_block->memory_level + 1, memory_block);
}

void Interpreter::leave_memory_block() {
    if(memory_block->enclosing_memory_block->memory_level != 0) {
        memory_block = memory_block->enclosing_memory_block;
    }
}

MemoryValue* Interpreter::visit(AST* node) {
    if(BinaryOperator* ast = dynamic_cast<BinaryOperator*>(node)) {
        return visit_binary_op(ast);

    } else if(UnaryOperator* ast = dynamic_cast<UnaryOperator*>(node)) {
        return visit_unary_op(ast);

    } else if(Value* ast = dynamic_cast<Value*>(node)) {
        return visit_value(ast);

    } else if(Compare* ast = dynamic_cast<Compare*>(node)) {
        return visit_compare(ast);

    } else if(Compound* ast = dynamic_cast<Compound*>(node)) {
        return visit_compound(ast);

    } else if(Assign* ast = dynamic_cast<Assign*>(node)) {
        return visit_assign(ast);

    } else if(Variable* ast = dynamic_cast<Variable*>(node)) {
        return visit_variable(ast);

    } else if(NoOperator* ast = dynamic_cast<NoOperator*>(node)) {
        return visit_no_operator(ast);
        
    } else if(DoubleCondition* ast = dynamic_cast<DoubleCondition*>(node)) {
        return visit_double_condition(ast);
        
    } else if(Negation* ast = dynamic_cast<Negation*>(node)) {
        return visit_negation(ast);
        
    } else if(VariableDeclaration* ast = dynamic_cast<VariableDeclaration*>(node)) {
        return visit_var_declaration(ast);

    } else if(IfCondition* ast = dynamic_cast<IfCondition*>(node)) {
        return visit_if_condition(ast);

    } else if(Print* ast = dynamic_cast<Print*>(node)) {
        return visit_print(ast);

    } else if(ArrayInit* ast = dynamic_cast<ArrayInit*>(node)) {
        return visit_array_init(ast);

    } else if(ArrayAccess* ast = dynamic_cast<ArrayAccess*>(node)) {
        return visit_array_access(ast);

    } else if(FunctionInit* ast = dynamic_cast<FunctionInit*>(node)) {
        return visit_function_init(ast);

    } else if(FunctionCall* ast = dynamic_cast<FunctionCall*>(node)) {
        return visit_function_call(ast);
    } 
} 

MemoryValue* Interpreter::visit_binary_op(BinaryOperator* op) {
    SingularMemoryValue* left = (SingularMemoryValue*) visit(op->left);
    SingularMemoryValue* right = (SingularMemoryValue*) visit(op->right);

    switch(op->op->type) {
        case TokenType::PLUS:
        {
            if(left->type == Type::STRING) {
                if(right->type != Type::STRING) {
                    type_mismatch_error(op->right->token);
                }

                std::string a = left->value;
                std::string b = right->value;

                return new SingularMemoryValue(a + b, Type::STRING);


            } else if(left->type == Type::FLOAT) {
                if(right->type != Type::FLOAT) {
                    type_mismatch_error(op->right->token);
                }

                double x = std::stod(left->value);
                double y = std::stod(right->value);

                return new SingularMemoryValue(std::to_string(x + y), Type::FLOAT);
            }
            type_mismatch_error(op->left->token);
        }
        case TokenType::MINUS:
        {   
            if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
                type_mismatch_error(op->right->token);
            }
            double x = std::stod(left->value);
            double y = std::stod(right->value);

            return new SingularMemoryValue(std::to_string(x - y), Type::FLOAT);
        }
        case TokenType::DIV:
        {
            if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
                type_mismatch_error(op->right->token);
            }
            double x = std::stod(left->value);
            double y = std::stod(right->value);

            return new SingularMemoryValue(std::to_string(x / y), Type::FLOAT);
        }
        case TokenType::MULT:
        {
            if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
                type_mismatch_error(op->right->token);
            }
            double x = std::stod(left->value);
            double y = std::stod(right->value);

            return new SingularMemoryValue(std::to_string(x * y), Type::FLOAT);
        }
        case TokenType::INT_DIV:
        {
            if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
                type_mismatch_error(op->right->token);
            }

            int x = std::stoi(left->value);
            int y = std::stoi(right->value);

            return new SingularMemoryValue(std::to_string(x / y), Type::FLOAT);
        }
        case TokenType::MODULO:
        {
            if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
                type_mismatch_error(op->right->token);
            }

            double x = std::stod(left->value);
            double y = std::stod(right->value);
            return new SingularMemoryValue(std::to_string(fmod(x, y)), Type::FLOAT);
        }
    }
}

SingularMemoryValue* Interpreter::visit_unary_op(UnaryOperator* op) {
    SingularMemoryValue* expr = (SingularMemoryValue*) visit(op->expr);

    if(op->op->type_of(TokenType::MINUS)) {
        if(expr->type == Type::FLOAT) {

            double value = std::stod(expr->value);
            return new SingularMemoryValue(std::to_string(-value), Type::FLOAT);

        } else {
            type_mismatch_error(op->expr->token);
        }
    } 
    return expr;
}

SingularMemoryValue* Interpreter::visit_value(Value* val) {
    Type type;

    if(val->token->type_of(TokenType::FLOAT)) {
        type = Type::FLOAT;

    } else if(val->token->type_of(TokenType::BOOLEAN)) {
        type = Type::BOOLEAN;

    } else if(val->token->type_of(TokenType::STRING)) {
        type = Type::STRING;

    } else if(val->token->type_of(TokenType::NONE)) {
        type = Type::NONE;
    }

    return new SingularMemoryValue(val->value, type);
}

SingularMemoryValue* Interpreter::visit_compare(Compare* c) {
    for(int i = 0; i < c->operators.size(); i++) {
        Token* op = c->operators.at(i);
        AST* left = c->comparables[i];
        AST* right = c->comparables[i + 1];

        SingularMemoryValue* left_memory_value = (SingularMemoryValue*) visit(left);
        SingularMemoryValue* right_memory_value = (SingularMemoryValue*) visit(right);

        std::string left_value = left_memory_value->value;
        std::string right_value = right_memory_value->value;

        if(op->type_of(TokenType::EQUALS)) {
            if(left_memory_value->type == Type::FLOAT && right_memory_value->type == Type::FLOAT) {
                double left_val = std::stod(left_value);
                double right_val = std::stod(right_value);

                if(left_val != right_val) {
                    return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
                }
            } else if(left_value != right_value) {
                return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
            }
        } else if(op->type_of(TokenType::NOT_EQUALS)) {
            if(left_memory_value->type == Type::FLOAT && right_memory_value->type == Type::FLOAT) {
                double left_val = std::stod(left_value);
                double right_val = std::stod(right_value);

                if(left_val == right_val) {
                    return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
                }
            } else if(left_value == right_value) {
                return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
            }
        }
    }
    return new SingularMemoryValue(Values::TRUE, Type::BOOLEAN);
}

MemoryValue* Interpreter::visit_compound(Compound* comp) {
    if(memory_block->memory_level == 0) {
        enter_new_memory_block();
    }

    for(AST* node : comp->children) {
        visit(node);
    }
    leave_memory_block();
    return NULL;
}

MemoryValue* Interpreter::visit_assign(Assign* assign) {
    std::string var_name = assign->left->value;
    memory_block->put(var_name, visit(assign->right));

    return NULL;
}

MemoryValue* Interpreter::visit_variable(Variable* var) {
    MemoryValue* val = memory_block->get(var->value);

    if(val != NULL) {
        return val;
    } else {
        std::string message = "Variable has not been initialized.";
        int line = var->token->line;
        int column = var->token->column;
        NameError(line, column, message).cast();
    }
}

MemoryValue* Interpreter::visit_no_operator(NoOperator* no_op) {
    return NULL;
}

SingularMemoryValue* Interpreter::visit_double_condition(DoubleCondition* cond) {
    std::string left_value = ((SingularMemoryValue*) visit(cond->left))->value;
    std::string right_value = ((SingularMemoryValue*) visit(cond->right))->value;

    if(cond->token->type_of(TokenType::AND)) {
        if(left_value == Values::TRUE && right_value == Values::TRUE) {
            return new SingularMemoryValue(Values::TRUE, Type::BOOLEAN);
        } else {
            return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
        }

    } else if(cond->token->type_of(TokenType::OR)) {
        if(left_value == Values::TRUE || right_value == Values::TRUE) {
            return new SingularMemoryValue(Values::TRUE, Type::BOOLEAN);
        } else {
            return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
        }
    }
}

SingularMemoryValue* Interpreter::visit_negation(Negation* neg) {
    SingularMemoryValue* value = (SingularMemoryValue*) visit(neg->statement);

    if(value->type != Type::BOOLEAN) {
        type_mismatch_error(neg->statement->token);
    }

    if(value->value == Values::TRUE) {
        return new SingularMemoryValue(Values::FALSE, Type::BOOLEAN);
    } else if(value->value == Values::FALSE) {
        return new SingularMemoryValue(Values::TRUE, Type::BOOLEAN);
    }
}

MemoryValue* Interpreter::visit_var_declaration(VariableDeclaration* decl) {
    for(Assign* assignment : decl->assignments) {
        visit(assignment);
    }

    return NULL;
}

MemoryValue* Interpreter::visit_if_condition(IfCondition* cond) {
    AST* condition = cond->condition;
    Compound* statement = cond->statement;

    std::string cond_value = ((SingularMemoryValue*) visit(condition))->value;


    if(cond_value == Values::TRUE) {
        enter_new_memory_block();
        visit(statement);
    } else {
        for(IfCondition* else_ : cond->elses) {
            std::string else_cond_value = ((SingularMemoryValue*) visit(else_->condition))->value;

            if(else_cond_value == Values::TRUE) {
                enter_new_memory_block();
                visit(else_->statement);
                return NULL;
            }
        }
    }
    

    return NULL;
}

MemoryValue* Interpreter::visit_print(Print* print) {
    if(SingularMemoryValue* printable = (SingularMemoryValue*) visit(print->printable)) {
        std::cout << printable->value << std::endl;
    } else {
        // print array
    }
    
    return NULL;
}

Array* Interpreter::visit_array_init(ArrayInit* array_init) {
    std::vector<MemoryValue*> elements;

    for(AST* el : array_init->elements) {
        MemoryValue* element = visit(el);
        elements.push_back(element);
    }

    return new Array(elements);
}

MemoryValue* Interpreter::visit_array_access(ArrayAccess* access) {
    MemoryValue* arr = visit(access->array);

    if(arr->type != Type::ARRAY) {
        std::string message = "Given object is not an array.";
        int line = access->array->token->line;
        int column = access->array->token->column;

        SyntaxError(line, column, message).cast();
    }

    Array* array = (Array*) arr;
    
    MemoryValue* index = visit(access->index);

    if(index->type != Type::FLOAT) {
        type_mismatch_error(access->index->token);
    }

    SingularMemoryValue* _index = (SingularMemoryValue*) index;
    int i = std::stoi(_index->value);

    if(i > array->elements.size() - 1) {
        std::string message = "Index out of bounds.";
        int line = access->index->token->line;
        int column = access->index->token->column;

        SyntaxError(line, column, message).cast();
    }

    return array->elements.at(i);
}

Function* Interpreter::visit_function_init(FunctionInit* func_init) {
    memory_block->put(func_init->func_name, new Function(func_init));
    return NULL;
}

MemoryValue* Interpreter::visit_function_call(FunctionCall* func_call) {
    MemoryValue* func = visit(func_call->function);

    if(func->type != Type::FUNCTION) {
        std::string message = "Given object is not a function.";
        int line = func_call->function->token->line;
        int column = func_call->function->token->column;

        SyntaxError(line, column, message).cast();
    }

    Function* function = (Function*) func;
    enter_new_memory_block();

    VariableDeclaration* func_params = function->func->params;

    visit(func_params);

    if(func_params->variables.size() != func_call->params.size()) {
        std::string message = "Inconsistent number of arguments.";
        int line = func_call->function->token->line;
        int column = func_call->function->token->column;

        SyntaxError(line, column, message).cast();
    }

    for(int i = 0; i < func_params->variables.size(); i++) {
        Variable* param = func_params->variables.at(i);
        AST* actual_param = func_call->params.at(i);
        
        Assign* assign = new Assign(param, new Token(TokenType::ASSIGN, "="), actual_param);
        visit(assign);
    }

    visit(function->func->block);
    leave_memory_block();

    return NULL;
}

void Interpreter::evaluate() {
    AST* tree = parser->parse();
    semantic_analyzer->visit(tree);
    visit(tree);
}

