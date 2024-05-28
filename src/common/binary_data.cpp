#include "inc/common/binary_data.hpp"
#include <iostream>
/*
 *
 *
 *      BINRAY DATA
 *
 *
 *
 */

auto binary_data_struct::add_byte(uint8_t byte) -> void{
    raw.push_back(byte);
}

// TODO: fix this maybe
auto binary_data_struct::add_instruction(uint32_t instr) -> void {
    raw.push_back (
            (instr & 0xff000000) >> 24
    );
    raw.push_back (
            (instr & 0xff0000) >> 16
    );
    raw.push_back(
            (instr & 0xff00) >> 8 
    );
    raw.push_back(instr & 0xff);
}

auto binary_data_struct::serialize(std::ofstream& binary_file) -> void {
    for(const auto e : raw) {
        const uint8_t send = e;
        binary_file.write(reinterpret_cast<const char*>(&send), sizeof(send));
    }
}

auto binary_data_struct::deserialize(std::ifstream& binary_file) -> void {

}
