#include "../headers/instruction.hpp"

/**
 * Default constructor
 */
LegacyInstruction::LegacyInstruction() : 
    flags(0), legacy() {}, rex(0), escseq(0), op(0), modrm(0), sib(0), disp(0), imm(0)
    {}


/**
 * Initializes Legacy prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetLegacyPrefix(uint8_t legacy_prefix_flags){
    this->flags |= LEGACY_FLAG;

    // AMD64 legacy prefixes can be encoded without order
    if(legacy_prefix_flags & LEGACY_OPERAND_SIZE_OVERRIDE_FLAG){
        this->legacy[0] = LEGACY__OPERAND_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & LEGACY_ADDRESS_SIZE_OVERRIDE_FLAG){
        this->legacy[1] = LEGACY_ADDRESS_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & LEGACY_LOCK_FLAG){
        this->legacy[2] = LEGACY_LOCK;
    }

    if(legacy_prefix_flags & LEGACY_REP_FLAG || legacy_prefix_flags & LEGACY_REPEZ_FLAG){
        this->legacy[3] = LEGACY_REPEZ;
    } else if(legacy_prefix_flags & LEGACY_REPNEZ_FLAG){
        this->legacy[3] = LEGACY_REPNEZ;
    }

    if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_CS_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_CS;
    } else if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_DS_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_DS;
    } else if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_ES_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_ES;
    } else if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_FS_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_FS;
    } else if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_GS_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_GS;
    } else if(legacy_prefix_flags & LEGACY_SEGMENT_OVERRIDE_SS_FLAG){
        this->legacy[4] = LEGACY_SEGMENT_OVERRIDE_SS;
    }
}

/**
 * Initializes REX prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetRexPrefix(uint8_t rex_w, unit8_t rex_r, uint8_t rex_x, uint8_t rex_b){
    this->flags |= REX_FLAG;
    this->rex = REX_CONST | rex_w | rex_r | rex_x | rex_b; 
}

/**
 * Initializes Escape prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetEscapeSequence(EscapeSequence escseq){
    this->flags |= ESC_FLAG;
    this->escseq = static_cast<uint16_t>(escseq);
}

/**
 * Encodes operation into an internal representation and extracts AMD64 opcode.
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetOpcode(uint32_t op){
    this->flags |= OP_FLAG;
    this->op = op;
}

/**
 * Encodes ModR/M bytes 
 */
void LegacyInstruction::SetModRM(uint8_t mod, uint8_t reg, uint8_t rm){
    this->flags |= MODRM_FLAG;
    this->modrm |= (mod << 6) | (reg << 3) | rm;
}

/**
 * Encodes SIB bytes
 */
void LegacyInstruction::SetSIB(uint8_t scale_factor, uint8_t index, uint8_t base){
    this->flags |= SIB_FLAG;
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