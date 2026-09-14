#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <array>
#include <optional>
#include <unordered_map>
#include "instruction.hpp"

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

 /**
  * Immediate Representation of parsed assembly instructions
  *
  * - A CPU operation (ADD/SUB/etc) is required
  * - Optionally 0-2 operands:
  *     No operand for instructions like CALL/SYSCALL/RET/etc.
  *     1 operand for arithmetic INC/DEC/MUL/etc.
  *     2 operands for ADD/SUB/MOV/etc.
  * - Optional immediate field
  */
struct Instruction{
    CpuInstruction cpu_instruction;
    std::optional<std::array<std::string, 2>> operands; // depending on instruction, operation can have 0-2 operands
    std::optional<int32_t> immediate;
};

class Parser{
    protected:
        std::vector<struct Instruction> instruction_stream;

    public:
        Parser();

        void Parse(const std::string input_program);
};

#endif