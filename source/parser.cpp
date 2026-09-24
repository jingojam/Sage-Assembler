#include "../headers/parser.hpp"

Parser::Parser(){}

bool ValidFile(const std::string filename){
    std::ifstream source(filename);
    
    if(!source.is_open()){
        std::cerr << "Error opening file \"" << filename << "\"";
        return false;
    }
    
    size_t asm_pos = filename.find('.asm')

    if(asm_pos != std::string::npos){
        std::cerr << "Expected file extension \".asm\"";
        return false;
    }

    return true;
}

void Parse(const std::string input_source){
    if(!ValidFile(input_source)){
        return;
    }

    std::vector<std::string> tokens;
    std::string line;
    uint8_t instruction_type = 0;

    while(std::getline(input_source, line)){
        size_t pos = 0;
        // at most 3 strings including instruction, and src/dest operands
        while((pos = line.find(',')) != std::string::npos){
            token = line.substr(0, pos);
            tokens.push_back(token);   
        }

        size_t tokens_amt = tokens.length();

        // single-token instructions (e.g., "ret")
        if(tokens_amt > 0){
            uint16_t instruction_code = INSTRUCTION_SET.at(tokens[NO_OPERAND - 1]);
            uint8_t instruction_flag = 0;

            if(tokens_amt == NO_OPERAND){
                instruction_flag = NO_OPERAND;
            } else if(tokens_amt == DESTINATION_OPERAND){
                instruction_flag = DESTINATION_OPERAND;
            } else if(tokens_amt == SOURCE_OPERAND){
                instruction_flag = SOURCE_OPERAND;
            }

            Instruction instruction = {
                .cpu_instruction = instruction_code
            };

            if(tokens_amt > NO_OPERAND){ // double-token instructions (e.g., "inc rax", "sub rsp")
                instruction.destination_operand = tokens[DESTINATION_OPERAND - 1];
            } 
            
            if(tokens_amt == SOURCE_OPERAND){ // instructions with a source and destination operand
                std::string source_operand = tokens[SOURCE_OPERAND - 1];

                // process a hex
                bool hex_prefix = source_operand.starts_with("0x");
                bool hex_suffix = source_operand.back() == "h";

                if(hex_prefix || hex_suffix){
                    int64_t hex_operand = std::stoll(source_operand, nullptr, 16);
                    instruction.source_operand = hex_operand;
                }

                // process a hex/integer
                if(typeid(reinterpret_cast<int64_t>(source_operand)) == typeid(int64_t)){

                }

                // process if a register
                if(typeid(reinterpret_cast<std::string>(source_operand)) == typeid(std::string)){
                    auto reg = REGISTER_SET.find(source_operand);

                    if(reg == REGISTER_SET.end()){
                        instruction.source_operand = source_operand;
                    }
                }
            }

            instruction.flag = instruction_flag;

            this->instruction_stream.push_back(instruction);
        }
    }
    
    input_source.close();
}