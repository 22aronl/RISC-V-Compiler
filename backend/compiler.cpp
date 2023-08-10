
#include <vector>

#include "block/block.h"
#include "../frontend/icg/env.h"

std::vector<Block *> generate_basic_blocks(std::vector<Node *> nodes)
{
    std::vector<bool> block_jumps(nodes.size(), false);
    for (int j = 0; j < nodes.size(); j++)
    {
        Node *i = nodes[j];
        if (i->type == Type::JUMP || i->type == Type::IF || i->type == Type::IF_FALSE)
        {
            block_jumps[i->jump_label] = true;
        }
        else if (i->type == Type::RETURN || i->type == Type::CALL)
        {
            block_jumps[j] = true;
        }
    }

    std::vector<Block *> blocks;
    std::vector<int> node_locations(nodes.size());
    int block_count = 0;

    std::vector<Node *> cur_block_nodes;
    for (int i = 0; i < nodes.size(); i++)
    {
        cur_block_nodes.push_back(nodes[i]);
        node_locations[i] = block_count;
        if (block_jumps[i] == 1)
        {
            blocks.push_back(new Block(cur_block_nodes));
            block_count++;
            cur_block_nodes.clear();
        }
    }

    if(cur_block_nodes.size() > 0)
    {
        blocks.push_back(new Block(cur_block_nodes));
        node_locations[nodes.size() - 1] = block_count;
    }

    blocks.push_back(new Block());

    //link blocks based on control flow
    for(int i = 0; i < nodes.size(); i++) {
        Node* curNode = nodes[i];
        if(curNode->type == Type::JUMP)
        {
            blocks[node_locations[i]]->add_next_block(node_locations[curNode->jump_label]);
        }
        else if(curNode->type == Type::IF || curNode->type == Type::IF_FALSE) {
            blocks[node_locations[i]]->add_next_block(node_locations[i + 1]);
            blocks[node_locations[i]]->add_next_block(node_locations[curNode->jump_label]);
        }
        else if(curNode->type == Type::RETURN) {
            blocks[node_locations[i]]->add_next_block(blocks.size() - 1);
        }
        else if(curNode->type == Type::CALL) {
            blocks[node_locations[i]]->add_next_block(node_locations[i + 1]);
        }
    }

    return blocks;

}