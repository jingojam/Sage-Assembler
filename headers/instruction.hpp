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

namespace Bytes {

    /**
     * Logical groupings for Byte mappings for 
     *  AMD64 encoding (mapped from AMD64 Programmer's Manual)
     */
    namespace AMD64 {

        /**
         * Legacy prefix byte
         */
        namespace LegacyPrefix {
            inline constexpr uint8_t OPERAND_SIZE_OVERRIDE = 0x66;
            inline constexpr uint8_t ADDRESS_SIZE_OVERRIDE = 0x67;
            inline constexpr uint8_t LOCK          = 0xf0;
            inline constexpr uint8_t REP           = 0xf3;
            inline constexpr uint8_t REPEZ         = 0xf3;
            inline constexpr uint8_t REPNEZ        = 0xf2;
            inline constexpr uint8_t SEGMENT_OVERRIDE_CS   = 0x2e;
            inline constexpr uint8_t SEGMENT_OVERRIDE_DS   = 0x3e; 
            inline constexpr uint8_t SEGMENT_OVERRIDE_ES   = 0x26;
            inline constexpr uint8_t SEGMENT_OVERRIDE_FS   = 0x64;
            inline constexpr uint8_t SEGMENT_OVERRIDE_GS   = 0x65; 
            inline constexpr uint8_t SEGMENT_OVERRIDE_SS   = 0x36;
        }

        /**
         * REX byte
         */
        namespace REX {
            inline constexpr uint8_t REX_CONST = 0x40;
            inline constexpr uint8_t B         = 0x01;
            inline constexpr uint8_t X         = 0x02;
            inline constexpr uint8_t R         = 0x04;
            inline constexpr uint8_t W         = 0x08;
        }

        /**
         * Escape sequence 
         */
        namespace EscapeSequence {
            inline constexpr uint16_t PRIMARY   = 0x0f00; 
            inline constexpr uint16_t SECONDARY = 0x0f38;
        }

        /**
         * Opcode values
         */
        namespace Opcode {
            /** 2-Operand Fundamental Arithmetic Ops (Reg/Mem to/from Reg) */
            inline constexpr uint8_t ADD = 0x01;  // ADD Ev, Gv
            inline constexpr uint8_t ADC = 0x11;  // ADC Ev, Gv (add carry-enabled)
            inline constexpr uint8_t SUB = 0x29;  // SUB Ev, Gv

            /** 1-Operand Arithmetic Ops */
            inline constexpr uint8_t IMUL = 0x0faf; // 2-byte Near Multi-operand IMUL (0x0F 0xAF)
            inline constexpr uint8_t IDIV = 0xf7;
            inline constexpr uint8_t MUL  = 0xf7;
            inline constexpr uint8_t DIV  = 0xf7;
            inline constexpr uint8_t INC  = 0xff;   // ModR/M form mandatory for AMD64 (uses /0 extension)

            /** 2-Operand Bit Ops */
            inline constexpr uint8_t OR  = 0x09;  // OR Ev, Gv
            inline constexpr uint8_t AND = 0x21;  // AND Ev, Gv
            inline constexpr uint8_t XOR = 0x31;  // XOR Ev, Gv

            /** 1-Operand Bit Ops */
            inline constexpr uint8_t NOT = 0xf7;  // Uses /2 ModR/M extension
            inline constexpr uint8_t NEG = 0xf7;  // Uses /3 ModR/M extension
            // SHL
            // SHR
            // ROR
            // ROL

            /** Data Transfer */
            inline constexpr uint8_t PUSH_GPR = 0x50;   // Base for PUSH r64 (0x50 + reg_id)
            inline constexpr uint8_t POP_GPR  = 0x58;   // Base for POP r64 (0x58 + reg_id)
            inline constexpr uint8_t LEA      = 0x8d;   // LEA Gv, M
            inline constexpr uint8_t MOV_GPR  = 0x89;   // MOV Ev, Gv (Register/Memory to Register)
            inline constexpr uint8_t MOV_IMM  = 0xb8;   // Base for MOV r64, imm64 (0xB8 + reg_id)

            /** Conditions */
            inline constexpr uint8_t CMP = 0x39;  // CMP Ev, Gv

            /** Control Flow */
            inline constexpr uint8_t CALL = 0xe8;  // CALL rel32
            inline constexpr uint8_t RET  = 0xc3;

            /** SHort Jumps (8-bit relative displacement) */
            inline constexpr uint8_t JO   = 0x70; // Jump if Overflow (OF=1)
            inline constexpr uint8_t JNO  = 0x71; // Jump if Not Overflow (OF=0)
            inline constexpr uint8_t JB   = 0x72; // Jump if Below / Carry / Not Above or Equal (CF=1) -> Unsigned <
            inline constexpr uint8_t JAE  = 0x73; // Jump if Above or Equal / Not Below / No Carry (CF=0) -> Unsigned >=
            inline constexpr uint8_t JE   = 0x74; // Jump if Equal / Zero (ZF=1) -> ==
            inline constexpr uint8_t JNE  = 0x75; // Jump if Not Equal / Not Zero (ZF=0) -> !=
            inline constexpr uint8_t JBE  = 0x76; // Jump if Below or Equal / Not Above (CF=1 or ZF=1) -> Unsigned <=
            inline constexpr uint8_t JA   = 0x77; // Jump if Above / Not Below or Equal (CF=0 and ZF=0) -> Unsigned >
            inline constexpr uint8_t JS   = 0x78; // Jump if Sign / Negative (SF=1)
            inline constexpr uint8_t JNS  = 0x79; // Jump if Not Sign / Positive (SF=0)
            inline constexpr uint8_t JP   = 0x7a; // Jump if Parity / Parity Even (PF=1)
            inline constexpr uint8_t JNP  = 0x7b; // Jump if Not Parity / Parity Odd (PF=0)
            inline constexpr uint8_t JL   = 0x7c; // Jump if Less / Not Greater or Equal (SF != OF) -> Signed <
            inline constexpr uint8_t JGE  = 0x7d; // Jump if Greater or Equal / Not Less (SF == OF) -> Signed >=
            inline constexpr uint8_t JLE  = 0x7e; // Jump if Less or Equal / Not Greater (ZF=1 or SF != OF) -> Signed <=
            inline constexpr uint8_t JG   = 0x7f; // Jump if Greater / Not Less or Equal (ZF=0 and SF == OF

            /** Near Jumps (32-bit relative displacement alternatives) */
            inline constexpr uint16_t JO_NEAR  = 0x0f80;
            inline constexpr uint16_t JNO_NEAR = 0x0f81;
            inline constexpr uint16_t JB_NEAR  = 0x0f82;
            inline constexpr uint16_t JAE_NEAR = 0x0f83;
            inline constexpr uint16_t JE_NEAR  = 0x0f84;
            inline constexpr uint16_t JNE_NEAR = 0x0f85;
            inline constexpr uint16_t JBE_NEAR = 0x0f86;
            inline constexpr uint16_t JA_NEAR  = 0x0f87;
            inline constexpr uint16_t JS_NEAR  = 0x0f88;
            inline constexpr uint16_t JNS_NEAR = 0x0f89;
            inline constexpr uint16_t JP_NEAR  = 0x0f8a;
            inline constexpr uint16_t JNP_NEAR = 0x0f8b;
            inline constexpr uint16_t JL_NEAR  = 0x0f8c;
            inline constexpr uint16_t JGE_NEAR = 0x0f8d;
            inline constexpr uint16_t JLE_NEAR = 0x0f8e;
            inline constexpr uint16_t JG_NEAR  = 0x0f8f;

            /** Syscall */
            inline constexpr uint16_t SYSCALL = 0x0f05;  // Native x86 instruction order (0x0F, 0x05)
        
            inline static constexpr std::unordered_map<std::string, uint8_t> PRIMARY_OPCODE_MAP = {
                
            };

            inline static constexpr std::unordered_map<std::string, uint16_t> SECONDARY_OPCODE_MAP = {
                {"syscall", 0x0f05}
            };
        }

        /**
         * ModRM byte
         */
        namespace ModRM {

            /**
             * ModRM.mod field
             */
            namespace Mod {
                inline constexpr uint8_t NO_DISP    = 0x00; // [rax]
                inline constexpr uint8_t DISP8      = 0x01; // [rax + 8-bit displacement]
                inline constexpr uint8_t DISP32     = 0x02; // [rax + 32-bit displacement]
                inline constexpr uint8_t REG_DIRECT = 0x03; // rax, rsp
            }

            /**
             * ModRM.reg field
             */
            namespace Reg {
                inline constexpr uint8_t RAX    = 0x00;
                inline constexpr uint8_t RCX    = 0x01;
                inline constexpr uint8_t RDX    = 0x02;
                inline constexpr uint8_t RBX    = 0x03;
                inline constexpr uint8_t AH_RSP = 0x04;
                inline constexpr uint8_t CH_RBP = 0x05;
                inline constexpr uint8_t DH_RSI = 0x06;
                inline constexpr uint8_t BH_RDI = 0x07; 
            }

            /**
             * ModRM.r/m field
             */
            namespace RM {
                inline constexpr uint8_t RAX = 0x00;
                inline constexpr uint8_t RCX = 0x01;
                inline constexpr uint8_t RDX = 0x02;
                inline constexpr uint8_t RBX = 0x03;
                inline constexpr uint8_t SIB = 0x04;
                inline constexpr uint8_t RBP = 0x05;
                inline constexpr uint8_t RSI = 0x06;
                inline constexpr uint8_t RDI = 0x07;
            }
        }

        /**
         * SIB bytes
         */
        namespace SIB {

            /**
             * 1/2/4/8-byte SIB scale factors
             */
            namespace ScaleFactor {
                inline constexpr uint8_t BYTE  = 0x00;
                inline constexpr uint8_t WORD  = 0x01;
                inline constexpr uint8_t DWORD = 0x02;
                inline constexpr uint8_t QWORD = 0x03;
            }

            /**
             * SIB index field
             */
            namespace Index {
                inline constexpr uint8_t RAX  = 0x00;
                inline constexpr uint8_t RCX  = 0x01;
                inline constexpr uint8_t RBX  = 0x02;
                inline constexpr uint8_t RDX  = 0x03;
                inline constexpr uint8_t NONE = 0x04;
                inline constexpr uint8_t RBP  = 0x05;
                inline constexpr uint8_t RSI  = 0x06;
                inline constexpr uint8_t RDI  = 0x07;
            }

            /**
             * SIB base field
             */
            namespace Base {
                inline constexpr uint8_t RAX         = 0x00;
                inline constexpr uint8_t RCX         = 0x01;
                inline constexpr uint8_t RBX         = 0x02;
                inline constexpr uint8_t RDX         = 0x03;
                inline constexpr uint8_t RSP         = 0x04;
                inline constexpr uint8_t NO_BASE_RBP = 0x05;
                inline constexpr uint8_t RSI         = 0x06;
                inline constexpr uint8_t RDI         = 0x07;
            }
        }
    }

    /**
     * Namespace for logical grouping of IR values
     */
    namepace IR {

        /**
         * Logical groupings of IR flags 
         */
        namespace Flags {

            /**
             * Assembler encoding flags
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
            namespace EncodingFlags {
                inline constexpr uint8_t LEGACY_FLAG = 0x01;
                inline constexpr uint8_t REX_FLAG    = 0x02;
                inline constexpr uint8_t ESC_FLAG    = 0x04;
                inline constexpr uint8_t OP_FLAG     = 0x08;
                inline constexpr uint8_t MODRM_FLAG  = 0x10;
                inline constexpr uint8_t SIB_FLAG    = 0x20;
                inline constexpr uint8_t DISP_FLAG   = 0x40;
                inline constexpr uint8_t IMM_FLAG    = 0x80;
            }

            /**
             * Legacy prefix flags
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
            namespace LegacyPrefixFlags {
                inline constexpr uint16_t OPERAND_SIZE_OVERRIDE_FLAG = 0x0001;
                inline constexpr uint16_t ADDRESS_SIZE_OVERRIDE_FLAG = 0x0002;
                inline constexpr uint16_t LOCK_FLAG                  = 0x0004;

                // REPEAT prefix flags (mutually exclusive)
                inline constexpr uint16_t REP_FLAG    = 0x0008;
                inline constexpr uint16_t REPEZ_FLAG  = 0x0010;
                inline constexpr uint16_t REPNEZ_FLAG = 0x0020;

                // SEGMENT prefix flags (mutually exclusive)
                inline constexpr uint16_t SEGMENT_OVERRIDE_CS_FLAG = 0x0040;
                inline constexpr uint16_t SEGMENT_OVERRIDE_DS_FLAG = 0x0080; 
                inline constexpr uint16_t SEGMENT_OVERRIDE_ES_FLAG = 0x0100;
                inline constexpr uint16_t SEGMENT_OVERRIDE_FS_FLAG = 0x0200;
                inline constexpr uint16_t SEGMENT_OVERRIDE_GS_FLAG = 0x0400; 
                inline constexpr uint16_t SEGMENT_OVERRIDE_SS_FLAG = 0x0800;
            }
        }

        /**
        * IR for nstruction operand types
        *
        * Operand:
        *  S - SOurce
        *  D - Destination
        *
        * Immediate:
        *  IMM64 - 64-bit Immediate
        *  IMM32 - 32-bit Immediate
        *  IMM16 - 16-bit Immediate
        *  IMM8  - 8-bit Immediate
        *
        * Memory:
        *  MEM64 - 64-bit Memory Operand
        *  MEM32 - 32-bit Memory Operand
        *  MEM16 - 16-bit Memory Operand
        *  MEM8  - 8-bit Memory Operand
        *
        * Register:
        *  REG64 - 64-bit Register Operand
        *  REG32 - 32-bit Register Operand
        *  REG16 - 16-bit Register Operand
        *  REG8  - 8-bit REgister Operand
        *
        *  IMM/MEM/REG V - Variable size (16/32/64 bits)
        */
        namespace OperandTypes {
            /**Immediate/constants */
            inline constexpr uint8_t IMM64 = 0x01;
            inline constexpr uint8_t IMM32 = 0x02;
            inline constexpr uint8_t IMM16 = 0x03;
            inline constexpr uint8_t IMM8  = 0x04;
            inline constexpr uint8_t IMMV  = 0x05;
            
            /**Memory operands */
            inline constexpr uint8_t MEM64 = 0x06;
            inline constexpr uint8_t MEM32 = 0x07;
            inline constexpr uint8_t MEM16 = 0x08;
            inline constexpr uint8_t MEM8  = 0x09;
            inline constexpr uint8_t MEMV  = 0x0a;

            /**CPU GPR (General Purpose Register) operands */
            inline constexpr uint8_t REG64 = 0x0b;
            inline constexpr uint8_t REG32 = 0x0c; 
            inline constexpr uint8_t REG16 = 0x0d;
            inline constexpr uint8_t REG8  = 0x0e;
            inline constexpr uint8_t REGV  = 0x0f;

            inline constexpr std::unordered_map<std::string, uint8_t> REGISTERS = {
                // 64-bit registers
                {"rax", REG64},
                {"rbx", REG64},
                {"rcx", REG64},
                {"rdx", REG64},
                {"rdi", REG64},
                {"rsi", REG64},
                {"rbp", REG64},
                {"rsp", REG64},
                {"r8",  REG64},
                {"r9",  REG64},
                {"r10", REG64},
                {"r11", REG64},
                {"r12", REG64},
                {"r13", REG64},
                {"r14", REG64},
                {"r15", REG64},

                // 32-bit registers
                {"eax", REG32},
                {"ebx", REG32},
                {"ecx", REG32},
                {"edx", REG32},
                {"edi", REG32},
                {"esi", REG32},
                {"ebp", REG32},
                {"esp", REG32},
                {"r8d",  REG32},
                {"r9d",  REG32},
                {"r10d", REG32},
                {"r11d", REG32},
                {"r12d", REG32},
                {"r13d", REG32},
                {"r14d", REG32},
                {"r15d", REG32},

                // 16-bit registers
                {"ax", REG16},
                {"bx", REG16},
                {"cx", REG16},
                {"dx", REG16},
                {"di", REG16},
                {"si", REG16},
                {"bp", REG16},
                {"sp", REG16},
                {"r8w",  REG16},
                {"r9w",  REG16},
                {"r10w", REG16},
                {"r11w", REG16},
                {"r12w", REG16},
                {"r13w", REG16},
                {"r14w", REG16},
                {"r15w", REG16},

                // 8-bit registers
                {"ah", REG8}, // ax upper 8 bits
                {"bh", REG8}, // bx upper 8 bits
                {"ch", REG8}, // cx upper 8 bits
                {"dh", REG8}, // dx upper 8 bits
                {"al", REG8}, // ax lower 8 bits
                {"bl", REG8}, // bx lower 8 bits
                {"cl", REG8}, // cx lower 8 bits
                {"dl", REG8}, // dx lower 8 bits
                {"dil", REG8},
                {"sil", REG8},
                {"bpl", REG8},
                {"spl", REG8},
                {"r8b",  REG8},
                {"r9b",  REG8},
                {"r10b", REG8},
                {"r11b", REG8},
                {"r12b", REG8},
                {"r13b", REG8},
                {"r14b", REG8},
                {"r15b", REG8}
            };
        }
    }
}

using namespace Bytes;

/**
 * Primary structural representation of an amd64 instruction operation
 */
struct Operation{
    uint16_t operation;
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

        uint16_t ToBigEndian(uint16_t word);

        void SetLegacyPrefix(uint8_t legacy_prefix_flags);

        void SetRexPrefix(uint8_t rex_flags);

        void SetEscapeSequence(EscapeSequence escseq);

        void SetOpcode(uint32_t operation);

        void SetModRM(uint8_t mod, uint8_t reg, uint8_t rm);

        void SetSIB(uint8_t scale_factor, uint8_t index, uint8_t base);

        void SetDisplacement();

        void SetImmediate();

        void SetFlags(uint8_t flags);

        uint8_t GetFlags();
};

#endif