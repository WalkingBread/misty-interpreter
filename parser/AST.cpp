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

Assign::Assign(AST* left, Token* op, AST* right) {
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

Variable::Variable(Token* token) {
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

ArrayInit::ArrayInit(std::vector<AST*> elements) {
    this->elements = elements;
}

ArrayAccess::ArrayAccess(AST* array, AST* index) {
    this->array = array;
    this->index = index;
}

FunctionInit::FunctionInit(std::string func_name, VariableDeclaration* params, Compound* block) {
    this->func_name = func_name;
    this->params = params;
    this->block = block;
}

FunctionCall::FunctionCall(AST* function, std::vector<AST*> params) {
    this->function = function;
    this->params = params;
}

Return::Return(Token* token, AST* returnable) {
    this->token = token;
    this->returnable = returnable;
}

Compound::Compound(bool inside_func) {
    this->inside_func = inside_func;
}

WhileLoop::WhileLoop(AST* condition, Compound* statement) {
    this->condition = condition;
    this->statement = statement;
}