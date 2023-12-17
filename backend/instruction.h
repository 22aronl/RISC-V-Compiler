#pragma once

enum class I_Type { LOAD, STORE, ADD };

enum class O_Type { REG, IMM, MEM, NONE };

class Operand {
  public:
    O_Type op;

    Operand(O_Type op) : op(op) {}
};

class SingleOperand : public Operand {
    int val;

  public:
    SingleOperand(O_Type op, int val) : Operand(op), val(val) {}
};

class MemOperand : Operand {
    int reg;
    int offset;
    int shift;

    MemOperand(O_Type op, int reg, int offset, int shift)
        : Operand(op), reg(reg), offset(offset), shift(shift) {}
};

class Instruction {
    I_Type type;
    Operand *x;
    Operand *y;
    Operand *z;

  public:
    Instruction(I_Type type, Operand *x, Operand *y, Operand *z) : type(type), x(x), y(y), z(z) {}

    Instruction(I_Type type, Operand *x, Operand *y) : type(type), x(x), y(y), z(nullptr) {}

    ~Instruction() {
        delete x;
        delete y;
        if (z != nullptr)
            delete z;
    }
};