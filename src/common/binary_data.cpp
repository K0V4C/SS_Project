#include "inc/common/binary_data.hpp"

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

// TODO: fix this
auto binary_data_struct::add_instruction(uint32_t instr) -> void {
    raw.push_back(instr & 0xff);
    raw.push_back(
            (instr & 0xff00) >> 8 
    );
    raw.push_back (
            (instr & 0xff0000) >> 16
    );
    raw.push_back (
            (instr & 0xff000000) >> 24
    );
}

auto binary_data_struct::serialize(std::ofstream& binary_file) -> void {

    for(const auto e : raw) {
        binary_file.write(reinterpret_cast<const char*>(&e), e);
    }

}