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
#include <variant>
#include <vector>

typedef std::pair<std::string, std::variant<std::string, int32_t>> sign_and_symbol;

class Asembler {
public:

    Asembler();

    auto asemble() -> void;
    auto backpatch() -> void;

    static auto symbol_exists(std::string) -> bool;
    static auto symbol_is_defined(std::string) -> bool;
    static auto get_symbol(std::string) -> symbol_struct;
    static auto get_symbol_value(std::string sym) -> uint32_t;
    
    static auto section_exists(uint32_t) -> bool;
    static auto get_section(uint32_t) -> section_struct;
    static auto get_section_idx(std::string) -> uint32_t;
    static auto get_section_name(uint32_t ndx) -> std::string;


    ~Asembler();

    // Used to do instructions and also directives
    static std::vector<std::unique_ptr<action>> file_actions;
    
    // Structures for equ directive
    
    static std::map<std::string, std::vector<sign_and_symbol>> equ_table;
    
    static auto is_calculatable(std::vector<sign_and_symbol>&) -> bool;

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
