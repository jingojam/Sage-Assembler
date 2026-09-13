#include "../headers/instruction.hpp"

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
void LegacyInstruction::SetLegacyPrefix(uint8_t legacy_prefix_flags){
    // AMD64 legacy prefixes can be encoded without order
    if(legacy_prefix_flags & OPERAND_SIZE_OVERRIDE_FLAG){
        this->legacy[0] = OPERAND_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & ADDRESS_SIZE_OVERRIDE_FLAG){
        this->legacy[1] = ADDRESS_SIZE_OVERRIDE;
    }

    if(legacy_prefix_flags & LOCK){
        this->legacy[2] = LOCK;
    }

    if(legacy_prefix_flags & REP_FLAG || legacy_prefix_flags & REPEZ_FLAG){
        this->legacy[3] = REPEZ;
    } else if(legacy_prefix_flags & REPNEZ_FLAG){
        this->legacy[3] = REPNEZ;
    }

    if(legacy_prefix_flags & SEGMENT_OVERRIDE_CS_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_CS;
    } else if(legacy_prefix_flags & SEGMENT_OVERRIDE_DS_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_DS;
    } else if(legacy_prefix_flags & SEGMENT_OVERRIDE_ES_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_ES;
    } else if(legacy_prefix_flags & SEGMENT_OVERRIDE_FS_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_FS;
    } else if(legacy_prefix_flags & SEGMENT_OVERRIDE_GS_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_GS;
    } else if(legacy_prefix_flags & SEGMENT_OVERRIDE_SS_FLAG){
        this->legacy[4] = SEGMENT_OVERRIDE_SS;
    }
}

/**
 * Initializes REX prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetRexPrefix(uint8_t rex_flags){
    this->rex = REX_MS_NIBBLE | rex_flags; 
}

/**
 * Initializes Escape prefixes.  
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetEscapeSequence(EscapeSequence escseq){
    this->escseq = static_cast<uint16_t>(escseq);
}

/**
 * Encodes operation into an internal representation and extracts AMD64 opcode.
 *
 * Note: The "magic" hexadecimal values used here are referenced from the AMD64 manual
 */
void LegacyInstruction::SetOpcode(uint32_t operation){

}