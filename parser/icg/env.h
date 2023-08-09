#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <unordered_map>
#include <string>
#include <vector>


enum class Type
{
    ASSIGN,
    COPY,
    JUMP,
    IF,
    IF_FALSE,
    PARAM,
    CALL,
    POINTER,
    POINTEE,
    RETURN
};

class R_Node
{
public:
    std::string arg1;
    std::string arg2;
    std::string op;
    R_Node(std::string arg1, std::string arg2, std::string op) : arg1(arg1), arg2(arg2), op(op)
    {
    }
};

class Node
{ // three address representation

public:
    Type type;
    std::string l_value;
    int jump_label;
    R_Node *r_node;
    std::vector<int> labels;

    Node(Type type, std::string l_value, R_Node *r_node) : type(type), l_value(l_value), r_node(r_node)
    {
    }

    Node(Type type, std::string l_value, int jump_label) : type(type), l_value(l_value), jump_label(jump_label)
    {
    }

    void add_label(int label)
    {
        labels.push_back(label);
    }
};

class Global_Environment
{
    int var_count;
    std::unordered_map<std::string, int> var_map;

public:
    Global_Environment();
    int get_new_var(std::string name);
};

class Environment
{
    int label_count;
    int var_count;
    std::unordered_map<std::string, int> var_map;
    Global_Environment *global_env;
    std::vector<Node *> icg_nodes;
    
    std::vector<int> upcoming_labels;

public:
    Environment(Global_Environment *global_env);

    int get_new_label();
    std::string get_new_var();

    void add_node(Node *node);

    void add_label(int label, int index);

    /// @brief adds a label to the next node that appears in the icg
    void emit_label(int label);

    void print_icg();

    int get_icg_size() {
        return icg_nodes.size();
    }

    std::vector<Node *> get_icg_nodes()
    {
        return icg_nodes;
    }
};

#endif // !_ENVIRONMENT_H_