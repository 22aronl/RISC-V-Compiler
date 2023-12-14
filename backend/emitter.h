#pragma once


#include <vector>

#include "instruction.h"

class Emitter {
    std::vector<Instruction> instruction_list;

    void emit(Instruction inst) {
        instruction_list.push_back(inst);
    }
};