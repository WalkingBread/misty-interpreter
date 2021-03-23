#include <iostream>
#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    current_token = lexer->get_next_token();
}

void Parser::error(Token* token) {
    std::string message = "Unexpected token: " + token->value;
    int line = token->line;
    int column = token->column;
    SyntaxError(line, column, message).cast();
}

Compound* Parser::compound_statement() {
    eat(TokenType::L_CURLY);
    std::vector<AST*> nodes = statement_list();

    eat(TokenType::R_CURLY);

    Compound* root = new Compound();
    root->children = nodes;

    return root;
}

std::vector<AST*> Parser::statement_list() {
    AST* node = statement();

    std::vector<AST*> nodes = {node};

    while(current_token->type_of(TokenType::SEMICOLON)) {
        eat(current_token->type);
        nodes.push_back(statement());
    } 

    return nodes;
}

Variable* Parser::variable() {
    Variable* node = new Variable(current_token);
    eat(TokenType::IDENTIFIER);

    return node;
}

VariableDeclaration* Parser::variable_declaration() {
    eat(TokenType::VARIABLE_DECL);
    Variable* var = new Variable(current_token);
    eat(TokenType::IDENTIFIER);

    std::vector<Variable*> variables = { var };

    while(current_token->type_of(TokenType::COMMA)) {
        eat(TokenType::COMMA);
        Variable* var = new Variable(current_token);
        eat(TokenType::IDENTIFIER);
        variables.push_back(var);
    }

    VariableDeclaration* var_decl = new VariableDeclaration(variables);

    if(current_token->type_of(TokenType::ASSIGN)) {
        eat(TokenType::ASSIGN);

        Variable* left = variables.at(0);
        AST* right = expr();
        Assign* assignment = new Assign(left, new Token(TokenType::ASSIGN, "="), right);

        var_decl->assignments.push_back(assignment);

        int i = 1;
        while(current_token->type_of(TokenType::COMMA)) {
            eat(TokenType::COMMA);
            if(i > variables.size()) {
                error(current_token);
            }

            Variable* left = variables.at(i);
            AST* right = expr();
            Assign* assignment = new Assign(left, new Token(TokenType::ASSIGN, "="), right);

            var_decl->assignments.push_back(assignment);
            i++;
        }
    }
    return var_decl;
}

NoOperator* Parser::empty() {
    return new NoOperator();
}

Assign* Parser::assignment_statement() {
    Variable* left = variable();
    Token* token = current_token;
    eat(TokenType::ASSIGN);
    AST* right = expr();

    return new Assign(left, token, right);
}

IfCondition* Parser::if_statement() {
    eat(TokenType::IF);
    eat(TokenType::L_PAREN);
    AST* condition = expr();
    eat(TokenType::R_PAREN);

    Compound* statement = compound_statement();
    return new IfCondition(condition, statement);
}

IfCondition* Parser::else_statement() {
    eat(TokenType::ELSE);
    if(current_token->type_of(TokenType::IF)) {
        return if_statement();

    } else if(current_token->type_of(TokenType::L_CURLY)) {
        AST* condition = new Value(new Token(TokenType::BOOLEAN, Values::TRUE));

        return new IfCondition(condition, compound_statement());
    }
}

Print* Parser::print_statement() {
    eat(TokenType::PRINT);
    eat(TokenType::L_PAREN);
    AST* printable = expr();
    eat(TokenType::R_PAREN);

    return new Print(printable);
}

AST* Parser::statement() {
    AST* node;

    switch(current_token->type) {
        case TokenType::VARIABLE_DECL:
            node = variable_declaration();
            break;

        case TokenType::IDENTIFIER:
            node = assignment_statement();
            break;

        case TokenType::IF:
        {
            IfCondition* cond = if_statement();
            while(current_token->type_of(TokenType::ELSE)) {
                cond->elses.push_back(else_statement());
            }

            node = cond;
            break;
        }

        case TokenType::PRINT:
            node = print_statement();
            break;

        default:
            node = empty();
    }

    return node;
}

AST* Parser::term() {
    AST* node = factor();

    if(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
        std::vector<AST*> comparables = { node };
        std::vector<Token*> operators;

        while(current_token->type_of(TokenType::EQUALS) || current_token->type_of(TokenType::NOT_EQUALS)) {
            Token* op = current_token;
            eat(current_token->type);

            comparables.push_back(factor());
            operators.push_back(op);
        }
        return new Compare(comparables, operators);
    }

    while(current_token->type_of(TokenType::MULT) || 
          current_token->type_of(TokenType::DIV) || 
          current_token->type_of(TokenType::INT_DIV) ||
          current_token->type_of(TokenType::MODULO)
    ) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinaryOperator(node, token, factor());
    }

    return node;
}

ArrayAccess* Parser::array_access(AST* array) {
    eat(TokenType::L_SQUARED);
    AST* index = expr();
    eat(TokenType::R_SQUARED);

    ArrayAccess* access = new ArrayAccess(array, index);

    while(current_token->type_of(TokenType::L_SQUARED)) {
        eat(TokenType::L_SQUARED);
        AST* index = expr();
        eat(TokenType::R_SQUARED);
        access = new ArrayAccess(access, index);
    }

    return access;
}

ArrayInit* Parser::array_init() {
    eat(TokenType::L_SQUARED);

    std::vector<AST*> elements = {};

    if(!current_token->type_of(TokenType::R_SQUARED)) {
        elements.push_back(expr());

        while(current_token->type_of(TokenType::COMMA)) {
            eat(TokenType::COMMA);
            elements.push_back(expr());
        }
    }

    eat(TokenType::R_SQUARED);
    return new ArrayInit(elements);
}

AST* Parser::factor() {
    Token* token = current_token;

    switch(token->type) {
        case TokenType::PLUS:
        {
            eat(TokenType::PLUS);
            AST* expr = factor();
            return new UnaryOperator(token, expr);
        }
        
        case TokenType::MINUS:
        {
            eat(TokenType::MINUS);
            return new UnaryOperator(token, factor());
        }

        case TokenType::FLOAT:
        {
            eat(TokenType::FLOAT);
            return new Value(token);
        }

        case TokenType::NOT:
        {
            eat(TokenType::NOT);
            return new Negation(token, factor());
        }

        case TokenType::STRING:
        {
            eat(TokenType::STRING);
            return new Value(token);
        }

        case TokenType::BOOLEAN:
        {
            eat(TokenType::BOOLEAN);
            return new Value(token);
        }
        case TokenType::NONE:
        {
            eat(TokenType::NONE);
            return new Value(token);
        }
        case TokenType::L_PAREN:
        {
            eat(TokenType::L_PAREN);
            AST* node = expr();
            eat(TokenType::R_PAREN);
            return node;
        }
        case TokenType::L_SQUARED:
        {
            AST* node = array_init();
            if(current_token->type_of(TokenType::L_SQUARED)) {
                node = array_access(node);
            }
            return node;
        }
        
        default:
            AST* node = variable();
            if(current_token->type_of(TokenType::L_SQUARED)) {
                node = array_access(node);
            }
            return node;
    }
    
}

void Parser::eat(TokenType type) {
    if(current_token->type == type) {
        current_token = lexer->get_next_token();
    } else {
        int current_type = static_cast<int>(current_token->type);
        int expected_type = static_cast<int>(type);

        error(current_token);
    }
}

AST* Parser::expr() {
    AST* node = term();

    while(current_token->type_of(TokenType::AND) || current_token->type_of(TokenType::OR)) {
        Token* op = current_token;
        eat(current_token->type);

        node = new DoubleCondition(node, op, expr());
    }

    while(current_token->type_of(TokenType::PLUS) || current_token->type_of(TokenType::MINUS)) {
        Token* token = current_token;
        eat(current_token->type);
        node = new BinaryOperator(node, token, term());
    }

    return node;
}

AST* Parser::parse() {
    Compound* program = new Compound();
    program->children = statement_list();

    if(!current_token->type_of(TokenType::END_OF_FILE)) {
        std::cout << static_cast<int>(current_token->type) << std::endl;
        error(current_token);
    }

    return program;
}

