#include "../../inc/asembler/asembler.hpp"
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <map>
#include <memory>
#include <stdexcept>
#include <sys/types.h>
#include <variant>
#include <vector>
#include <iomanip>
#include <fstream>
#include <set>
#include <cmath>

std::vector<std::unique_ptr<action>> Asembler::file_actions = std::vector<std::unique_ptr<action>>();

std::map<std::string, std::vector<sign_and_symbol>> Asembler::equ_table = std::map<std::string, std::vector<sign_and_symbol>>();

std::map<std::string, symbol_struct> Asembler::symbol_table = std::map<std::string, symbol_struct>();

/*
 *
 *
 *          BASE INIT FOR ASEMBLER CLASS
 *
 *
 *
 */

Asembler::Asembler() {
    // Init first entry in symbol_table
    Asembler::symbol_table["NULL"] = {
        Asembler::next_symbol_idx++,
        0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
    };

    // Reserve first section
    Asembler::section_table["NULL"] = {
        Asembler::next_section_idx++,
        "NULL"
    };

}

auto Asembler::asemble() -> void {

    auto& all_actions = Asembler::file_actions;

    for(auto& action : all_actions ) {
        action->execute();
    }
}

auto Asembler::symbol_exists(std::string symbol_name) -> bool {

    for(auto& symbol_pair : symbol_table) {

        auto& symbol = symbol_pair.second;

        if(symbol.symbol_name == symbol_name) return true;
    }

    return false;
}

auto Asembler::symbol_is_defined(std::string symbol_name) -> bool {
    
    if(symbol_exists(symbol_name) and symbol_table[symbol_name].ndx != 0) return true;
    
    return false;
    
}

auto Asembler::get_symbol(std::string symbol_name) -> symbol_struct {

    for(auto& symbol_pair : symbol_table) {

           auto& symbol = symbol_pair.second;

           if(symbol.symbol_name == symbol_name) {
               return symbol;
           }
    }

    return {
        0, 0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
    };
}

auto Asembler::section_exists(uint32_t ndx) -> bool {

    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        if(section.section_idx == ndx) return true;
    }

    return false;
}

auto Asembler::get_section(uint32_t ndx) -> section_struct {
    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        if(section.section_idx == ndx) return section;
    }

    return {
        0, "NULL"
    };
}

auto Asembler::get_section_idx(std::string section_name) -> uint32_t {
    
    if(Asembler::symbol_exists(section_name) == false) {
        throw std::runtime_error("get_section_idx : section does not exist");
    }
    
    
    for(auto& sec : Asembler::section_table) {
        if(sec.first == section_name) {
            return sec.second.section_idx;
        }
    }
    
    return 0;
}

auto Asembler::get_section_name(uint32_t ndx) -> std::string {
    
    std::string section_name = "NULL";
    
    for(auto& section : Asembler::section_table) {
        
        if(section.second.section_idx == ndx) {
            section_name = section.first;
            break;
        }
        
    }    
    
    return section_name;
}

auto Asembler::get_symbol_value(std::string sym) -> u_int32_t {
    
    if(Asembler::symbol_exists(sym) == false) {
        throw std::runtime_error(" get_symbol_value : symbol not existant : " + sym);
    }
    
    
    return Asembler::symbol_table[sym].value;
}

auto Asembler::is_calculatable(std::vector<sign_and_symbol> & equ_entry) -> bool {
    
    std::map<std::string, int32_t> section_counter; // If symbol is defined and with sign + -> + 1
                                                     // if symbol is defined and with sign - -> - 1
                                                     // if symbol is define and it is ABS    -> + 0
                                                     // if it is not defined skip it (can be later defined by equ) -> return false
    
    for(uint32_t i = 0; i < equ_entry.size(); i+= 1) {
        
        std::variant<std::string, int32_t>& symbol_or_literal = equ_entry[i].second;
        
        if(std::holds_alternative<int32_t>(symbol_or_literal)) {
            continue;
        }
        
        std::string symbol_name  = std::get<std::string>(symbol_or_literal);
        
        // Check if it is defined if it is not return false
        if(!Asembler::symbol_is_defined(symbol_name)) {
            return false;
        }
        
        // Find the section name
        
        uint32_t ndx = Asembler::symbol_table[symbol_name].ndx;
        
        std::string section_name = get_section_name(ndx);
        
        if(section_name == "ABS") {
            continue;
        }
        
        int32_t sign = equ_entry[i].first == "+" ? 1 : -1;
        
        section_counter[section_name] += sign;
    
    }
    
    for(auto& _entry : section_counter ) {
        
        auto cnt = _entry.second;
        
        if(cnt != 0) {
            return false;
        }
        
    }
    
    return true;
}


auto Asembler::backpatch() -> void {
    
    // Resolve equ symbols
    
    bool change = equ_table.size() > 0;
    
    std::set<std::string> fixed_equ_symbols;
    
    for(auto e : equ_table) {
        fixed_equ_symbols.insert(e.first);
    }
    
    while(change) {
        
        change = !change;
        
        auto table_copy = equ_table;
        
        for(auto& _equ_entry : equ_table) {
            
            // Try to resolve one entry
            
            auto& symbol = _equ_entry.first;
            auto& equ_entry = _equ_entry.second;
            
            // First find out if it is calculatable
            
            if(Asembler::is_calculatable(equ_entry) == false) {
                continue;
            }
            
            // This entry is calculatable so we should update it
            
            int32_t total = 0;
            
            for(uint32_t i = 0; i < equ_entry.size(); i += 1) {
                
                int32_t sign = equ_entry[i].first == "+" ? 1 : -1;
                
                std::variant<std::string, int32_t> sym_or_lit = equ_entry[i].second;

                if(std::holds_alternative<int32_t>(sym_or_lit)) {
                    total += std::get<int32_t>(sym_or_lit) * sign;
                    continue;
                }
                
                std::string symbol_name = std::get<std::string>(sym_or_lit);
                
                uint32_t symbol_value = Asembler::get_symbol_value(symbol_name) * sign;
                
                total += symbol_value;
                
            }
            
            // As we finnaly have symbol value 
            
            table_copy.erase(symbol);
            uint32_t abs_idx = Asembler::get_section_idx("ABS");
            
            Asembler::symbol_table[symbol].value = total;
            Asembler::symbol_table[symbol].ndx   = abs_idx;
            
        }
        
        if(table_copy.size() != equ_table.size()) {
            change = true;
        }
        
        equ_table = table_copy;
        
    }
    
    if(equ_table.size() > 0) {
        
        for(auto e : equ_table) {
            std::cout << e.first << std::endl;
        }
        
        throw std::runtime_error("equ symbols couldn't be resovled! : " + std::to_string(equ_table.size()));
    }
    
    // Now we resolve relocations and additional strucutres
    
    for(auto& section_pair : section_table) {
        
        auto& section = section_pair.second;
        
        for(auto it = section.relocations.begin(); it != section.relocations.end();) {
            
            auto& relocation = *it;
                   
            if(relocation.type == RELOCATION_TYPE::EQU_FILE_LOCAL) {
                
                if(std::abs(static_cast<int32_t>(get_symbol_value(relocation.symbol_name))) > std::pow(2,12)) {
                    throw std::runtime_error("symbol can't be placed inside 12bits");
                }
                
                if(fixed_equ_symbols.find(relocation.symbol_name) == fixed_equ_symbols.end()) {
                    throw std::runtime_error("backpatching_err: symbol value not know at asm time!");
                }
                
                // Fix instruction
                
                uint32_t symbol_value = get_symbol_value(relocation.symbol_name);
                
                uint8_t first_nibble  = (symbol_value    & 0xf00) >> 8;
                uint8_t second_nibble = (symbol_value    & 0x0f0) >> 0;
                uint8_t third_nibble  = symbol_value    & 0x00f;
                
                uint32_t offset = relocation.offset;
                
                // this should fix instruction itself
            
                section.binary_data.raw[offset + 0] |= second_nibble | third_nibble;
                section.binary_data.raw[offset + 1] |= first_nibble;
                
                it = section.relocations.erase(it);
                continue;
                
                
            } else if (relocation.type == RELOCATION_TYPE::ABS32) {
                
                if(fixed_equ_symbols.find(relocation.symbol_name) == fixed_equ_symbols.end()) {
                    ++it;
                    continue;
                }
                
                // fix the pool
                
                uint32_t symbol_value = get_symbol_value(relocation.symbol_name);
                
                uint32_t offset = relocation.offset;
                
                section.binary_data.raw[offset + 0] = (symbol_value & 0x000000ff)   >> 0; 
                section.binary_data.raw[offset + 1] = (symbol_value & 0x0000ff00)   >> 8; 
                section.binary_data.raw[offset + 2] = (symbol_value & 0x00ff0000)   >> 16; 
                section.binary_data.raw[offset + 3] = (symbol_value & 0xff000000)   >> 24; 
                
                // Remove relocations
                
                it = section.relocations.erase(it);
                continue;
                
            }
            
            std::cerr << "backpatching: We shouldn't ever get to here!";
        }
    }
    
    // Check if global errors
    
    for(auto& symbol : symbol_table) {
        
        // This is how it sohuld work
        // if(symbol.second.symbol_bind == SYMBOL_BIND::GLOBAL and symbol.second.ndx == 0) {
        //     throw std::runtime_error("global symbol not resolved");
        // }
        
        // This is how Micko said to do it
        if(symbol.second.symbol_bind == SYMBOL_BIND::GLOBAL and symbol.second.ndx == 0) {
            symbol.second.symbol_bind = SYMBOL_BIND::EXTERN;
        }
        
    }
    
    
    // Prepare relocations

    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        for(auto& relocation : section.relocations) {
            
            if(!symbol_exists(relocation.symbol_name)) {
                
                std::cout << relocation.symbol_name;
                throw std::runtime_error(" Backpatching relocations symbol does not exits");
            }

            auto symbol = get_symbol(relocation.symbol_name);

            if( symbol.symbol_bind == SYMBOL_BIND::GLOBAL or symbol.symbol_bind == SYMBOL_BIND::EXTERN) {
                continue;
            }

            if(symbol.symbol_bind == SYMBOL_BIND::LOCAL and symbol.ndx == 0) {
                throw std::runtime_error("Backpatching Undefined symbol");
            }

            if(!section_exists(symbol.ndx)) {
                throw std::runtime_error(" Backpatching relocations section does not exist");
            }

            auto section = get_section(symbol.ndx);

            relocation.symbol_name = section.name;
            relocation.addend = symbol.value;
        }
    }
}

auto Asembler::generate_txt_file(std::string file_name) -> void {
    
    file_name.pop_back();
    
    file_name += "txt";
    
    std::ofstream txt_file(file_name, std::ios::out);
    
    // Symbol table
    // 

    txt_file << "Symbol table:\n";

    {
        txt_file   << std::setw(5)     << std::left << "idx"   << " | "
                    << std::setw(20)    << std::left << "value" << " | "
                    << std::setw(5)     << std::left << "size"  << " | "
                    << std::setw(10)    << std::left << "type"  << " | "
                    << std::setw(10)    << std::left << "bind"  << " | "
                    << std::setw(5)     << std::left << "ndx"   << " | "
                    << std::setw(36)    << std::left << "name"  << " |\n";

    }

    for(auto& e: Asembler::symbol_table) {
        txt_file << e.second << std::endl;
    }
    
    txt_file << std::endl;
    
    for(int i = 0 ; i < 111; i+=1) {
        txt_file << "=";
    }
    txt_file << std::endl;
    
    
    // Section + Relocation table for that section
    
    txt_file << "\n\nSection Table:\n";
    {
        txt_file << std::setw(40) << std::left    << "section name"        << " | "
                  << std::setw(20) << std::left    << "section idx "        << " | "
                  << std::setw(43) << std::left    << "Rel Under Section"   << " |\n";
    }

    {
        for(auto& e : Asembler::section_table) {
            txt_file << e.second << std::endl;
        }
    }
    
    txt_file << std::endl;
    
    for(int i = 0 ; i < 111; i+=1) {
        txt_file << "=";
    }
    txt_file << std::endl;
}

auto Asembler::serialize(std::string file_name) -> void {

    std::ofstream binary_file(file_name, std::ios::out | std::ios::binary);
    const uint32_t symbol_table_size = symbol_table.size() - 1; // -1 for the NULL
    binary_file.write(reinterpret_cast<const char*>(&symbol_table_size), sizeof(symbol_table_size));

    for(auto e : symbol_table) {
         if(e.second.symbol_name == "NULL") continue;
        e.second.serialize(binary_file);
    }

    const uint32_t section_table_size = section_table.size() - 1; // -1 for te NULL
    binary_file.write(reinterpret_cast<const char*>(&section_table_size), sizeof(section_table_size));

    for(auto e : section_table) {
        if(e.second.name == "NULL") continue;
        e.second.serialize(binary_file);
    }

}

Asembler::~Asembler() {}

/*
 *
 *      SECTION TABLE
 *
 *
 *
 */

std::string Asembler::current_section = "";

uint32_t Asembler::next_section_idx = 0;

std::map<std::string, section_struct> Asembler::section_table = std::map<std::string, section_struct>();

auto Asembler::get_section_counter() -> uint32_t {
    if (Asembler::current_section == "") return 0;

    return Asembler::section_table[Asembler::current_section].binary_data.raw.size();
}

auto Asembler::get_current_section() -> section_struct& {
    return Asembler::section_table[Asembler::current_section];
}


auto Asembler::print_section_table() -> void {

    std::cout << "\n\nSection Table:\n";
    {
        std::cout << std::setw(40) << std::left    << "section name"        << " | "
                  << std::setw(20) << std::left    << "section idx "        << " | "
                  << std::setw(43) << std::left    << "Rel Under Section"   << " |\n";
    }

    {
        for(auto& e : Asembler::section_table) {
            std::cout << e.second << std::endl;
        }
    }
};

/*

    SYMBOL TABLE

*/

uint32_t Asembler::next_symbol_idx = 0;

auto Asembler::print_symbol_table() -> void {

    std::cout << "Symbol table:\n";

    {
        std::cout   << std::setw(5)     << std::left << "idx"   << " | "
                    << std::setw(20)    << std::left << "value" << " | "
                    << std::setw(5)     << std::left << "size"  << " | "
                    << std::setw(10)    << std::left << "type"  << " | "
                    << std::setw(10)    << std::left << "bind"  << " | "
                    << std::setw(5)     << std::left << "ndx"   << " | "
                    << std::setw(36)    << std::left << "name"  << " |\n";

    }

    for(auto& e: Asembler::symbol_table) {
        std::cout << e.second << std::endl;
    }

}
