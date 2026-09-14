#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <unordered_map>
#include <string>
#include <array>
#include <cstdint>
#include <optional>

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

/**
 * AMD64 legacy prefix representation flags
 * 
 * These are flag bytes to represent mutually inclusive legacy prefixes,
 * which is part of SageAMD64 IR in signalling presence of such prefixes in encoding
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
static constexpr uint16_t OPERAND_SIZE_OVERRIDE_FLAG = 0x0001;
static constexpr uint16_t ADDRESS_SIZE_OVERRIDE_FLAG = 0x0002;
static constexpr uint16_t LOCK_FLAG                  = 0x0004;

// REPEAT prefix flags (mutually exclusive)
static constexpr uint16_t REP_FLAG    = 0x0008;
static constexpr uint16_t REPEZ_FLAG  = 0x0010;
static constexpr uint16_t REPNEZ_FLAG = 0x0020;

// SEGMENT prefix flags (mutually exclusive)
static constexpr uint16_t SEGMENT_OVERRIDE_CS_FLAG = 0x0040;
static constexpr uint16_t SEGMENT_OVERRIDE_DS_FLAG = 0x0080; 
static constexpr uint16_t SEGMENT_OVERRIDE_ES_FLAG = 0x0100;
static constexpr uint16_t SEGMENT_OVERRIDE_FS_FLAG = 0x0200;
static constexpr uint16_t SEGMENT_OVERRIDE_GS_FLAG = 0x0400; 
static constexpr uint16_t SEGMENT_OVERRIDE_SS_FLAG = 0x0800;

/**
 * AMD64 Legacy prefix bytes
 *
 * In contrast to the flags above, these bytes represent
 * actual values mapped from AMD64 specifications
 */
static constexpr uint8_t OPERAND_SIZE_OVERRIDE = 0x66;
static constexpr uint8_t ADDRESS_SIZE_OVERRIDE = 0x67;
static constexpr uint8_t LOCK                  = 0xf0;
static constexpr uint8_t REP                   = 0xf3;
static constexpr uint8_t REPEZ                 = 0xf3;
static constexpr uint8_t REPNEZ                = 0xf2;
static constexpr uint8_t SEGMENT_OVERRIDE_CS   = 0x2e;
static constexpr uint8_t SEGMENT_OVERRIDE_DS   = 0x3e; 
static constexpr uint8_t SEGMENT_OVERRIDE_ES   = 0x26;
static constexpr uint8_t SEGMENT_OVERRIDE_FS   = 0x64;
static constexpr uint8_t SEGMENT_OVERRIDE_GS   = 0x65; 
static constexpr uint8_t SEGMENT_OVERRIDE_SS   = 0x36;


/**
 * AMD64 REX prefix bytes
 *
 * These bytes represent the actual REX prefix fields in AMD64
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
static constexpr uint8_t REX_MS_NIBBLE = 0x40;
static constexpr uint8_t REX_B         = 0x01;
static constexpr uint8_t REX_X         = 0x02;
static constexpr uint8_t REX_R         = 0x04;
static constexpr uint8_t REX_W         = 0x08;

/**
 * AMD64 escape sequences
 *
 * These bytes are mapped to actual AMD64 escape sequences for opcode maps
 */
enum class EscapeSequence{
    NONE    = 0x0000,
    PRIMARY = 0x0f00,
    EXT0SSE = 0x0f38,
    EXT1SSE = 0x0f3a
};

/**
 * Mapping scheme for (internally represented) encoded operations to the primary/secondary opcode map
 *
 * Bit fields:
 *  Bits 0-7: Source Operand Field
 *  Bits 8-15: Destination Operand Field
 *  Bits 16-31: CPU Instruction Opcode Mapping
 * The source and destination operand bit fields are ignored for instructions that does not require an operand 
 *
 * For example, the assembly instructions: 
 *  add rax, rbx
 *  inc rax
 *  syscall
 *
 * will be represented, respectively, as:
 *  ADD (CpuInstruction::ADD) RAX (REG_64_D), RBX (RBX_64_S)
 *  INC (CpuInstruction::INC) RAX (REG_64_D)
 *  SYSCALL (CpuInstruction::SYSCALL)
 * 
 * And encoded as:
 *  00 00 19 0a
 *  00 ff 19 00
 *  0f 05 00 00
 *
 */


/**
 * AMD64 instruction operand types
 *
 * These bytes are part of the IR, not mapped to actual AMD64 constants
 * OperandType provides a mapping for operands for simpler and quicker asembler passes
 */
enum class OperandType : uint8_t{
    /**Source Immediate/constants */
    IMM_64_S   = 0x00,
    IMM_32_S   = 0x01,
    IMM_16_S   = 0x02,
    IMM_8_S    = 0x03,
    IMM_V_S  = 0x04,
    
    /**Source Memory operands */
    MEM_64_S  = 0x05,
    MEM_32_S  = 0x06,
    MEM_16_S  = 0x07,
    MEM_8_S   = 0x08,
    MEM_V_S = 0x09,

    /**Source CPU GPR (General Purpose Register) operands */
    REG_64_S  = 0x0a,
    REG_32_S  = 0x0b, 
    REG_16_S  = 0x0c,
    REG_8_S   = 0x0d,
    REG_V_S = 0x0e,

    /**Destination Immediate/constants */
    IMM_64_D  = 0x0f,
    IMM_32_D  = 0x10,
    IMM_16_D  = 0x11,
    IMM_8_D   = 0x12,
    IMM_V_D = 0x13,

    /**Destination Memory operands */
    MEM_64_D  = 0x14,
    MEM_32_D  = 0x15,
    MEM_16_D  = 0x16,
    MEM_8_D   = 0x17,
    MEM_V_D = 0x18,

    /**Destination CPU GPR (General Purpose Register) operands */
    REG_64_D  = 0x19,
    REG_32_D  = 0x1a,
    REG_16_D  = 0x1b,
    REG_8_D   = 0x1c,
    REG_V_D = 0x1d
};

/** 
 * CPU instructions mapped to AMD64 primary/secondary opcode maps
 *
 * These bytes correspond to actual AMD64 opcodes.
 */
enum class CpuInstruction : uint16_t{
    /** 2-Operand Fundamental Arithmetic Ops (Reg/Mem to/from Reg) */
    ADD = 0x01,  // ADD Ev, Gv
    ADC = 0x11,  // ADC Ev, Gv
    SUB = 0x29,  // SUB Ev, Gv

    /** 1-Operand Arithmetic Ops */
    IMUL = 0x0faf, // 2-byte Near Multi-operand IMUL (0x0F 0xAF)
    IDIV = 0xf7,
    MUL  = 0xf7,
    DIV  = 0xf7,
    INC  = 0xff,   // ModR/M form mandatory for AMD64 (uses /0 extension)

    /** 2-Operand Bit Ops */
    OR  = 0x09,  // OR Ev, Gv
    AND = 0x21,  // AND Ev, Gv
    XOR = 0x31,  // XOR Ev, Gv

    /** 1-Operand Bit Ops */
    NOT = 0xf7,  // Uses /2 ModR/M extension
    NEG = 0xf7,  // Uses /3 ModR/M extension
    // SHL
    // SHR
    // ROR
    // ROL

    /** Data Transfer */
    PUSH_GPR = 0x50,   // Base for PUSH r64 (0x50 + reg_id)
    POP_GPR  = 0x58,   // Base for POP r64 (0x58 + reg_id)
    LEA      = 0x8d,   // LEA Gv, M
    MOV_GPR  = 0x89,   // MOV Ev, Gv (Register/Memory to Register)
    MOV_IMM  = 0xb8,   // Base for MOV r64, imm64 (0xB8 + reg_id)

    /** Conditions */
    CMP = 0x39,  // CMP Ev, Gv

    /** Control Flow */
    CALL = 0xe8,  // CALL rel32
    RET  = 0xc3,

    /** SHort Jumps (8-bit relative displacement) */
    JO   = 0x70, // Jump if Overflow (OF=1)
    JNO  = 0x71, // Jump if Not Overflow (OF=0)
    JB   = 0x72, // Jump if Below / Carry / Not Above or Equal (CF=1) -> Unsigned <
    JAE  = 0x73, // Jump if Above or Equal / Not Below / No Carry (CF=0) -> Unsigned >=
    JE   = 0x74, // Jump if Equal / Zero (ZF=1) -> ==
    JNE  = 0x75, // Jump if Not Equal / Not Zero (ZF=0) -> !=
    JBE  = 0x76, // Jump if Below or Equal / Not Above (CF=1 or ZF=1) -> Unsigned <=
    JA   = 0x77, // Jump if Above / Not Below or Equal (CF=0 and ZF=0) -> Unsigned >
    JS   = 0x78, // Jump if Sign / Negative (SF=1)
    JNS  = 0x79, // Jump if Not Sign / Positive (SF=0)
    JP   = 0x7a, // Jump if Parity / Parity Even (PF=1)
    JNP  = 0x7b, // Jump if Not Parity / Parity Odd (PF=0)
    JL   = 0x7c, // Jump if Less / Not Greater or Equal (SF != OF) -> Signed <
    JGE  = 0x7d, // Jump if Greater or Equal / Not Less (SF == OF) -> Signed >=
    JLE  = 0x7e, // Jump if Less or Equal / Not Greater (ZF=1 or SF != OF) -> Signed <=
    JG   = 0x7f, // Jump if Greater / Not Less or Equal (ZF=0 and SF == OF

    /** Near Jumps (32-bit relative displacement alternatives) */
    JO_NEAR  = 0x0f80,
    JNO_NEAR = 0x0f81,
    JB_NEAR  = 0x0f82,
    JAE_NEAR = 0x0f83,
    JE_NEAR  = 0x0f84,
    JNE_NEAR = 0x0f85,
    JBE_NEAR = 0x0f86,
    JA_NEAR  = 0x0f87,
    JS_NEAR  = 0x0f88,
    JNS_NEAR = 0x0f89,
    JP_NEAR  = 0x0f8a,
    JNP_NEAR = 0x0f8b,
    JL_NEAR  = 0x0f8c,
    JGE_NEAR = 0x0f8d,
    JLE_NEAR = 0x0f8e,
    JG_NEAR  = 0x0f8f,

    /** Syscall */
    SYSCALL = 0x0f05,  // Native x86 instruction order (0x0F, 0x05)
};

/**
 * AMD64 ModRM fields 
 */
// Mod (bits 6-7)
static constexpr uint8_t MOD_MEMORY_ADDRESS   = 0x00;
static constexpr uint8_t MOD8_DISPLACEMENT_MEMORY_ADDRESS = 0x01;
static constexpr uint8_t MOD32_DISPLACEMENT_MEMORY_ADDRESS = 0x02;
static constexpr uint8_t MOD_REGISTER_OPERAND = 0x03;

// R/M
static constexpr uint8_t RAX     = 0x00;
static constexpr uint8_t RCX     = 0x01;
static constexpr uint8_t RDX     = 0x02;
static constexpr uint8_t RBX     = 0x03;
static constexpr uint8_t AH_RSP  = 0x04;
static constexpr uint8_t CH_RBP  = 0x05;
static constexpr uint8_t DH_RSI  = 0x06;
static constexpr uint8_t BH_RDI  = 0x07;
static constexpr uint8_t RAX_RM  = 0x00;
static constexpr uint8_t RCX_RM  = 0x01;
static constexpr uint8_t RDX_RM  = 0x02;
static constexpr uint8_t RBX_RM  = 0x03;
static constexpr uint8_t HAS_SIB = 0x04;
static constexpr uint8_t RBP_RM  = 0x05;
static constexpr uint8_t RSI_RM  = 0x06;
static constexpr uint8_t RDI_RM  = 0x07;

/**
 * AMD64 SIB fields
 */

// SIB scale factors
static constexpr uint8_t SIB1_SCALE_FACTOR = 0x00; // scale 8 bits
static constexpr uint8_t SIB2_SCALE_FACTOR = 0x01; // scale 16 bits
static constexpr uint8_t SIB4_SCALE_FACTOR = 0x02; // scale 32 bits
static constexpr uint8_t SIB8_SCALE_FACTOR = 0x03; // scale 64 bits

// SIB index
static constexpr uint8_t SIB_RAX_INDEX = 0x00;
static constexpr uint8_t SIB_RCX_INDEX = 0x01;
static constexpr uint8_t SIB_RBX_INDEX = 0x02;
static constexpr uint8_t SIB_RDX_INDEX = 0x03;
static constexpr uint8_t SIB_NO_INDEX  = 0x04;
static constexpr uint8_t SIB_RBP_INDEX = 0x05;
static constexpr uint8_t SIB_RSI_INDEX = 0x06;
static constexpr uint8_t SIB_RDI_INDEX = 0x07;

// SIB base
static constexpr uint8_t SIB_RAX_BASE    = 0x00;
static constexpr uint8_t SIB_RCX_BASE    = 0x01;
static constexpr uint8_t SIB_RBX_BASE    = 0x02;
static constexpr uint8_t SIB_RDX_BASE    = 0x03;
static constexpr uint8_t SIB_RSP_BASE    = 0x04;
static constexpr uint8_t SIB_RBP_NO_BASE = 0x05;
static constexpr uint8_t SIB_RSI_BASE    = 0x06;
static constexpr uint8_t SIB_RDI_BASE    = 0x07;

// SIB base for ModRM r/m = 0x04


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

/**
 * Primary structural representation of an amd64 instruction operation
 */
struct Operation{
    CpuInstruction cpu_instruction;
    uint8_t operands[2]; // depending on instruction, operation can have 0-2 operands
};

/** 
 * Legacy x86_64 instruction encoding fields
 */
class LegacyInstruction{
    private:
        uint8_t flags;
        std::array<uint8_t, 5> legacy; // 0-5 legacy prefixes
        uint8_t rex;                   // REX prefix
        uint16_t escseq;               // Escape sequence bytes
        uint16_t op;                    // opcode
        uint8_t modrm;                 // ModR/M 
        uint8_t sib;                   // SIB bytes
        int64_t disp;                  // Displacement
        int32_t imm;                  // Immediate

    public:
        LegacyInstruction();

        ~LegacyInstruction();

        void SetLegacyPrefix(uint8_t legacy_prefix_flags);

        void SetRexPrefix(uint8_t rex_flags);

        void SetEscapeSequence(EscapeSequence escseq);

        void SetOpcode(uint32_t operation);

        void SetModRM();

        void SetSIB();

        void SetDisplacement();

        void SetImmediate();
};

#endif