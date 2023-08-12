
#include "statement.h"

void Var_Declaration::gen_icg(Environment *env, int start_label, int end_label)
{
    // DO Nothing
}

void Var_Assignment::gen_icg(Environment *env, int start_label, int end_label)
{
    // TODO: Change to accomodate arrays and structs
    std::string left_name = ((Variable *)leftOp)->get_name();
    // std::string right_node = value->reduce(env);
    if (op == "=")
    {
        env->add_node(new Node(Type::ASSIGN, left_name, value->gen(env)));
    }
    else
    {
        env->add_node(new Node(Type::ASSIGN, left_name, new R_Node(left_name, value->reduce(env), op.substr(0, 1))));
    }
}

void Array_Declaration::gen_icg(Environment *env, int start_label, int end_label)
{
    // TODO: Implement arrays
}


void For_Loop::gen_icg(Environment *env, int start_label, int end_label)
{
    std::string l_node = condition->reduce(env);
    env->add_node(new Node(Type::IF_FALSE, l_node, end_label));
    int label = env->get_new_label();
    env->emit_label(label);
    int label2 = env->get_new_label();
    body->gen_icg(env, label, label2);
    env->emit_label(label2);
    increment->gen_icg(env, label2, end_label);
    env->add_node(new Node(Type::JUMP, "", start_label));
}

void If_Statement::gen_icg(Environment *env, int start_label, int end_label)
{
    int label1 = env->get_new_label();
    int label2 = env->get_new_label();

    std::string l_node = condition->reduce(env);
    env->add_node(new Node(Type::IF_FALSE, l_node, label2));
    env->emit_label(label1);
    body->gen_icg(env, label1, end_label);

    env->add_node(new Node(Type::JUMP, "", end_label));
    env->emit_label(label2);
    if(has_else) {
        else_body->gen_icg(env, label2, end_label);
    }
}


void Return_Statement::gen_icg(Environment *env, int start_label, int end_label)
{
    std::string l_node = value->reduce(env);
    env->add_node(new Node(Type::RETURN, l_node, 1)); //1 is the label pointing the the end
}


void Sequence::gen_icg(Environment* env, int start_label, int end_label)
{
    if(statement1 == nullptr)
    {
        statement2->gen_icg(env, start_label, end_label);
    }
    else if(statement2 == nullptr)
    {
        statement1->gen_icg(env, start_label, end_label);
    }
    else
    {
        int label = env->get_new_label();
        statement1->gen_icg(env, start_label, label);
        env->emit_label(label);
        statement2->gen_icg(env, label, end_label);
    }
}