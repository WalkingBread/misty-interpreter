#include <iostream>
#include "Interpreter.h"

void Interpreter::type_mismatch_error() {
    std::cout << "Type mismatch error" << std::endl;
    exit(0);
}

Interpreter::Interpreter(std::string code) {
    lexer = new Lexer(code);
    parser = new Parser(lexer);
    global_memory = new Memory();
    symbol_table_builder = new SymbolTableBuilder();
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
    } 
}

MemoryValue* Interpreter::visit_binary_op(BinaryOperator* op) {
    MemoryValue* left = visit(op->left);
    MemoryValue* right = visit(op->right);

    if(left->type != Type::FLOAT || right->type != Type::FLOAT) {
        type_mismatch_error();
    }

    double x = std::stod(left->value);
    double y = std::stod(right->value);

    switch(op->op->type) {
        case TokenType::PLUS:
        {
            return new MemoryValue(std::to_string(x + y), Type::FLOAT);
        }
        case TokenType::MINUS:
        {
            return new MemoryValue(std::to_string(x - y), Type::FLOAT);
        }
        case TokenType::DIV:
        {
            return new MemoryValue(std::to_string(x / y), Type::FLOAT);
        }
        case TokenType::MULT:
        {
            return new MemoryValue(std::to_string(x * y), Type::FLOAT);
        }
        case TokenType::INT_DIV:
        {
            int a = (int) x;
            int b = (int) y;

            return new MemoryValue(std::to_string(a / b), Type::FLOAT);
        }
        case TokenType::MODULO:
        {
            return new MemoryValue(std::to_string(fmod(x, y)), Type::FLOAT);
        }
    }
}

MemoryValue* Interpreter::visit_unary_op(UnaryOperator* op) {
    if(op->op->type_of(TokenType::MINUS)) {
        if(visit(op->expr)->type == Type::FLOAT) {
            double value = std::stod(visit(op->expr)->value);
            return new MemoryValue(std::to_string(-value), Type::FLOAT);

        } else {
            type_mismatch_error();
        }
    } 
    return visit(op->expr);
}

MemoryValue* Interpreter::visit_value(Value* val) {
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

    return new MemoryValue(val->value, type);
}

MemoryValue* Interpreter::visit_compare(Compare* c) {
    for(int i = 0; i < c->operators.size(); i++) {
        Token* op = c->operators.at(i);
        AST* left = c->comparables[i];
        AST* right = c->comparables[i + 1];

        if(op->type_of(TokenType::EQUALS)) {
            if(visit(left)->value != visit(right)->value) {
                return new MemoryValue(Values::FALSE, Type::BOOLEAN);
            }
        } else if(op->type_of(TokenType::NOT_EQUALS)) {
            if(visit(left)->value == visit(right)->value) {
                return new MemoryValue(Values::FALSE, Type::BOOLEAN);
            }
        }
    }
    return new MemoryValue(Values::TRUE, Type::BOOLEAN);
}

MemoryValue* Interpreter::visit_compound(Compound* comp) {
    for(AST* node : comp->children) {
        visit(node);
    }
    return NULL;
}

MemoryValue* Interpreter::visit_assign(Assign* assign) {
    std::string var_name = assign->left->value;
    global_memory->put(var_name, visit(assign->right));

    return NULL;
}

MemoryValue* Interpreter::visit_variable(Variable* var) {
    MemoryValue* val = global_memory->get(var->value);

    if(val != NULL) {
        return val;
    } else {
        std::cout << "Name error: " << var->value << std::endl;
        exit(0);
    }
}

MemoryValue* Interpreter::visit_no_operator(NoOperator* no_op) {
    return NULL;
}

MemoryValue* Interpreter::visit_double_condition(DoubleCondition* cond) {
    if(cond->token->type_of(TokenType::AND)) {
        if(visit(cond->left)->value == Values::TRUE && visit(cond->right)->value == Values::TRUE) {
            return new MemoryValue(Values::TRUE, Type::BOOLEAN);
        } else {
            return new MemoryValue(Values::FALSE, Type::BOOLEAN);
        }

    } else if(cond->token->type_of(TokenType::OR)) {
        if(visit(cond->left)->value == Values::TRUE || visit(cond->right)->value == Values::TRUE) {
            return new MemoryValue(Values::TRUE, Type::BOOLEAN);
        } else {
            return new MemoryValue(Values::FALSE, Type::BOOLEAN);
        }
    }
}

MemoryValue* Interpreter::visit_negation(Negation* neg) {
    MemoryValue* value = visit(neg->statement);

    if(value->type != Type::BOOLEAN) {
        type_mismatch_error();
    }

    if(value->value == Values::TRUE) {
        return new MemoryValue(Values::FALSE, Type::BOOLEAN);
    } else if(value->value == Values::FALSE) {
        return new MemoryValue(Values::TRUE, Type::BOOLEAN);
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

    if(visit(condition)->value == Values::TRUE) {
        visit(statement);
    } else {
        for(IfCondition* else_ : cond->elses) {
            if(visit(else_->condition)->value == Values::TRUE) {
                visit(else_->statement);
                return NULL;
            }
        }
    }
    return NULL;
}

MemoryValue* Interpreter::visit_print(Print* print) {
    MemoryValue* printable = visit(print->printable);
    std::cout << printable->value << std::endl;
    return NULL;
}

void Interpreter::evaluate() {
    AST* tree = parser->parse();
    symbol_table_builder->visit(tree);
    visit(tree);
}

