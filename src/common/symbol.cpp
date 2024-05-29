#include "../../inc/common/symbol.hpp"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

/*
 *
 *
 *      SYMBOL TABLE
 *
 *
 */

symbol_struct::symbol_struct
(
    uint32_t idx, uint32_t value, int32_t size, SYMBOL_TYPE symbol_type,
    SYMBOL_BIND symbol_bind, uint32_t ndx, std::string symbol_name
) : symbol_idx(idx),value(value), size(size), symbol_type(symbol_type),
    symbol_bind(symbol_bind), ndx(ndx), symbol_name(symbol_name){};

std::ostream& operator<<(std::ostream& os, const symbol_struct& obj) {

    {
        os          << std::setw(5)     << std::left << obj.symbol_idx                          << " | "
                    << std::setw(20)    << std::left << obj.value                               << " | "
                    << std::setw(5)     << std::left << obj.size                                << " | "
                    << std::setw(10)    << std::left << symbol_type_2_string(obj.symbol_type)   << " | "
                    << std::setw(10)    << std::left << symbol_bind_2_string(obj.symbol_bind)   << " | "
                    << std::setw(5)     << std::left << obj.ndx                                 << " | "
                    << std::setw(36)    << std::left << obj.symbol_name                         << " | ";
    }

    return os;
}


auto symbol_struct::serialize(std::ofstream& binary_file) -> void {
    
    binary_file.write(reinterpret_cast<const char*>(&symbol_idx), sizeof(symbol_idx));
    binary_file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    binary_file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    binary_file.write(reinterpret_cast<const char*>(&symbol_type), sizeof(symbol_type));
    binary_file.write(reinterpret_cast<const char*>(&symbol_bind), sizeof(symbol_bind));
    binary_file.write(reinterpret_cast<const char*>(&ndx), sizeof(ndx));
    binary_file.write(reinterpret_cast<const char*>(symbol_name.c_str()), symbol_name.size() + 1);
}

auto symbol_struct::deserialize(std::ifstream& binary_file) -> symbol_struct {

    uint32_t _idx;
    uint32_t _value;
    int32_t  _size;
    SYMBOL_TYPE _symbol_type;
    SYMBOL_BIND _symbol_bind;
    uint32_t    _ndx;
    std::string _symbol_name;

    binary_file.read(reinterpret_cast<char*>(&_idx), sizeof(_idx));
    binary_file.read(reinterpret_cast<char*>(&_value), sizeof(_value));
    binary_file.read(reinterpret_cast<char*>(&_size), sizeof(_size));
    binary_file.read(reinterpret_cast<char*>(&_symbol_type), sizeof(_symbol_type));
    binary_file.read(reinterpret_cast<char*>(&_symbol_bind), sizeof(_symbol_bind));
    binary_file.read(reinterpret_cast<char*>(&_ndx), sizeof(_ndx));
    std::getline(binary_file, _symbol_name, '\0');

    return {
        _idx,
        _value,
        _size,
        _symbol_type,
        _symbol_bind,
        _ndx,
        _symbol_name
    };
}
