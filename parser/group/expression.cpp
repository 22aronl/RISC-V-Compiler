
#include "expression.h"
#include <iostream>

R_Node* Number::gen(Environment* env) {
    return new R_Node(std::to_string(value), "", "");
}

std::string Number::reduce(Environment* env) {
    return std::to_string(value);
}

R_Node* Variable::gen(Environment* env) {
    return new R_Node(name, "", "");
}

std::string Variable::reduce(Environment* env) {
    return name;
}

R_Node* Function_Call::gen(Environment* env) {
    //TODO: Implement
    //TODO: Probablay unecessary
    std::cout << "Function_Call::gen not implemented" << std::endl;
    return nullptr;
}

std::string Function_Call::reduce(Environment* env) {
    for(auto arg : args) {
        std::string arg_val = arg->reduce(env);
        env->add_node(new Node(Type::PARAM, "", new R_Node(arg_val, "", "")));
    }

    std::string l_node = env->get_new_var();
    env->add_node(new Node(Type::CALL, l_node, new R_Node(name, std::to_string(args.size()), ",")));
    return l_node;
}

R_Node* Binary_Operation::gen(Environment* env) {
    return new R_Node(left->reduce(env), right->reduce(env), op);
}

std::string Binary_Operation::reduce(Environment* env) {
    std::string left_arg = left->reduce(env);
    std::string right_arg = right->reduce(env);

    std::string l_node = env->get_new_var();

    env->add_node(new Node(Type::ASSIGN, l_node, new R_Node(left_arg, right_arg, op)));

    return l_node;
}

R_Node* Negation::gen(Environment* env) {
    return new R_Node(value->reduce(env), "", "-");
}

std::string Negation::reduce(Environment* env) {
    std::string arg = value->reduce(env);

    std::string l_node = env->get_new_var();

    env->add_node(new Node(Type::ASSIGN, l_node, new R_Node(arg, "", "-")));

    return l_node;
}

R_Node* Array_Access::gen(Environment* env) {
    //TODO: Later
    return nullptr;
}

std::string Array_Access::reduce(Environment* env) {
    //TODO: Later
    return "";
}

R_Node* Struct_Access::gen(Environment* env) {
    //TODO: Later
    return nullptr;
}

std::string Struct_Access::reduce(Environment* env) {
    //TODO: Later
    return "";
}