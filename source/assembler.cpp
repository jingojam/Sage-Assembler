#include "../headers/assembler.hpp"

Assembler::Assembler(){}

uint32_t Assembler::MapInstructionToIR(struct Operation operation){
    return static_cast<uint32_t>((operation.cpu_instruction << 16) | (operation.operand[0] << 8) | operation.operand[0]);
}