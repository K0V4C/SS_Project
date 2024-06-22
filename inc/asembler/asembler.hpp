#ifndef _ASEMBLER_HPP
#define _ASEMBLER_HPP

#include "asm_actions/actions.hpp"
#include "../common/binary_data.hpp"
#include "../common/symbol.hpp"
#include "../common/relocation.hpp"
#include "../common/section.hpp"
#include "../common/types.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
class Asembler {
public:

    Asembler();

    auto asemble() -> void;
    auto backpatch() -> void;

    auto symbol_exists(std::string) -> bool;
    auto get_symbol(std::string) -> symbol_struct;

    auto section_exists(uint32_t) -> bool;
    auto get_section(uint32_t) -> section_struct;

    ~Asembler();

    // Used to do instructions and also directives
    static std::vector<std::unique_ptr<action>> file_actions;


    static std::map<std::string, symbol_struct> symbol_table;
    static uint32_t next_symbol_idx;
    static auto print_symbol_table() -> void;

    // Used to help find values
    static uint32_t next_section_idx;

    static auto get_section_counter() -> uint32_t;
    static auto get_current_section() -> section_struct&;
    static std::string current_section;
    static std::map<std::string, section_struct> section_table;
    static auto print_section_table() -> void;

    static auto generate_txt_file(std::string file_name) -> void;
    static auto serialize(std::string file_name) -> void;

};


#endif
