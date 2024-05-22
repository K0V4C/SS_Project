/*
 *      =======
 *      +LABEL+
 *      =======
 */

#include "../../inc/asm_actions/label_actions.hpp"
#include "../../inc/asembler.hpp"
#include <stdexcept>

label::label(std::string name) : symbol_name(name){}

auto label::execute() -> void {
    
    // Check for redefinition
    // TODO: check if it is good

    auto it = Asembler::symbol_table.find(symbol_name);
    
    auto current_section_idx = Asembler::section_table[Asembler::current_section].section_idx;

    // Check if already in table
    if(it != Asembler::symbol_table.end()) {

        auto check = Asembler::symbol_table[symbol_name].ndx;

        if(check) {
            throw std::runtime_error(
                std::string("Symbol redefinition: already defined @")
                + std::to_string(it->second.value) 
            );
        }

        auto& symbol = Asembler::symbol_table[symbol_name];

        // TODO: echck if this is correct
        symbol.value = Asembler::section_counter;
        symbol.ndx   = current_section_idx;
        return;
    }
    
    // Create new symbol
    Asembler::symbol_table[symbol_name] = {
        Asembler::section_counter,
        0,
        SYMBOL_TYPE::NOTYP,
        SYMBOL_BIND::LOCAL,
        current_section_idx,
        symbol_name
    };

}

label::~label() {}




