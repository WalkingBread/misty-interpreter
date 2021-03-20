#include "AST.h"
#include <iostream>

AST::~AST() {}

Value::Value(Token* token) {
    this->token = token;
    this->value = token->value;
}

BinaryOperator::BinaryOperator(AST* left, Token* op, AST* right) {
    this->token = op;
    this->op = op;
    this->left = left;
    this->right = right;
}

UnaryOperator::UnaryOperator(Token* op, AST* expr) {
    this->token = this->op = op;
    this->expr = expr;
}

Assign::Assign(Variable* left, Token* op, AST* right) {
    this->token = this->op = op;
    this->left = left;
    this->right = right;
}

Compare::Compare(std::vector<AST*> comparables, std::vector<Token*> operators) {
    this->comparables = comparables;
    this->operators = operators;
}

DoubleCondition::DoubleCondition(AST* left, Token* op, AST* right) {
    this->token = this->op = op;
    this->left = left;
    this->right = right;
}

Variable::Variable(Token* token) : AST() {
    this->token = token;
    this->value = token->value;
}

VariableDeclaration::VariableDeclaration(std::vector<Variable*> variables) {
    this->variables = variables;
}

Negation::Negation(Token* op, AST* statement) {
    this->token = this->op = op;
    this->statement = statement;
}

IfCondition::IfCondition(AST* condition, Compound* statement) {
    this->condition = condition;
    this->statement = statement;
}

Print::Print(AST* printable) {
    this->printable = printable;
}

