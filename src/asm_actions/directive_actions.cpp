
/*
 *      ============
 *      +DIRECTIVES+
 *      ============
 */

#include "../../inc/asm_actions/directive_actions.hpp"
#include "../../inc/asembler.hpp"
#include <cstdint>

// EXTERN 
// This should init struct needed for extern
directive_global_and_extern::directive_global_and_extern(std::vector<std::string> symbol_list)
    : symbol_list(symbol_list) {}

auto directive_global_and_extern::execute() -> void {
    Asembler::symbol_table["EE"] = {};
    // TODO

} 

directive_global_and_extern::~directive_global_and_extern(){}

// SECTION 

directive_section::directive_section(std::string section_name)
    : section_name(section_name){}

auto directive_section::execute() -> void {
    
    int32_t next_idx = Asembler::section_table.size() + 1;

    Asembler::section_table[section_name] = {section_name, 
                                            next_idx,
                                            std::vector<Asembler::relocation_struct>(),
                                            Asembler::literal_pool()
                                            };
}

directive_section::~directive_section() {}

// SKIP 

directive_skip::directive_skip(int32_t skip_leap)
    : skip_leap(skip_leap){}

auto directive_skip::execute() -> void {
    // TODO:
}

directive_skip::~directive_skip(){}

