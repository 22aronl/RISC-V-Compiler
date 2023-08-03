#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"

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
    std::string name;
    Expression *value;
    std::string op;
public:
    Var_Assignment(std::string name, Expression *value, std::string op)
    {
        this->name = name;
        this->value = value;
        this->op = op;
    }

};

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

#endif // !STATEMENT_H