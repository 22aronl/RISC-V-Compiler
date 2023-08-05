#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include "statement.h"
#include "expression.h"

class Global
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

class Global_Var_Declaration : public Global
{
    std::string type;
    std::string name;
    Expression *value;

public:
    Global_Var_Declaration(std::string type, std::string name, Expression *value)
    {
        this->type = type;
        this->name = name;
        this->value = value;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Global_Var_Declaration " << type << " " << name << std::endl;
        value->print(i + 1);
    }
};

class Global_Array_Declaration : public Global
{
    std::string type;
    std::string name;
    Expression *size;

public:
    Global_Array_Declaration(std::string type, std::string name, Expression *size)
    {
        this->type = type;
        this->name = name;
        this->size = size;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Global_Array_Declaration " << type << " " << name << std::endl;
        size->print(i + 1);
    }
};

class Struct_Declaration : public Global
{
    std::string name;
    std::vector<Statement *> vars;

public:
    Struct_Declaration(std::string name, std::vector<Statement *> vars)
    {
        this->name = name;
        this->vars = vars;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Struct_Declaration " << name << std::endl;
        for (auto var : vars)
        {
            var->print(i + 1);
        }
    }
};

class Struct_Var_Declaration : public Global
{
    std::string type;
    std::string name;

public:
    Struct_Var_Declaration(std::string type, std::string name)
    {
        this->type = type;
        this->name = name;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Struct_Var_Declaration " << type << " " << name << std::endl;
    }
};

class Function_Declaration : public Global
{
    std::string type;
    std::string name;
    std::vector<Var_Declaration *> args;
    std::vector<Statement *> body;

public:
    Function_Declaration(std::string type, std::string name, std::vector<Var_Declaration *> args, std::vector<Statement *> body)
    {
        this->type = type;
        this->name = name;
        this->args = args;
        this->body = body;
    }

    void print(int i)
    {
        print_offset(i);
        std::cout << "Function_Declaration " << type << " " << name << std::endl;
        for (auto arg : args)
        {
            arg->print(i + 1);
        }
        for (auto statement : body)
        {
            statement->print(i + 1);
        }
    }
};

#endif // !GLOBAL_H