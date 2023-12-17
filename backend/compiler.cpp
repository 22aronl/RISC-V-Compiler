
#include <unordered_map>
#include <vector>

#include "../frontend/icg/env.h"
#include "block/block.h"

#include "compiler.h"
#include "emitter.h"

class RegUse {
  public:
    int x_reg;
    int y_reg;
    int z_reg;

    RegUse(int x, int y, int z) : x_reg(x), y_reg(y), z_reg(z) {}
};

// reg x = reg y, reg z
// use false if only y, true if y and z
RegUse get_reg(std::string x, std::string y, std::string z, bool use, Node* node, Block *b, Emitter *em, Environment *env) {
    int x_reg = -1, y_reg = -1, z_reg = -1;
    x_reg = b->find_current_register(x);

    // reg currently in a register
    y_reg = b->find_current_register(y);
    if (y_reg == -1) {
        // an empty slot for our register
        y_reg = b->find_empty_register(-1);
        if (y_reg != -1) {
            b->fill_register(y_reg, y, em, env);
        }
        //y_reg might be -1 -> y is not in a register and there are no empty registers 
    }

    if (use) {
        z_reg = b->find_current_register(z);
        if (z_reg == -1) {
            z_reg = b->find_empty_register(y_reg);
            if (z_reg != -1) {
                b->fill_register(z_reg, z, em, env);
            }
            //z_reg might be -1 -> z is not in a register and there are no empty registers
        }
    }

    

    return RegUse(x_reg, y_reg, z_reg);
}

void generate_machine_code(Block *b, Emitter *em, Environment *env) {}

void generate_block_dag(Block *b) {
    std::unordered_map<std::string, int> var_map;

    for (auto node : b->nodes) {
        // TODO: Build Dag
    }
}

void calculate_next_use(Block *b) {
    std::unordered_map<std::string, int>& next_use = b->next_use;
    next_use.insert(std::make_pair("", -1));
    for (int i = b->nodes.size() - 1; i >= 0; i--) {
        Node *cur_node = b->nodes[i];
        int x = -1;
        int y = -1;
        int z = -1;

        if (cur_node->r_node->arg1 != "" && next_use.find(cur_node->r_node->arg1) != next_use.end())
            y = next_use[cur_node->r_node->arg1];

        if (cur_node->r_node->arg2 != "" && next_use.find(cur_node->r_node->arg2) != next_use.end())
            z = next_use[cur_node->r_node->arg2];

        if (next_use.find(cur_node->l_value) != next_use.end())
            x = next_use[cur_node->l_value];

        cur_node->set_next_use(x, y, z);
        next_use[cur_node->l_value] = -1;

        if (cur_node->r_node->arg1 != "")
            next_use[cur_node->r_node->arg1] = i;
        if (cur_node->r_node->arg2 != "")
            next_use[cur_node->r_node->arg2] = i;
    }
}

std::vector<Block *> generate_basic_blocks(Environment *env) {
    std::vector<Node *> nodes = env->get_icg_nodes();
    std::vector<bool> block_jumps(nodes.size(), false);
    std::vector<int> jump_cor(env->get_label_count());

    for (int j = 0; j < nodes.size(); j++) {
        Node *i = nodes[j];
        for (int label : i->labels) {
            jump_cor[label] = j;
        }
    }

    for (int j = 0; j < nodes.size(); j++) {
        Node *i = nodes[j];
        if (i->type == Type::JUMP || i->type == Type::IF || i->type == Type::IF_FALSE) {
            block_jumps[jump_cor[i->jump_label]] = true;
            // block_jumps[i->jump_label] = true;
            block_jumps[j + 1] = true;
        } else if (i->type == Type::RETURN || i->type == Type::CALL) {
            block_jumps[j] = true;
        }
    }

    std::vector<Block *> blocks;
    std::vector<int> node_locations(nodes.size() + 1);
    int block_count = 0;

    std::vector<Node *> cur_block_nodes;
    for (int i = 0; i < nodes.size(); i++) {
        if (block_jumps[i] == 1) {
            blocks.push_back(new Block(cur_block_nodes, blocks.size()));
            block_count++;
            cur_block_nodes.clear();
        }

        cur_block_nodes.push_back(nodes[i]);
        node_locations[i] = block_count;
    }

    if (cur_block_nodes.size() > 0) {
        blocks.push_back(new Block(cur_block_nodes, blocks.size()));
        node_locations[nodes.size() - 1] = block_count;
    }

    blocks.push_back(new Block(blocks.size()));

    jump_cor[2] = 0;
    jump_cor[0] = jump_cor[1] = nodes.size();
    node_locations[nodes.size()] = blocks.size() - 1;

    // link blocks based on control flow
    for (int i = 0; i < nodes.size(); i++) {
        Node *curNode = nodes[i];
        if (curNode->type == Type::JUMP) {
            if (nodes[i - 1]->type == Type::RETURN && node_locations[i] == node_locations[i - 1])
                continue;
            blocks[node_locations[i]]->add_next_block(node_locations[jump_cor[curNode->jump_label]]);
            curNode->jump_label = node_locations[jump_cor[curNode->jump_label]];
        } else if (curNode->type == Type::IF || curNode->type == Type::IF_FALSE) {
            blocks[node_locations[i]]->add_next_block(node_locations[i + 1]);
            blocks[node_locations[i]]->add_next_block(node_locations[jump_cor[curNode->jump_label]]);
            curNode->jump_label = node_locations[jump_cor[curNode->jump_label]];
        } else if (curNode->type == Type::RETURN) {
            blocks[node_locations[i]]->add_next_block(blocks.size() - 1);
            curNode->jump_label = node_locations[jump_cor[curNode->jump_label]];
        } else if (curNode->type == Type::CALL) {
            blocks[node_locations[i]]->add_next_block(node_locations[i + 1]);
            curNode->jump_label = node_locations[jump_cor[curNode->jump_label]];
        }
    }

    for (auto block : blocks) {
        for (int out_block : block->next_blocks) {
            blocks[out_block]->add_in_block(block->get_location());
        }
    }

    for (auto block : blocks)
        calculate_next_use(block);

    return blocks;
}