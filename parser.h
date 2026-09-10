#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "instruction.h"

/**
 * Reference: AMD64 Technology, AMD64 Architecture Programmer's Manual Volumes 1-5
 * Publication No. 40332, Revision 1.0, July 2026
 */

class Parser{
    private:
        std::vector<LegacyInstruction> instruction_stream;

    public:
        Parser();

        ~Parser();
};

#endif