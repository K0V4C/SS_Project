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


    //
    //
    //  FORWARD TABLE
    //
    //

    struct forward_struct {

        forward_struct();
        ~forward_struct();

        std::string symbol_name;

        std::vector<uint32_t> locations;

        friend std::ostream& operator<<(std::ostream& os, const forward_struct& obj);
    };

    static auto link_symbol(std::string, uint32_t location) -> void;
    static std::map<std::string, forward_struct> forward_table;
    static auto print_forward_table() -> void;

    // Used to help find values
    static uint32_t next_section_idx;

    static auto get_section_counter() -> uint32_t;
    static auto get_current_section() -> section_struct&;
    static std::string current_section;
    static std::map<std::string, section_struct> section_table;
    static auto print_section_table() -> void;


    static auto serialize(std::string file_name) -> void;

};


#endif
