#include "inc/common/relocation.hpp"
#include <iomanip>

/*
 *
 *
 *      RELOCATION TABLE
 *
 *
 */

relocation_struct::relocation_struct()
    : offset(0), type(RELOCATION_TYPE::ABS32), symbol_name(""), addend(0) {}

relocation_struct::relocation_struct
(
    uint32_t offset, RELOCATION_TYPE type, std::string symbol_name, int32_t addend
) : offset(offset), type(type), symbol_name(symbol_name), addend(addend){};


std::ostream& operator<<(std::ostream& os, const relocation_struct& obj) {

    {
        os  << std::setw(25) << std::left << obj.offset                             << " | "
            << std::setw(25) << std::left << relocation_type_2_string(obj.type)     << " | "
            << std::setw(25) << std::left << obj.symbol_name                        << " | "
            << std::setw(25) << std::left << obj.addend                             << " | ";
    }


    return os;
}

auto relocation_struct::serialize(std::ofstream& binary_file) -> void {

    binary_file.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    binary_file.write(reinterpret_cast<const char*>(&type), sizeof(type));
    binary_file.write(reinterpret_cast<const char*>(symbol_name.c_str()), symbol_name.size() + 1);
    binary_file.write(reinterpret_cast<const char*>(&addend), sizeof(addend));

}

auto relocation_struct::deserialize(std::ifstream& binary_file) -> void {

}