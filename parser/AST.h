#ifndef AST_H
#define AST_H

#include "../lexer/Token.h"

class AST {
    public:
        virtual int visit() = 0;
};

class Num : public AST {
    public:
        int value;
        Token* token;

        Num(Token* token);

        int visit();
};

class BinOp : public AST {
    public:
        AST* left;
        Token* op;
        AST* right;
        Token* token;

        BinOp(AST* left, Token* op, AST* right);

        int visit();
};

#endif