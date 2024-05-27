#ifndef _BINARY_DATA
#define _BINARY_DATA

#include <cstdint>
#include <vector>
#include <fstream>

#include "types.hpp"

struct binary_data_struct {
        
        binary_data_struct(){}
        
        auto add_instruction(uint32_t) -> void;

        auto add_byte(uint8_t) -> void;

        std::vector<uint8_t> raw; 


        auto serialize(std::ofstream& binary_file) -> void;
        static auto deserialize(std::ifstream& binary_file) -> void;
};

#endif