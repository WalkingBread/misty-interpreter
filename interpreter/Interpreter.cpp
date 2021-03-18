#include <iostream>
#include "Interpreter.h"

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
    } 
}

MemoryValue* Interpreter::visit_binary_op(BinaryOperator* op) {
    double x = std::stod(visit(op->left)->value);
    double y = std::stod(visit(op->right)->value);

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
        double value = std::stod(visit(op->expr)->value);
        return new MemoryValue(std::to_string(-value), Type::FLOAT);
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
                return new MemoryValue("False", Type::BOOLEAN);
            }
        } else if(op->type_of(TokenType::NOT_EQUALS)) {
            if(visit(left)->value == visit(right)->value) {
                return new MemoryValue("False", Type::BOOLEAN);
            }
        }
    }
    return new MemoryValue("True", Type::BOOLEAN);
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
        if(visit(cond->left)->value == "True" && visit(cond->right)->value == "True") {
            return new MemoryValue("True", Type::BOOLEAN);
        } else {
            return new MemoryValue("False", Type::BOOLEAN);
        }
    } else if(cond->token->type_of(TokenType::OR)) {
        if(visit(cond->left)->value == "True" || visit(cond->right)->value == "True") {
            return new MemoryValue("True", Type::BOOLEAN);
        } else {
            return new MemoryValue("False", Type::BOOLEAN);
        }
    }
}

MemoryValue* Interpreter::visit_negation(Negation* neg) {
    MemoryValue* value = visit(neg->statement);

    if(value->value == "True") {
        return new MemoryValue("False", Type::BOOLEAN);
    } else if(value->value == "False") {
        return new MemoryValue("True", Type::BOOLEAN);
    }
}

MemoryValue* Interpreter::visit_var_declaration(VariableDeclaration* decl) {
    return NULL;
}

void Interpreter::evaluate() {
    AST* tree = parser->parse();
    symbol_table_builder->visit(tree);
    visit(tree);
}

