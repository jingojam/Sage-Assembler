#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "instruction.hpp"

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

struct Instruction{
    uint64_t line_number;
    LegacyInstruction instruction;
    std::string label; 
};

class Parser{
    protected:
        std::vector<struct Instruction> instruction_stream;

    public:
        Parser();

        void Parse(const std::string input_program);
};

#endif