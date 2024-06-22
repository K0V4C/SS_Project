#include "../../inc/common/section.hpp"

#include <cstdint>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

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
    
    for(int i = 0; i < 111; i+=1) {
        os << "-";
    }
    
    os << std::endl;
    
    // Binary data
    
    os << "Binary data: \n";
    
    uint32_t counter = 0;
    
    for(uint8_t data : obj.binary_data.raw) {
        
        if(counter % 8 == 0) {
            os << std::endl;
            os << std::setw(8) << std::setfill('0') << std::hex << std::right << counter << " : ";
            
        }
        
        os << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data);
        os << " ";
        
        counter += 1;
    }
    
    os << std::setfill(' ');
    
    return os;
}

auto section_struct::serialize(std::ofstream& binary_file) -> void {

    binary_file.write(reinterpret_cast<const char*>(name.c_str()), name.size() + 1);
    binary_file.write(reinterpret_cast<const char*>(&section_idx), sizeof(section_idx));

    // Put number of relocations
    const uint32_t num_of_realocs = relocations.size();
    binary_file.write(reinterpret_cast<const char*>(&num_of_realocs), sizeof(num_of_realocs));

    for(auto e : relocations) {
        e.serialize(binary_file);
    }

    const uint32_t binary_size = binary_data.raw.size();
    binary_file.write(reinterpret_cast<const char*>(&binary_size), sizeof(binary_size));

    binary_data.serialize(binary_file);
}

auto section_struct::deserialize(std::ifstream& binary_file) -> section_struct {

    std::string _section_name;
    uint32_t    _section_idx;

    std::getline(binary_file, _section_name, '\0');
    binary_file.read(reinterpret_cast<char*>(&_section_idx), sizeof(_section_idx));

    uint32_t _num_of_realocs;
    binary_file.read(reinterpret_cast<char*>(&_num_of_realocs), sizeof(_num_of_realocs));

    std::vector<relocation_struct> _relocations;

    for(int i = 0; i < _num_of_realocs; i++) {
        auto relocation = relocation_struct::deserialize(binary_file);
        _relocations.push_back(relocation);
    }

    uint32_t _binary_size;
    binary_file.read(reinterpret_cast<char*>(&_binary_size), sizeof(_binary_size));

    auto _data = binary_data_struct::deserialize(binary_file, _binary_size);

    section_struct new_section = {
        _section_idx,
        _section_name
    };

    new_section.relocations = _relocations;
    new_section.binary_data = _data;

    return new_section;

}
