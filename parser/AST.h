#ifndef AST_H
#define AST_H

#include "../lexer/Token.h"
#include <vector>
#include <map>
#include <cmath>

class AST {
    public:
        Token* token;

        virtual ~AST() = 0;
};

class Value : public AST {
    public:
        std::string value;

        Value(Token* token);
        ~Value() override {};
};

class BinaryOperator : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        BinaryOperator(AST* left, Token* op, AST* right);
        ~BinaryOperator() override {};

};

class UnaryOperator : public AST {
    public:
        Token* op;
        AST* expr;

        UnaryOperator(Token* op, AST* expr);
        ~UnaryOperator() override {};

};

class Compound : public AST {
    public:
        std::vector<AST*> children;
        ~Compound() override {};

};

class Variable : public AST {
    public:
        std::string value;

        Variable(Token* token);
        ~Variable() override {};
};

class VariableDeclaration : public AST {
    public:     
        std::vector<Variable*> variables;

        VariableDeclaration(std::vector<Variable*> variables);
        ~VariableDeclaration() override {};
};

class Assign : public AST {
    public:
        Variable* left;
        Token* op;
        AST* right;

        Assign(Variable* left, Token* op, AST* right);
        ~Assign() override {};
};


class NoOperator : public AST {};

class Compare : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        std::vector<AST*> comparables;
        std::vector<Token*> operators;

        Compare(std::vector<AST*> comparables, std::vector<Token*> operators);
        ~Compare() override {};
};

class Negation : public AST {
    public:
        AST* statement;
        Token* op;

        Negation(Token* op, AST* statement);
        ~Negation() override {};
};

class DoubleCondition : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        DoubleCondition(AST* left, Token* op, AST* right);
        ~DoubleCondition() override {};
};

class IfCondition : public AST {
    public:
        AST* condition;
        Compound* statement;

        std::vector<IfCondition*> elses;

        IfCondition(AST* condition, Compound* statement);
        ~IfCondition() override {};
};

class Print : public AST {
    public:
        AST* printable;

        Print(AST* printable);
        ~Print() override {};
};

#endif