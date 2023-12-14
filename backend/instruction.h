#pragma once

enum class I_Type
{
    LOAD,
    STORE,
    ADD
};

enum class O_Type 
{
    REG,
    IMM
};

class Operand {
    O_Type op;
    int val;
};

class Instruction {
    I_Type type;
    Operand x;
    Operand y;
    Operand z;
};