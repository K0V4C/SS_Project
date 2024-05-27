#include "inc/common/section.hpp"

#include <iomanip>
#include <fstream>
#include <iostream>

/*
 *
 *      SECTION TABLE 
 *
 *
 *
 */

section_struct::section_struct (uint32_t idx, std::string name) 
    :   
        section_idx(idx),
        name(name),
        relocations({}),
        binary_data({}){}

auto section_struct::add_relocation(
        uint32_t offset,
        RELOCATION_TYPE type,
        std::string symbol,
        int32_t addened) -> void {
    
    relocations.push_back({offset, type, symbol, addened});
}


std::ostream& operator<<(std::ostream& os, const section_struct& obj) {

    os  << std::left << std::setw(40) << obj.name        << " | " 
        << std::left << std::setw(20) << obj.section_idx << " | " 
        << std::left << std::setw(43) << "vvvvvvvvvv"    << " |\n";
    
    for(int i = 0; i < 111; i++ ) os << "_";
    
    os  << "\nRelocation Table:\n";

    os  << std::setw(25) << std::left << "offset"           << " | "
        << std::setw(25) << std::left << "relocation type"  << " | "
        << std::setw(25) << std::left << "symbol name"      << " | "
        << std::setw(25) << std::left << "addend"           << " | \n";
   

    for(auto& rel : obj.relocations) {
        os << rel << std::endl;
    }

      
    for(int i = 0; i < 111; i++ ) os << "^";
    os << std::endl;

    return os;
}

auto section_struct::serialize(std::ofstream& binary_file) -> void {

    binary_file.write(reinterpret_cast<const char*>(name.c_str()), name.size() + 1);
    binary_file.write(reinterpret_cast<const char*>(&section_idx), sizeof(section_idx));

    // Put number of relocations
    const auto num_of_realocs = relocations.size();
    binary_file.write(reinterpret_cast<const char*>(&num_of_realocs), sizeof(num_of_realocs));

    for(auto e : relocations) {
        e.serialize(binary_file);
    }

    // Now write binary 

    for(int i = 0; i < 60; i++ ) {
        const auto t = 0xff;
        binary_file.write(reinterpret_cast<const char*> (&t), sizeof(t));
    }

    const auto binary_size = binary_data.raw.size();
    binary_file.write(reinterpret_cast<const char*>(&binary_size), sizeof(binary_size));

    std::cout << "binary size: " << binary_size;
    binary_data.serialize(binary_file);

}

auto section_struct::deserialize(std::ifstream& binary_file) -> void {

}
