#include "../headers/instruction.hpp"

/**
 * Default constructor
 */
LegacyInstruction::LegacyInstruction() : 
    flags(0), legacy() {}, rex(0), escseq(0), op(0), modrm(0), sib(0), disp(0), imm(0){}

/** 
 * COnverts a 16-bit unsigned integer to big endian byte order 
 */
uint16_t LegacyInstruction::ToBigEndian(uint16_t word){
    return static_cast<uint16_t>(((word & 0x00ff) << 8) | (word >> 8));
}

/**
 * Initializes Legacy prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetLegacyPrefix(uint8_t legacy_prefix_flags){
    this->flags |= IR::Flags::EncodingFlags::LEGACY_FLAG;

    // AMD64 legacy prefixes can be encoded without order
    if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::OPERAND_SIZE_OVERRIDE_FLAG){
        this->legacy[0] = AMD64::LegacyPrefix::OPERAND_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::ADDRESS_SIZE_OVERRIDE_FLAG){
        this->legacy[1] = AMD64::LegacyPrefix::ADDRESS_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::LOCK_FLAG){
        this->legacy[2] = AMD64::LegacyPrefix::LOCK;
    }

    if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::REP_FLAG || legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::REPEZ_FLAG){
        this->legacy[3] = AMD64::LegacyPrefix::REPEZ;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::REPNEZ_FLAG){
        this->legacy[3] = AMD64::LegacyPrefix::REPNEZ;
    }

    if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_CS_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_CS;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_DS_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_DS;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_ES_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_ES;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_FS_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_FS;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_GS_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_GS;
    } else if(legacy_prefix_flags & IR::Flags::LegacyPrefixFlags::SEGMENT_OVERRIDE_SS_FLAG){
        this->legacy[4] = AMD64::LegacyPrefix::SEGMENT_OVERRIDE_SS;
    }
}

/**
 * Initializes REX prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetRexPrefix(uint8_t rex_w, unit8_t rex_r, uint8_t rex_x, uint8_t rex_b){
    this->flags |= IR::Flags::EncodingFlags::REX_FLAG;
    this->rex = AMD64::REX::REX_CONST | rex_w | rex_r | rex_x | rex_b; 
}

/**
 * Initializes Escape prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetEscapeSequence(uint16_t escseq){
    this->flags |= IR::Flags::EncodingFlags::ESC_FLAG;
    this->escseq = escseq;
}

/**
 * Encodes operation into an internal representation and extracts AMD64 opcode.
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetOpcode(uint32_t op){
    this->flags |= IR::Flags::EncodingFlags::OP_FLAG;
    this->op = op;
}

/**
 * Encodes ModR/M bytes 
 */
void LegacyInstruction::SetModRM(uint8_t mod, uint8_t reg, uint8_t rm){
    this->flags |= IR::Flags::EncodingFlags::MODRM_FLAG;
    this->modrm |= (mod << 6) | (reg << 3) | rm;
}

/**
 * Encodes SIB bytes
 */
void LegacyInstruction::SetSIB(uint8_t scale_factor, uint8_t index, uint8_t base){
    this->flags |= IR::Flags::EncodingFlags::SIB_FLAG;
    this->sib |= (scale_factor << 6) | (index << 3) | base;
}

/**
 * Sets internal flags for encoder decisions
 */
void LegacyInstruction::SetFlags(uint8_t flags){
    this->flags = flags;
}

uint8_t LegacyInstruction::GetFlags(){
    return this->flags;
}