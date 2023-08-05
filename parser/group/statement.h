#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include <vector>

class Statement
{
};

class Var_Declaration : public Statement
{
    std::string type;
    std::string name;
public:
    Var_Declaration(std::string type, std::string name)
    {
        this->type = type;
        this->name = name;
    }
};

class Var_Assignment : public Statement
{
    Expression* leftOp;
    Expression *value;
    std::string op;
public:
    Var_Assignment(Expression* leftOp, Expression *value, std::string op)
    {
        this->leftOp = leftOp;
        this->value = value;
        this->op = op;
    }

};

//declares array
class Array_Declaration : public Statement
{
    std::string type;
    std::string name;
    Expression *size;
public:
    Array_Declaration(std::string type, std::string name, Expression* size) {
        this->type = type;
        this->name = name;
        this->size = size;
    }
};

class For_Loop : public Statement
{
    Statement *init;
    Expression *condition;
    Statement *increment;
    //body is a vector of statements
    std::vector<Statement*> body;

public:
    For_Loop(Statement *init, Expression *condition, Statement *increment, std::vector<Statement*> body)
    {
        this->init = init;
        this->condition = condition;
        this->increment = increment;
        this->body = body;
    }
};

class If_Statement : public Statement
{
    Expression *condition;
    std::vector<Statement*> body;
    std::vector<Statement*> else_body;
    bool has_else = false;
public:
    If_Statement(Expression *condition, std::vector<Statement*> body)
    {
        this->condition = condition;
        this->body = body;
    }

    If_Statement(Expression *condition, std::vector<Statement*> body, std::vector<Statement*> else_body)
    {
        this->condition = condition;
        this->body = body;
        this->else_body = else_body;
        this->has_else = true;
    }
};

class Return_Statement : public Statement
{
    Expression *value;
public:
    Return_Statement(Expression *value)
    {
        this->value = value;
    }
};

#endif // !STATEMENT_H 