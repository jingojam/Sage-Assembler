#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string.h>
#include "instruction.hpp"
#include "parser.hpp"

class Assembler : public Parser{
    public:
        Assembler();

        void Assemble(const std::string& source, const std::string& output);

        uint32_t MapInstructionToIR(struct Operation operation);
}

#endif