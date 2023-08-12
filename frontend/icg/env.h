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
    int next_use1;
    int next_use2;
    R_Node(std::string arg1, std::string arg2, std::string op) : arg1(arg1), arg2(arg2), op(op)
    {
    }

    void set_next_use(int next_use1, int next_use2)
    {
        this->next_use1 = next_use1;
        this->next_use2 = next_use2;
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
    int l_next_use;

    Node(Type type, std::string l_value, R_Node *r_node) : type(type), l_value(l_value), r_node(r_node)
    {
    }

    Node(Type type, std::string l_value, int jump_label) : type(type), l_value(l_value), jump_label(jump_label)
    {
        r_node = new R_Node("", "", "");
    }

    void add_label(int label)
    {
        labels.push_back(label);
    }

    void set_next_use(int next_use1, int next_use2, int next_use3)
    {
        l_next_use = next_use1;
        r_node->set_next_use(next_use2, next_use3);
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

    int get_icg_size()
    {
        return icg_nodes.size();
    }

    int get_label_count()
    {
        return label_count;
    }

    std::vector<Node *> get_icg_nodes()
    {
        return icg_nodes;
    }
};

#endif // !_ENVIRONMENT_H_