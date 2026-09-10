#include "instruction.h"

/**
 * Default constructor
 */
LegacyInstruction::LegacyInstruction() : 
    legacy() {}, rex(0), escseq(0), op(0), modrm(0), sib(0), disp(0), imm(0)
    {}


/**
 * Initializes Legacy prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetLegacyPrefix(bool operand_size_override_prefix, bool address_size_override_prefix, SegmentOverridePrefix segment_override_prefix, bool lock_prefix, RepeatPrefix repeat_prefix){
    // AMD64 legacy prefixes can be encoded without order
    if(operand_size_override_prefix){
        this->legacy[0] = 0x66;
    }

    if(address_size_override_prefix){
        this->legacy[1] = 0x67;
    }

    switch(segment_override_prefix){
        case SegmentOverridePrefix::CS:
            this->legacy[2] = 0x2e;
            break;
        case SegmentOverridePrefix::DS:
            this->legacy[2] = 0x3e;
            break;
        case SegmentOverridePrefix::ES:
            this->legacy[2] = 0x26;
            break;
        case SegmentOverridePrefix::FS:
            this->legacy[2] = 0x64;
            break;
        case SegmentOverridePrefix::GS:
            this->legacy[2] = 0x65;
            break;
        case SegmentOverridePrefix::SS:
            this->legacy[2] = 0x36;
            break;
        default:
            this->legacy[2] = 0x00
            break;
    }

    if(lock_prefix){
        this->legacy[3] = 0xf0;
    }

    switch(repeat_prefix){
        case RepeatPrefix::REP:
        case RepeatPrefix::REPEZ:
            this->legacy[4] = 0xf3;
            break;
        case RepeatPrefix::REPNEZ:
            this->legacy[4] = 0xf2;
            break;
        default:
            this->legacy[4] = 0x00;
            break;
    }
}

/**
 * Initializes REX prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetRexPrefix(bool rex_w, bool rex_r, bool rex_x, bool rex_b){
    this->rex = 0x40;

    if(rex_w){
        this->rex = this->rex | 0x08;
    }
    
    if(rex_r){
        this->rex = this->rex | 0x04;
    }

    if(rex_x){
        this->rex = this->rex | 0x02;
    }

    if(rex_b){
        this->rex = this->rex | 0x01;
    }
}

/**
 * Initializes Escape prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetEscapeSequence(EscapeSequence escseq){
    switch(escseq){
        case EscapeSequence::PRIMARY:
            this->escseq = 0x0f0f;
            break;
        case EscapeSequence::EXT0SSE:
            this->escseq = 0x380f;
            break;
        case EscapeSequence::EXT1SSE:
            this->escseq = 0x3a0f;
            break;
        default:
            this->escseq = 0;
            break;
    }
}

/**
 * Encodes operation into an internal representation and maps AMD64 opcode.
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetOpcode(struct Operation operation){
    // the operation/cpu instruction byte is placed at the most significant byte position (8th byte)
    uint64_t encoded_operation = static_cast<uint64_t>(operation.cpu_instruction) << 56;

    // then encode every operand in the instruction from left to right (by order of parsing)
    for(size_t i = 0; i < operation.operands.size(); i++){
        uint64_t operand = static_cast<uint8_t>(operation.operands[i])

        // starting from the 48th bit (6th byte) down to the 0th bit (1st byte)
        encoded_operation = (encoded_operation | operand) << (48 - (i*8));
    }

    auto it = PRIMARY_OPCODE_MAP.find(encoded_operation);

    if(it != PRIMARY_OPCODE_MAP.end()){
        this->op = it->second;
    } else{
        // FUture ref: push an error to a global error buffer
        return;
    }
}