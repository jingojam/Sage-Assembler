#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <array>
#include <optional>
#include <unordered_map>
#include <variant>
#include <typeinfo>
#include "instruction.hpp"

// v.1

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

static const uint8_t NO_OPERAND          = 1;
static const uint8_t DESTINATION_OPERAND = 2;
static const uint8_t SOURCE_OPERAND      = 3;

std::unordered_map<std::string, uint16_t> INSTRUCTION_SET = {
    {"add", CpuInstruction::ADD},
    {"sub", CpuInstruction::SUB},
    {"mul", CpuInstruction::MUL},
    {"div", CpuInstruction::DIV},
    {"mov", CpuInstruction::MOV}
};

std::unordered_map<std::string, uint8_t> REGISTER_SET = {
    {"rax", 0x00},
    {"rbx", 0x01},
    {"rcx", 0x02},
    {"rdx", 0x03},
    {"rdi", 0x04},
    {"rsi", 0x05},
    {"rbp", 0x06},
    {"rsp", 0x07},
    {"r8", 0x08},
    {"r9", 0x09},
    {"r10", 0x0a},
    {"r11", 0x0b},
    {"r12", 0x0c},
    {"r13", 0x0d},
    {"r14", 0x0e},
    {"r15", 0x0f},
};

 /**
  * Immediate Representation of parsed assembly instructions
  *
  * - A CPU operation (ADD/SUB/etc) is required
  * - Optionally 0-2 operands:
  *     No operand for instructions like CALL/SYSCALL/RET/etc.
  *     1 operand for arithmetic INC/DEC/MUL/etc.
  *     2 operands for ADD/SUB/MOV/etc.
  *     
  *     1st index is the destination operand (for double-token instructions)
  *     2nd index is the source operand
  *
  * - Optional immediate field
  */
struct Instruction{
    uint8_t flag;
    CpuInstruction cpu_instruction;
    std::string destination_operand;
    std::variant<std::string, int64_t> source_operand;
};

class Parser{
    protected:
        std::vector<struct Instruction> instruction_stream;

    private:
        bool ValidFile(const std::string filename);

    public:
        Parser();

        void Parse(const std::string input_source);
};

#endif