#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include "statement.h"
#include "expression.h"

class Global
{
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
};

class Struct_Declaration : public Global
{
    std::string name;
    std::vector<Var_Declaration *> vars;

public:
    Struct_Declaration(std::string name, std::vector<Var_Declaration *> vars)
    {
        this->name = name;
        this->vars = vars;
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
};

#endif // !GLOBAL_H