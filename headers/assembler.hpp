#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string.h>
#include "instruction.hpp"
#include "parser.hpp"
#include "encoder.hpp"

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

class Assembler{
    private:
        Parser parser;
        Encoder encoder;

    public:
        Assembler();

        void Assemble(const std::string& source, const std::string& output);

        uint32_t MapInstructionToIR(struct Operation operation);
}

#endif