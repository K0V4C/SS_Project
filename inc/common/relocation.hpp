#ifndef _RELOCATION_HPP
#define _RELOCATION_HPP

#include <string>
#include <cstdint>
#include <fstream>

#include "types.hpp"

// And relocation table 
struct relocation_struct {

    relocation_struct();

    relocation_struct(  uint32_t,
                        RELOCATION_TYPE,
                        std::string,
                        int32_t
                        );

    uint32_t offset;
    RELOCATION_TYPE type;
    std::string symbol_name; // maybe use it to find idx?
    int32_t addend;

    friend std::ostream& operator<<(std::ostream& os, const relocation_struct& obj);

    auto serialize(std::ofstream& binary_data) -> void;
    static auto deserialize(std::ifstream& binary_file) -> void;

};

#endif