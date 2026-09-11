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
 * AMD64 legacy prefix flags
 * 
 * Flags:
 *   16 bits wide, most significant nibble ignored (x)
 *   xxxx 0000 0000 0000 NONE
 *   xxxx 0000 0000 0001 OPERAND_SIZE_OVERRIDE
 *   xxxx 0000 0000 0010 ADDRESS_SIZE_OVERRIDE
 *   xxxx 0000 0000 0100 LOCK
 *   xxxx 0000 0000 1000 REP
 *   xxxx 0000 0001 0000 REPE/Z
 *   xxxx 0000 0010 0000 REPNE/Z
 *   xxxx 0000 0100 0000 SEGMENT_OVERRIDE (CS)
 *   xxxx 0000 1000 0000 SEGMENT_OVERRIDE (DS)
 *   xxxx 0001 0000 0000 SEGMENT_OVERRIDE (ES)
 *   xxxx 0010 0000 0000 SEGMENT_OVERRIDE (FS)
 *   xxxx 0100 0000 0000 SEGMENT_OVERRIDE (GS)
 *   xxxx 1000 0000 0000 SEGMENT_OVERRIDE (SS)
 */
inline static uint16_t OPERAND_SIZE_OVERRIDE_FLAG = 0x0001;
inline static uint16_t ADDRESS_SIZE_OVERRIDE_FLAG = 0x0002;
inline static uint16_t LOCK_FLAG                  = 0x0004;

// REPEAT prefix flags (mutually exclusive)
inline static uint16_t REP_FLAG                   = 0x0008;
inline static uint16_t REPEZ_FLAG                 = 0x0010;
inline static uint16_t REPNEZ_FLAG                = 0x0020;

// SEGMENT prefix flags (mutually exclusive)
inline static uint16_t SEGMENT_OVERRIDE_CS_FLAG   = 0x0040;
inline static uint16_t SEGMENT_OVERRIDE_DS_FLAG   = 0x0080; 
inline static uint16_t SEGMENT_OVERRIDE_ES_FLAG   = 0x0100;
inline static uint16_t SEGMENT_OVERRIDE_FS_FLAG   = 0x0200;
inline static uint16_t SEGMENT_OVERRIDE_GS_FLAG   = 0x0400; 
inline static uint16_t SEGMENT_OVERRIDE_SS_FLAG   = 0x0800;

/**
 * AMD64 Legacy prefix bytes
 */
inline static uint8_t OPERAND_SIZE_OVERRIDE = 0x66;
inline static uint8_t ADDRESS_SIZE_OVERRIDE = 0x67;
inline static uint8_t LOCK                  = 0xf0;
inline static uint8_t REP                   = 0xf3;
inline static uint8_t REPEZ                 = 0xf3;
inline static uint8_t REPNEZ                = 0xf2;
inline static uint8_t SEGMENT_OVERRIDE_CS   = 0x2e;
inline static uint8_t SEGMENT_OVERRIDE_DS   = 0x3e; 
inline static uint8_t SEGMENT_OVERRIDE_ES   = 0x26;
inline static uint8_t SEGMENT_OVERRIDE_FS   = 0x64;
inline static uint8_t SEGMENT_OVERRIDE_GS   = 0x65; 
inline static uint8_t SEGMENT_OVERRIDE_SS   = 0x36;


/**
 * AMD64 REX prefix bytes
 *
 * Flags:
 *   8 bits wide, most significant nibble is a constant (0x40)
 *
 *   const  wrxb
 *   0100   0001
 *   0100   0010
 *   0100   0100
 *   0100   1000
 */
inline static uint8_t REX_MS_NIBBLE   = 0x40;
inline static uint8_t REX_B           = 0x01;
inline static uint8_t REX_X           = 0x02;
inline static uint8_t REX_R           = 0x04;
inline static uint8_t REX_W           = 0x08;

/**
 * AMD64 escape sequences
 */
enum class EscapeSequence{
    NONE    = 0x0000,
    PRIMARY = 0x0f00,
    EXT0SSE = 0x0f38,
    EXT1SSE = 0x0f3a
};

/**
 * AMD64 instruction operand types
 */
enum class OperandType{
    /**Source Immediate/constants */
    IMM_64_S   = 0,   // 64-bit
    IMM_32_S   = 1,   // 32-bit
    IMM_16_S   = 2,   // 16-bit
    IMM_8_S    = 3,    // 8-bit
    IMM_GE16_S = 4, // variable (>=16-bit)
    
    /**Source Memory operands */
    MEM_64_S  = 5,
    MEM_32_S  = 6,
    MEM_16_S  = 7,
    MEM_8_S   = 8,
    MEM_V16_S = 9,

    /**Source CPU GPR (General Purpose Register) operands */
    REG_64_S  = 10,
    REG_32_S  = 11, 
    REG_16_S  = 12,
    REG_8_S   = 13,
    REG_V16_S = 14,

    /**Destination Immediate/constants */
    IMM_64_D  = 15,
    IMM_32_D  = 16,
    IMM_16_D  = 17,
    IMM_8_D   = 18,
    IMM_V16_D = 19,

    /**Destination Memory operands */
    MEM_64_D  = 20,
    MEM_32_D  = 21,
    MEM_16_D  = 22,
    MEM_8_D   = 23,
    MEM_V16_D = 24,

    /**Destination CPU GPR (General Purpose Register) operands */
    REG_64_D  = 25,
    REG_32_D  = 26,
    REG_16_D  = 27,
    REG_8_D   = 28,
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
    JMP_CND = 0x07,
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

        void SetLegacyPrefix(uint8_t legacy_prefix_flags);

        void SetRexPrefix(uint8_t rex_flags);

        void SetEscapeSequence(EscapeSequence escseq);

        void SetOpcode(struct Operation operation);
};

#endif