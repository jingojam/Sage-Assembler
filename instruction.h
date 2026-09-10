#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <unordered_map>
#include <string>
#include <array>
#include <cstdint>

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

/**
 * AMD64 segment override prefix
 */
enum class SegmentOverridePrefix{
    NONE,
    CS,
    DS,
    ES,
    FS,
    GS,
    SS
};

/**
 * AMD64 repeat prefixes
 */
enum class RepeatPrefix{
    NONE,
    REP,
    REPEZ,
    REPNEZ
};

/**
 * AMD64 escape sequences
 */
enum class EscapeSequence{
    PRIMARY,
    EXT0SSE,
    EXT1SSE
};

/**
 * AMD64 instruction operand types
 */
enum class OperandType{
    /**Source Immediate/constants */
    IMM_64_S = 0,   // 64-bit
    IMM_32_S = 1,   // 32-bit
    IMM_16_S = 2,   // 16-bit
    IMM_8_S = 3,    // 8-bit
    IMM_GE16_S = 4, // variable (>=16-bit)
    
    /**Source Memory operands */
    MEM_64_S = 5,
    MEM_32_S = 6,
    MEM_16_S = 7,
    MEM_8_S = 8,
    MEM_V16_S = 9,

    /**Source CPU GPR (General Purpose Register) operands */
    REG_64_S = 10,
    REG_32_S = 11, 
    REG_16_S = 12,
    REG_8_S = 13,
    REG_V16_S = 14,

    /**Destination Immediate/constants */
    IMM_64_D = 15,
    IMM_32_D = 16,
    IMM_16_D = 17,
    IMM_8_D = 18,
    IMM_V16_D = 19,

    /**Destination Memory operands */
    MEM_64_D = 20,
    MEM_32_D = 21,
    MEM_16_D = 22,
    MEM_8_D = 23,
    MEM_V16_D = 24,

    /**Destination CPU GPR (General Purpose Register) operands */
    REG_64_D = 25,
    REG_32_D = 26,
    REG_16_D = 27,
    REG_8_D = 28,
    REG_V16_D = 29
};

/** 
 * CPU instructions mapped to x86-64 primary opcode map (high nibble)
 */
enum class CpuInstruction{
    ADD = 0x00,
    ADC = 0x01,
    AND = 0x02,
    XOR = 0x03,
    INC = 0x04, 
    PUSH = 0x05,
    JMP_COND = 0x07,
    MOV0 = 0x0a,
    MOV1 = 0x0b,
    SUB = 0x20
};

/**
 * Primary structural representation of an x86-64 instruction operation
 */
struct Operation{
    CpuInstruction cpu_instruction;
    vector<OperandType> operands; // depending on instruction, operation can have 0-5 operands
};

/**
 * Mapping for (internally) encoded operations to the primary opcode map
 */
inline static std::unordered_map<uint64_t, uint8_t> PRIMARY_OPCODE_MAP = {
    /**ADD MEM_8_D, REG_8_S */
    {(static_cast<uint64_t>(CpuInstruction::ADD)  << 56) |
     (static_cast<uint64_t>(OperandType::MEM_8_D) << 48) |
     (static_cast<uint64_t>(OperandType::REG_8_S) << 40), 0x00},

    /**ADD REG_8_D, REG_8_S */
    {(static_cast<uint64_t>(CpuInstruction::ADD)  << 56) |
     (static_cast<uint64_t>(OperandType::REG_8_D) << 48) |
     (static_cast<uint64_t>(OperandType::REG_8_S) << 40), 0x00},
};

/** 
 * Legacy x86_64 instruction encoding fields
 */
class LegacyInstruction{
    private:
        std::array<uint8_t, 5> legacy; // 0-5 legacy prefixes
        uint8_t rex;                   // REX prefix
        uint16_t escseq;               // Escape sequence bytes
        uint8_t op;                    // opcode
        uint8_t modrm;                 // ModR/M 
        uint8_t sib;                   // SIB bytes
        int64_t disp;                  // Displacement
        uint64_t imm;                  // Immediate

    public:
        LegacyInstruction();

        ~LegacyInstruction();

        void SetLegacyPrefix(bool operand_size_override_prefix, bool address_size_override_prefix, SegmentOverridePrefix segment_override_prefix, bool lock_prefix, RepeatPrefix repeat_prefix);

        void SetRexPrefix(bool rex_w, bool rex_r, bool rex_x, bool rex_b);

        void SetEscapeSequence(EscapeSequence escseq);

        void SetOpcode(struct Operation operation);
};

#endif