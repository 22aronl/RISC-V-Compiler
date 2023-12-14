#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <iostream>
#include <vector>
#include <unordered_map>

#include "../emitter.h"
#include "../../frontend/icg/env.h"

#define NUM_REGISTERS 16

class Block
{
    std::vector<int> in_blocks;
    bool is_end = false;
    int internal_location;

public:

    std::vector<int> next_blocks;
    std::vector<Node*> nodes;
    std::vector<std::string> live_out;
    std::vector<std::string> live_in;

    std::vector<std::string> register_list[NUM_REGISTERS];
    std::unordered_map<std::string, std::vector<int>> variable_descriptors;
    std::unordered_map<std::string, bool> reg_in_memory;

    Block(int loc)
    {
        is_end = true;
        //nodes = std::vector<Node*>();
        this->internal_location = loc;

        // set_up_reg_list();
    }

    Block(std::vector<Node*> nodes, int loc)
    {
        this->nodes = nodes;
        this->internal_location = loc;
        // set_up_reg_list();
    }

    ~Block() {
        // for(int i = 0; i < NUM_REGISTERS; i++) {
        //     delete register_list[i];
        // }
    }

    void add_next_block(int block)
    {
        next_blocks.push_back(block);
    }

    void add_in_block(int block) {
        in_blocks.push_back(block);
    }

    int get_location() {
        return this->internal_location;
    }

    int find_current_register(std::string variable) {
        auto vec = variable_descriptors[variable];
        if(vec.size() == 0)
            return -1;
        return vec[0];
    }

    void fill_register(int index, std::string reg, Emitter* em, Environment* env) {
        
    }

    int find_empty_register() {
        for(int i = 0; i < NUM_REGISTERS; i++) {
            auto reg_list = register_list[i];
            if(reg_list.size() == 0) {
                return i;
            }
        }
        return -1;
    }

    // void set_up_reg_list() {
    //     for(int i = 0; i < NUM_REGISTERS; i++) {
    //         register_list[i] = new std::vector<std::string>();
    //     }
    // }
};

#endif // !_BLOCK_H_