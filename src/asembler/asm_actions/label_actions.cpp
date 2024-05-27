/*
 *      =======
 *      +LABEL+
 *      =======
 */

#include "inc/asembler/asm_actions/label_actions.hpp"
#include "inc/asembler/asembler.hpp"
#include <stdexcept>

label::label(std::string name) : symbol_name(name){}

auto label::execute() -> void {
    
    // Check for redefinition
    // TODO: check if it is good

    auto it = Asembler::symbol_table.find(symbol_name);
    
    auto& section = Asembler::get_current_section();

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
        symbol.value = Asembler::get_section_counter();
        symbol.ndx   = section.section_idx;
        return;
    }
    
    // Create new symbol
    Asembler::symbol_table[symbol_name] = {
        Asembler::next_symbol_idx++,
        Asembler::get_section_counter(),
        0,
        SYMBOL_TYPE::NOTYP,
        SYMBOL_BIND::LOCAL,
        section.section_idx,
        symbol_name
    };

}

label::~label() {}




