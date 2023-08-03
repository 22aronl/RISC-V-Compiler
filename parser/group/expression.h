#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <string>
#include <vector>

class Expression
{
};

class Number : public Expression
{
    int value;

public:
    Number(int value)
    {
        this->value = value;
    }
};

class Variable : public Expression
{
    std::string name;

public:
    Variable(std::string name)
    {
        this->name = name;
    }
};

class Function_Call : public Expression
{
    std::string name;
    std::vector<Expression *> args;

public:
    Function_Call(std::string name, std::vector<Expression *> args)
    {
        this->name = name;
        this->args = args;
    }
};

class Array_Access : public Expression
{
    std::string name;
    Expression *index;

public:
    Array_Access(std::string name, Expression *index)
    {
        this->name = name;
        this->index = index;
    }
};

class Struct_Access : public Expression
{
    std::string name;
    std::string member;

public:
    Struct_Access(std::string name, std::string member)
    {
        this->name = name;
        this->member = member;
    }
};

class Binary_Operation : public Expression
{
    Expression *left;
    Expression *right;
    std::string op;

public:
    Binary_Operation(Expression *left, Expression *right, std::string op)
    {
        this->left = left;
        this->right = right;
        this->op = op;
    }
};

class Negation : public Expression
{
    Expression *value;

public:
    Negation(Expression *value)
    {
        this->value = value;
    }
};



#endif // !_EXPRESSION_H_