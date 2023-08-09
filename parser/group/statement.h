#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include <vector>

class Statement
{
protected:
    void print_offset(int i)
    {
        for (int j = 0; j < i; j++)
        {
            std::cout << "  ";
        }
    }

public:
    virtual void print(int i) = 0;

    virtual void gen_icg(Environment *env, int start_label, int end_label) = 0;

    void print()
    {
        print(0);
    }
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

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "Var_Declaration " << type << " " << name << std::endl;
    }
};

class Var_Assignment : public Statement
{
    Expression *leftOp;
    Expression *value;
    std::string op;

public:
    Var_Assignment(Expression *leftOp, Expression *value, std::string op)
    {
        this->leftOp = leftOp;
        this->value = value;
        this->op = op;
    }

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "Var_Assignment " << op << std::endl;
        leftOp->print(i + 1);
        value->print(i + 1);
    }
};

// declares array
class Array_Declaration : public Statement
{
    std::string type;
    std::string name;
    Expression *size;

public:
    Array_Declaration(std::string type, std::string name, Expression *size)
    {
        this->type = type;
        this->name = name;
        this->size = size;
    }

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "Array_Declaration " << type << " " << name << std::endl;
        size->print(i + 1);
    }
};

class For_Loop : public Statement
{
    Statement *init;
    Expression *condition;
    Statement *increment;
    // body is a vector of statements
    Statement* body;

public:
    For_Loop(Statement *init, Expression *condition, Statement *increment,  Statement* body)
    {
        this->init = init;
        this->condition = condition;
        this->increment = increment;
        this->body = body;
    }

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "For_Loop" << std::endl;
        init->print(i + 1);
        condition->print(i + 1);
        increment->print(i + 1);
        body->print(i + 1);
    }
};

class If_Statement : public Statement
{
    Expression *condition;
    Statement* body;
    Statement* else_body;
    bool has_else = false;

public:
    If_Statement(Expression *condition, Statement* body)
    {
        this->condition = condition;
        this->body = body;
    }

    If_Statement(Expression *condition, Statement* body, Statement* else_body)
    {
        this->condition = condition;
        this->body = body;
        this->else_body = else_body;
        this->has_else = true;
    }

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "If_Statement" << std::endl;
        condition->print(i + 1);
        body->print(i + 1);
        if (has_else)
        {
            print_offset(i);
            std::cout << "Else" << std::endl;
            else_body->print(i + 1);
        }
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

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        print_offset(i);
        std::cout << "Return_Statement" << std::endl;
        value->print(i + 1);
    }
};

class Sequence : public Statement
{
    Statement *statement1;
    Statement *statement2;

public:
    Sequence(Statement *statement1, Statement *statement2)
    {
        this->statement1 = statement1;
        this->statement2 = statement2;
    }

    void gen_icg(Environment *env, int start_label, int end_label);

    void print(int i)
    {
        statement1->print(i);
        statement2->print(i);
    }
};

#endif // !STATEMENT_H