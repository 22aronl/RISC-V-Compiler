
#include "env.h"
#include <iostream>

Environment::Environment(Global_Environment *global_env)
{
    this->global_env = global_env;
    this->var_count = 0;
    this->label_count = 0;
    this->dec_var_count = 0;
}

int Environment::get_new_label()
{
    return ++label_count;
}

std::string Environment::get_new_var()
{
    return "_t" + std::to_string(var_count++);
}

void Environment::add_node(Node *node)
{
    icg_nodes.push_back(node);
    if (upcoming_labels.size() > 0)
    {
        for (int label : upcoming_labels)
        {
            node->add_label(label);
        }
        upcoming_labels.clear();
    }
}

void Environment::add_label(int label, int index)
{
    icg_nodes[index]->add_label(label);
}

void Environment::add_variable(std::string name)
{
    var_map[name] = this->dec_var_count++;
}

void Environment::emit_label(int label)
{
    upcoming_labels.push_back(label);
}

void Environment::print_icg()
{
    for(int i = 0; i < icg_nodes.size(); i++)
    {
        Node* curNode = icg_nodes[i];
        for(int label : icg_nodes[i]->labels)
        {
            std::cout << label << ":";
        }

        std::cout << "\t";

        if(curNode->type == Type::RETURN) {
            std::cout << "RETURN " << curNode->l_value << std::endl;
        } else if (curNode->type == Type::JUMP)
            std::cout << "JUMP " << curNode->jump_label << std::endl;
        else if (curNode->type == Type::IF_FALSE)
            std::cout << "IF_FALSE " << curNode->l_value << " " << curNode->jump_label << std::endl;
        else if (curNode->type == Type::ASSIGN)
            std::cout << "ASSIGN " << curNode->l_value << " = " << curNode->r_node->arg1 << " " << curNode->r_node->op << " " << curNode->r_node->arg2 << std::endl;
        else if(curNode ->type == Type::COPY)
            std::cout << "COPY " << curNode->l_value << " = " << curNode->r_node->arg1 << std::endl;
        else if(curNode->type == Type::POINTER)
            std::cout << "POINTER " << curNode->l_value << " = " << curNode->r_node->arg1 << std::endl;
        else if(curNode->type == Type::POINTEE)
            std::cout << "POINTEE " << curNode->l_value << " = " << curNode->r_node->arg1 << std::endl;
        else if(curNode->type == Type::PARAM)
            std::cout << "PARAM " << curNode->l_value << " = " << curNode->r_node->arg1 << std::endl;
        else if(curNode->type == Type::CALL)
            std::cout << "CALL " << curNode->l_value << " = " << curNode->r_node->arg1 << std::endl;
        else
            std::cout << "ERROR " << std::endl;
    }
}