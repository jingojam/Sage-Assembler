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

/**
 * AMD64 encoding field flags
 * 
 * Flags:
 *  8 bits wide
 *  0000 0001 LEGACY_FLAG
 *  0000 0010 REX_FLAG
 *  0000 0100 ESC_FLAG
 *  0000 1000 OP_FLAG
 *  0001 0000 MODRM_FLAG
 *  0010 0000 SIB_FLAG
 *  0100 0000 DISP_FLAG
 *  1000 0000 IMM_FLAG
 */
static constexpr uint8_t LEGACY_FLAG = 0x01;
static constexpr uint8_t REX_FLAG    = 0x02;
static constexpr uint8_t ESC_FLAG    = 0x04;
static constexpr uint8_t OP_FLAG     = 0x08;
static constexpr uint8_t MODRM_FLAG  = 0x10;
static constexpr uint8_t SIB_FLAG    = 0x20;
static constexpr uint8_t DISP_FLAG   = 0x40;
static constexpr uint8_t IMM_FLAG    = 0x80;

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