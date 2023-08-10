#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <iostream>
#include <vector>

#include "../../frontend/icg/env.h"

class Block
{
    std::vector<Node*> nodes;
    std::vector<int> next_blocks;
    bool is_end = false;

public:
    Block()
    {
        is_end = true;
    }

    Block(std::vector<Node*> nodes)
    {
        this->nodes = nodes;
    }

    void add_next_block(int block)
    {
        next_blocks.push_back(block);
    }
};

#endif // !_BLOCK_H_