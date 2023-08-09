#include <iostream>

class Emulator
{
    int registers[32];
    int mem[5000];
    int pc;
    bool running;

    void instruction_immediate(int rd, int opcodeB, int rs1, int imm12, int shamt, int opcodeC)
    {
        switch (opcodeB)
        {
        case 0b000: // ADDI
            registers[rd] = registers[rs1] + imm12;
            break;
        case 0b010: // SLTI
            registers[rd] = registers[rs1] < imm12;
            break;
        case 0b011: // SLTIU
            registers[rd] = (unsigned int)registers[rs1] < (unsigned int)imm12;
            break;
        case 0b100: // XORI
            registers[rd] = registers[rs1] ^ imm12;
            break;
        case 0b110: // ORI
            registers[rd] = registers[rs1] | imm12;
            break;
        case 0b111: // ANDI
            registers[rd] = registers[rs1] & imm12;
            break;
        case 0b001: // SLLI
            registers[rd] = registers[rs1] << shamt;
            break;
        case 0b101:
            if (opcodeC == 0b00000) // SRLI
                registers[rd] = (unsigned int)registers[rs1] >> shamt;
            else if (opcodeC == 0b010000) // SRAI
                registers[rd] = registers[rs1] >> shamt;
            break;
        default:
            std::cout << "Error: instruction_immediate" << std::endl;
            std::cout << "rd: " << rd << std::endl;
            std::cout << "opcodeB: " << opcodeB << std::endl;
            std::cout << "rs1: " << rs1 << std::endl;
            std::cout << "imm12: " << imm12 << std::endl;
            std::cout << "shamt: " << shamt << std::endl;
            break;
        }
    }

    void instruction_register(int rd, int opcodeB, int rs1, int rs2, int shamt, int opcodeC)
    {
        switch (opcodeB)
        {
        case 0b000:
            if (opcodeC == 0b00000) // ADD
                registers[rd] = registers[rs1] + registers[rs2];
            else if (opcodeC == 0b01000) // SUB
                registers[rd] = registers[rs1] - registers[rs2];
            break;
        case 0b001: // SLL
            registers[rd] = registers[rs1] << (registers[rs2] & 0b11111);
            break;
        case 0b010: // SLT
            registers[rd] = registers[rs1] < registers[rs2];
            break;
        case 0b011: // SLTU
            registers[rd] = (unsigned int)registers[rs1] < (unsigned int)registers[rs2];
            break;
        case 0b100: // XOR
            registers[rd] = registers[rs1] ^ registers[rs2];
            break;
        case 0b101:
            if (opcodeC == 0b00000) // SRL
                registers[rd] = (unsigned int)registers[rs1] >> (registers[rs2] & 0b11111);
            else if (opcodeC == 0b010000) // SRA
                registers[rd] = registers[rs1] >> (registers[rs2] & 0b11111);
            break;
        case 0b110: // OR
            registers[rd] = registers[rs1] | registers[rs2];
            break;
        case 0b111: // AND
            registers[rd] = registers[rs1] & registers[rs2];
            break;
        default:
            std::cout << "Error: instruction_register" << std::endl;
            std::cout << "rd: " << rd << std::endl;
            std::cout << "opcodeB: " << opcodeB << std::endl;
            std::cout << "rs1: " << rs1 << std::endl;
            std::cout << "rs2: " << rs2 << std::endl;
            std::cout << "shamt: " << shamt << std::endl;
            break;
        }
    }

    void instruction_branch(int opcodeB, int rs1, int rs2, int offset12, int pc_loc)
    {
        switch (opcodeB)
        {
        case 0b000: // beq
            if (registers[rs1] == registers[rs2])
                pc = pc_loc + offset12;
            break;
        case 0b001: // bne
            if (registers[rs1] != registers[rs2])
                pc = pc_loc + offset12;
            break;
        case 0b100: // blt
            if (registers[rs1] < registers[rs2])
                pc = pc_loc + offset12;
            break;
        case 0b101: // bge
            if (registers[rs1] >= registers[rs2])
                pc = pc_loc + offset12;
            break;
        case 0b110: // bltu
            if ((unsigned int)registers[rs1] < (unsigned int)registers[rs2])
                pc = pc_loc + offset12;
            break;
        case 0b111: // bgeu
            if ((unsigned int)registers[rs1] >= (unsigned int)registers[rs2])
                pc = pc_loc + offset12;
            break;
        default:
            std::cout << "Error: instruction_branch" << std::endl;
            std::cout << "opcodeB: " << opcodeB << std::endl;
            std::cout << "rs1: " << rs1 << std::endl;
            std::cout << "rs2: " << rs2 << std::endl;
            std::cout << "offset12: " << offset12 << std::endl;
            break;
        }
    }

    int signExtend(int value, int bits)
    {
        int signBit = 1 << (bits - 1);
        return (value ^ signBit) - signBit;
    }

    void instruction(int pc_loc)
    {
        int instruction = mem[pc_loc];
        int opcode = instruction >> 25;
        int rd = (instruction >> 20) & 0b1111;
        int opcodeB = (instruction >> 17) & 0b111;
        int rs1 = (instruction >> 12) & 0b1111;
        int rs2 = (instruction >> 7) & 0b1111;
        int imm20 = instruction & 0xFFFFF;
        int imm12 = instruction & 0xFFF;
        int shamt = (instruction >> 7) & 0b11111;
        int opcodeC = instruction & 0b11111;
        // offset[20|10:1|11|19:12]
        int offset20 = ((instruction >> 31) & 0x1) << 20 |  // imm[20]
                       ((instruction >> 21) & 0x3FF) << 1 | // imm[10:1]
                       ((instruction >> 20) & 0x1) << 11 |  // imm[11]
                       ((instruction >> 19) & 0x1FF) << 12; // imm[19:12]

        offset20 = signExtend(offset20, 21);

        int offset12 = ((instruction >> 31) & 0x1) << 12 | // imm[12]
                       ((instruction >> 25) & 0x3F) << 5 | // imm[10:5]
                       ((instruction >> 7) & 0x1F);        // imm[4:1], imm[11]

        offset12 = signExtend(offset12, 13);

        switch (opcode)
        {
        case 0b0110111: // LUI
            registers[rd] = imm20 << 12;
            break;
        case 0b0010111: // AUIPC
            registers[rd] = pc_loc + (imm20 << 12);
            break;
        case 0b0010011: // I-type
            instruction_immediate(rd, opcodeB, rs1, imm12, shamt, opcodeC);
            break;
        case 0b0110011: // R-type
            instruction_register(rd, opcodeB, rs1, rs2, shamt, opcodeC);
            break;
        case 0b111011: // ecall
            if (registers[17] == 10)
                running = false;
            else if (registers[17] == 4)
                std::cout << mem[registers[10]] << std::endl;
            break;
        case 0b0000011: // load
            if (opcodeB == 000)
                registers[rd] = signExtend((mem[registers[rs1] + imm12] & 0xFF), 8);
            else if (opcodeB == 001)
                registers[rd] = signExtend((mem[registers[rs1] + imm12] & 0xFFFF), 16);
            else if (opcodeB == 010)
                registers[rd] = mem[registers[rs1] + imm12];
            else if (opcodeB == 100)
                registers[rd] = (mem[registers[rs1] + imm12] & 0xFF);
            else if (opcodeB == 101)
                registers[rd] = (mem[registers[rs1] + imm12] & 0xFFFF);
            break;
        case 0b0100011: // store
            if (opcodeB == 0b000)
                mem[registers[rs1] + imm12] = registers[rs2] & 0xFF;
            else if (opcodeB == 0b001)
                mem[registers[rs1] + imm12] = registers[rs2] & 0xFFFF;
            else if (opcodeB == 0b010)
                mem[registers[rs1] + imm12] = registers[rs2];
            break;
        case 0b1100111: // jalr
        {
            int t = pc_loc + 4;
            pc = (registers[rs1] + signExtend(imm12, 12)) & ~1;
            registers[rd] = t;
            break;
        }
        case 0b1101111: // jal
            registers[rd] = pc_loc + 4;
            pc = pc_loc + offset20;
            break;
        case 0b1100011: // branch
            instruction_branch(opcodeB, rs1, rs2, offset12, pc_loc);
            break;
        default:
            std::cout << "Error: instruction " << instruction << std::endl;
            break;
        }
    }

public:
    Emulator()
    {
        for (int i = 0; i < 32; i++)
        {
            registers[i] = 0; // TODO: Set the stack pointer to the correct location //or maybe not
        }
        pc = 0;
        running = true;

        int num;
        int mem_offset = 0;
        // Read input until end of stdin (Ctrl+D on Unix-like systems, Ctrl+Z on Windows)
        while (std::cin >> num)
        {
            mem[mem_offset] = num;
        }
        std::cout << "Read " << mem_offset << " words of memory." << std::endl;
    }

    void run()
    {
        std::cout << "Simulation started." << std::endl;
        int counter = 0;
        while (running)
        {
            instruction(pc);
            pc += 2;
            counter++;
            if (counter > 1000000)
            {
                std::cout << "Simulation timed out." << std::endl;
                return;
            }
        }
        std::cout << "Simulation finished." << std::endl;
    }
};