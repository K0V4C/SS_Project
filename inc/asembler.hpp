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

    // Used to figure out address
    static int32_t line_counter;

    // Used to do instructions and also directives
    static std::vector<std::unique_ptr<action>> file_actions;
    
    // Classic for symbol table
    struct symbol_struct {

        symbol_struct(      int32_t,
                            int32_t,
                            int32_t,
                            SYMBOL_TYPE,
                            SYMBOL_BIND,
                            int32_t,
                            std::string 
                            );
        
        symbol_struct(){};

        int32_t symbol_idx;
        int32_t value;
        int32_t size;
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

        relocation_struct(){};

        relocation_struct(  int32_t,
                            RELOCATION_TYPE,
                            std::string,
                            int32_t
                            );

        int32_t offset;
        RELOCATION_TYPE type;
        std::string symbol_name; // maybe use it to find idx?
        int32_t addend;

        friend std::ostream& operator<<(std::ostream& os, const relocation_struct& obj);

    };

    // There is no need for one singular relocation table becaouse we can just append it to every single one
    
    struct literal_pool {
        std::vector<int32_t> data;
    };

    // Used to store values coresponding to sections
    struct section_struct {

        section_struct(){};

        section_struct( std::string                         name,
                        int32_t                             section_idx,
                        std::vector<relocation_struct>      relocations,
                        literal_pool                        pool
                        );

        std::string name;
        int32_t section_idx; 
        std::vector<relocation_struct> relocations;
        literal_pool pool;

        friend std::ostream& operator<<(std::ostream& os, const section_struct& obj);

    };

    static std::map<std::string, Asembler::section_struct> section_table;


    static auto print_section_table() -> void;
};


#endif
