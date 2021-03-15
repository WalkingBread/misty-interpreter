#include "AST.h"
#include <iostream>

Num::Num(Token* token) {
    this->token = token;
    this->value = std::stoi(token->value);
}

int Num::visit() {
    return value;
}

BinOp::BinOp(AST* left, Token* op, AST* right) {
    this->token = op;
    this->op = op;
    this->left = left;
    this->right = right;
}

int BinOp::visit() {
    if(op->type_of(TokenType::PLUS)) {
        return left->visit() + right->visit();
    } else if(op->type_of(TokenType::MINUS)) {
        return left->visit() - right->visit();
    } else if(op->type_of(TokenType::DIV)) {
        return left->visit() / right->visit();
    } else if(op->type_of(TokenType::MULT)) {
        return left->visit() * right->visit();
    }
}