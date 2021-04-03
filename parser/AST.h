#ifndef AST_H
#define AST_H

#include "../lexer/Token.h"
#include <vector>
#include <map>
#include <cmath>

class AST {
    public:
        Token* token = NULL;

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
        bool inside_func;

        Compound(bool inside_func);
        ~Compound() override {};

};

class Variable : public AST {
    public:
        std::string value;

        Variable(Token* token);
        ~Variable() override {};
};

class Assign : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;

        Assign(AST* left, Token* op, AST* right);
        ~Assign() override {};
};

class VariableDeclaration : public AST {
    public:     
        std::vector<Variable*> variables;
        std::vector<Assign*> assignments;

        VariableDeclaration(std::vector<Variable*> variables);
        ~VariableDeclaration() override {};
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

class ArrayInit : public AST {
    public:
        std::vector<AST*> elements;

        ArrayInit(std::vector<AST*> elements);
        ~ArrayInit() override {};
};

class ArrayAccess : public AST {
    public:
        AST* array;
        AST* index;

        ArrayAccess(AST* array, AST* index);
        ~ArrayAccess() override {};
};

class FunctionInit : public AST {
    public:
        std::string func_name;
        VariableDeclaration* params;
        Compound* block;

        FunctionInit(std::string func_name, VariableDeclaration* params, Compound* block);
        ~FunctionInit() override {};
};

class FunctionCall : public AST {
    public:
        AST* function;
        std::vector<AST*> params;

        FunctionCall(AST* function, std::vector<AST*> params);
        ~FunctionCall() override {};
};

class Return : public AST {
    public:
        AST* returnable;

        Return(Token* token, AST* returnable);
        ~Return() override {};
};

class WhileLoop : public AST {
    public:
        AST* condition;
        Compound* statement;

        WhileLoop(AST* condition, Compound* compound);
        ~WhileLoop() override {};
};

class ClassInit : public AST {
    public:
        std::string class_name;
        Compound* block;

        ClassInit(std::string class_name, Compound* block);
        ~ClassInit() override {};
};

class CastValue : public AST {
    public:
        AST* value;
        Token* type;

        CastValue(AST* value, Token* type);
        ~CastValue() override {};
};

class Import : public AST {
    public:
        std::string path;
        std::string name;

        Import(Token* path, std::string name);
        ~Import() override {};
};

class ObjectDive : public AST {
    public:
        AST* parent;
        Variable* child;

        ObjectDive(AST* parent, Token* colon, Variable* child);
        ~ObjectDive() override {};
};

#endif