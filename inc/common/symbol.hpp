#ifndef _SYMBOL_HPP
#define _SYMBOL_HPP

#include "types.hpp"

#include <string>
#include <cstdint>

// Classic for symbol table
    struct symbol_struct {
        
        static int32_t next_symbol_idx;

        // Auto get idx
        symbol_struct(      uint32_t        idx, 
                            uint32_t        value,
                            int32_t         size,
                            SYMBOL_TYPE     symbol_type,
                            SYMBOL_BIND     symbol_bind,
                            int32_t         ndx,
                            std::string     symbol_name 
                            ); 

       // Everything by yourself 
        symbol_struct(){};

        uint32_t  symbol_idx;
        uint32_t value;
        int32_t  size;
        SYMBOL_TYPE symbol_type;
        SYMBOL_BIND symbol_bind;
        int32_t ndx;
        std::string symbol_name;

        friend std::ostream& operator<<(std::ostream&, const symbol_struct& obj);

        auto serialize(std::ofstream& binary_file) -> void;
        static auto deserialize(std::ifstream& binary_file) -> void;

    };

    #endif