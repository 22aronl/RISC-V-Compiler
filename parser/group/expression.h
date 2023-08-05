#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <string>
#include <vector>

class Expression
{
protected:
    void print_offset(int i) {
        for (int j = 0; j < i; j++)
        {
            std::cout << "  ";
        }
    }

public:
    virtual void print(int i) = 0;

    void print() {
        print(0);
    }
};

class Number : public Expression
{
    int value;

public:
    Number(int value)
    {
        this->value = value;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Number " << value << std::endl;
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

    void print(int i)
    {
        print_offset(i);
        std::cout << "Variable " << name << std::endl;
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

    void print(int i)
    {
        print_offset(i);
        std::cout << "Function_Call " << name << std::endl;
        for (auto arg : args)
        {
            arg->print(i + 1);
        }
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

    void print(int i)
    {
        print_offset(i);
        std::cout << "Array_Access " << name << std::endl;
        index->print(i + 1);
    }
};

class Struct_Access : public Expression
{
    std::string name;
    Struct_Access *member;
    bool continues = false;

public:
    Struct_Access(std::string name, Struct_Access *member)
    {
        this->name = name;
        this->member = member;
        this->continues = true;
    }

    Struct_Access(std::string name)
    {
        this->name = name;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Struct_Access " << name << std::endl;
        if (continues)
        {
            member->print(i + 1);
        }
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

    void print(int i)
    {
        print_offset(i);
        std::cout << "Binary_Operation " << op << std::endl;
        left->print(i + 1);
        right->print(i + 1);
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

    void print(int i)
    {
        print_offset(i);
        std::cout << "Negation" << std::endl;
        value->print(i + 1);
    }
};

#endif // !_EXPRESSION_H_