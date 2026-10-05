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
        
    }
    
    input_source.close();
}