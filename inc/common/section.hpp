#ifndef _SECTION_HPP
#define _SECTION_HPP

#include "relocation.hpp"
#include "binary_data.hpp"

#include <vector>
#include <cstdint>
#include <string>


// Used to store values coresponding to sections
struct section_struct {

    section_struct(){};

    section_struct(uint32_t idx, std::string name);

    auto add_relocation(uint32_t, RELOCATION_TYPE, std::string, int32_t)->void;

    std::string name;
    int32_t section_idx; 
    std::vector<relocation_struct> relocations;
    binary_data_struct binary_data;

    friend std::ostream& operator<<(std::ostream& os, const section_struct& obj);

    auto serialize(std::ofstream& binary_file) -> void;

};

#endif // _SECTION_HPP