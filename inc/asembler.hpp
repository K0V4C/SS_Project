#ifndef _ASEMBLER_HPP
#define _ASEMBLER_HPP

#include "asm_actions/actions.hpp"
#include "types.hpp"

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

    ~Asembler();

    // Used to do instructions and also directives
    static std::vector<std::unique_ptr<action>> file_actions;
    
    // Classic for symbol table
    struct symbol_struct {
        
        static int32_t next_symbol_idx;

        // Auto get idx
        symbol_struct(      uint32_t         value,
                            int32_t         size,
                            SYMBOL_TYPE     symbol_type,
                            SYMBOL_BIND     symbol_bind,
                            int32_t         ndx,
                            std::string     symbol_name 
                            ); 

       // Everything by yourself 
        symbol_struct(){};

        int32_t  symbol_idx;
        uint32_t value;
        int32_t  size;
        SYMBOL_TYPE symbol_type;
        SYMBOL_BIND symbol_bind;
        int32_t ndx;
        std::string symbol_name;

        friend std::ostream& operator<<(std::ostream&, const symbol_struct& obj);

    };

    static std::map<std::string, Asembler::symbol_struct> symbol_table;

    static auto print_symbol_table() -> void;

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

    };

    // There is no need for one singular relocation table becaouse we can just append it to every single one 
    
    struct binary_data {
        
        binary_data(){}
        
        auto add_instruction(uint32_t) -> void;

        auto add_byte(uint8_t) -> void;

        std::vector<uint8_t> raw; 
    };


    // Used to store values coresponding to sections
    struct section_struct {

        static int32_t next_section_idx;

        section_struct(){};

        section_struct(std::string name);

        auto add_realocation(uint32_t, RELOCATION_TYPE, std::string, int32_t)->void;

        std::string name;
        int32_t section_idx; 
        std::vector<relocation_struct> relocations;
        Asembler::binary_data binary_data;

        friend std::ostream& operator<<(std::ostream& os, const section_struct& obj);

    };

    // Used to help find values
    static auto get_section_counter() -> uint32_t;
    static auto get_current_section() -> section_struct&;
    // Used to figure out current_section
    static std::string current_section;

    static std::map<std::string, Asembler::section_struct> section_table;


    static auto print_section_table() -> void;
};


#endif
