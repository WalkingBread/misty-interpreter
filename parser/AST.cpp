#include "AST.h"
#include <iostream>

Value::Value(Token* token) {
    this->token = token;
    this->value = token->value;
}

std::string Value::visit(std::map<std::string, std::string>* scope) {
    return value;
}

BinaryOperator::BinaryOperator(AST* left, Token* op, AST* right) {
    this->token = op;
    this->op = op;
    this->left = left;
    this->right = right;
}

std::string BinaryOperator::visit(std::map<std::string, std::string>* scope) {
    double x = std::stod(left->visit(scope));
    double y = std::stod(right->visit(scope));

    switch(op->type) {
        case TokenType::PLUS:
        {
            return std::to_string(x + y);
        }
        case TokenType::MINUS:
        {
            return std::to_string(x - y);
        }
        case TokenType::DIV:
        {
            return std::to_string(x / y);
        }
        case TokenType::MULT:
        {
            return std::to_string(x * y);
        }
        case TokenType::INT_DIV:
        {
            int a = (int) x;
            int b = (int) y;

            return std::to_string(a / b);
        }
        case TokenType::MODULO:
        {
            return std::to_string(fmod(x, y));
        }
    }
}

UnaryOperator::UnaryOperator(Token* op, AST* expr) {
    this->token = this->op = op;
    this->expr = expr;
}

std::string UnaryOperator::visit(std::map<std::string, std::string>* scope) {
    if(op->type_of(TokenType::MINUS)) {
        double value = std::stod(expr->visit(scope));
        return std::to_string(-value);
    } 
    return expr->visit(scope);
}

std::string Compound::visit(std::map<std::string, std::string>* scope) {
    for(AST* node : children) {
        node->visit(scope);
    }
    return "";
}

Assign::Assign(Variable* left, Token* op, AST* right) {
    this->token = this->op = op;
    this->left = left;
    this->right = right;
}

std::string Assign::visit(std::map<std::string, std::string>* scope) {
    std::string var_name = left->value;
    std::map<std::string, std::string> &t_scope = *scope;
    t_scope[var_name] = right->visit(scope);
    return "";
}

Compare::Compare(AST* left, Token* op, AST* right) {
    this->token = this->op = op;
    this->left = left;
    this->right = right;
}

std::string Compare::visit(std::map<std::string, std::string>* scope) {
    if(token->type_of(TokenType::EQUALS)) {
        if(left->visit(scope) == right->visit(scope)) {
            return "True";
        } else {
            return "False";
        }
    } else if(token->type_of(TokenType::NOT_EQUALS)) {
        if(left->visit(scope) != right->visit(scope)) {
            return "True";
        } else {
            return "False";
        }
    }
    
}

DoubleCondition::DoubleCondition(AST* left, Token* op, AST* right) {
    this->token = this->op = op;
    this->left = left;
    this->right = right;
}

std::string DoubleCondition::visit(std::map<std::string, std::string>* scope) {
    if(token->type_of(TokenType::AND)) {
        if(left->visit(scope) == "True" && right->visit(scope) == "True") {
            return "True";
        } else {
            return "False";
        }
    } else if(token->type_of(TokenType::OR)) {
        if(left->visit(scope) == "True" || right->visit(scope) == "True") {
            return "True";
        } else {
            return "False";
        }
    }
    
}

Variable::Variable(Token* token) {
    this->token = token;
    this->value = token->value;
}

std::string Variable::visit(std::map<std::string, std::string>* scope) {
    if(scope->find(value) != scope->end()) {
        return scope->find(value)->second;
    } else {
        std::cout << "Name error" << std::endl;
        exit(0);
    }
}

VariableDeclaration::VariableDeclaration(std::vector<Variable*> variables) {
    this->variables = variables;
}

std::string VariableDeclaration::visit(std::map<std::string, std::string>* scope) {
    return "";
}

std::string NoOperator::visit(std::map<std::string, std::string>* scope) {
    return "";
}

Negation::Negation(Token* op, AST* statement) {
    this->token = this->op = op;
    this->statement = statement;
}

std::string Negation::visit(std::map<std::string, std::string>* scope) {
    std::string value = statement->visit(scope);

    if(value == "True") {
        return "False";
    } else if(value == "False") {
        return "True";
    }
}

