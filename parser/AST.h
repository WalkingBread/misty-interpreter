#ifndef AST_H
#define AST_H

#include "../lexer/Token.h"
#include <vector>
#include <map>
#include <cmath>

class AST {
    public:
        Token* token;
        virtual std::string visit(std::map<std::string, std::string>* scope) = 0;
};

class Value : public AST {
    public:
        std::string value;

        Value(Token* token);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class BinaryOperator : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        BinaryOperator(AST* left, Token* op, AST* right);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class UnaryOperator : public AST {
    public:
        Token* op;
        AST* expr;

        UnaryOperator(Token* op, AST* expr);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class Compound : public AST {
    public:
        std::vector<AST*> children;

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class Variable : public AST {
    public:
        std::string value;

        Variable(Token* token);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class VariableDeclaration : public AST {
    public:     
        std::vector<Variable*> variables;

        VariableDeclaration(std::vector<Variable*> variables);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class Assign : public AST {
    public:
        Variable* left;
        Token* op;
        AST* right;

        Assign(Variable* left, Token* op, AST* right);

        std::string visit(std::map<std::string, std::string>* scope) override;
};


class NoOperator : public AST {
    public:
        std::string visit(std::map<std::string, std::string>* scope) override;
};

class Compare : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        std::vector<AST*> comparables;
        std::vector<Token*> operators;

        Compare(std::vector<AST*> comparables, std::vector<Token*> operators);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class Negation : public AST {
    public:
        AST* statement;
        Token* op;

        Negation(Token* op, AST* statement);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

class DoubleCondition : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        DoubleCondition(AST* left, Token* op, AST* right);

        std::string visit(std::map<std::string, std::string>* scope) override;
};

#endif