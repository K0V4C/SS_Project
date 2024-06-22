
/*
 *      ============
 *      +DIRECTIVES+
 *      ============
 */

#include "../../../inc/asembler/asm_actions/directive_actions.hpp"
#include "../../../inc/asembler/asembler.hpp"
#include <cstdint>
#include <stdexcept>
#include <variant>
#include <algorithm>

//
//
//      GLOBAL AND EXTERN
//
//

// This should init struct needed for extern
directive_global::directive_global(std::vector<std::string> symbol_list)
    : symbol_list(symbol_list) {}

auto directive_global::execute() -> void {   
    for(auto& symbol : this->symbol_list) {
        auto it = Asembler::symbol_table.find(symbol);

        // If it exists just set its bind to symbol_bind 
        if(it != Asembler::symbol_table.end()) { 
            
            Asembler::symbol_table[symbol].symbol_bind = SYMBOL_BIND::GLOBAL;

        } else {
            
            Asembler::symbol_table[symbol] = {
                Asembler::next_symbol_idx++,
                Asembler::get_section_counter(),
                0,
                SYMBOL_TYPE::NOTYP,
                SYMBOL_BIND::GLOBAL,
                0,
                symbol
            };
        }
    }

} 

directive_global::~directive_global(){}

// This should init struct needed for extern
directive_extern::directive_extern(std::vector<std::string> symbol_list)
    : symbol_list(symbol_list) {}

auto directive_extern::execute() -> void {   
    for(auto& symbol : this->symbol_list) {
        auto it = Asembler::symbol_table.find(symbol);

        // If it exists just set its bind to symbol_bind 
        if(it != Asembler::symbol_table.end()) { 
            
            Asembler::symbol_table[symbol].symbol_bind = SYMBOL_BIND::EXTERN;

        } else {
            
            Asembler::symbol_table[symbol] = {
                Asembler::next_symbol_idx++,
                Asembler::get_section_counter(),
                0,
                SYMBOL_TYPE::NOTYP,
                SYMBOL_BIND::EXTERN,
                0,
                symbol
            };
        }
    }

} 

directive_extern::~directive_extern(){}

// 
//
//         SECTION DIRECTIVE
//
//

directive_section::directive_section(std::string section_name)
    : section_name(section_name){}

auto directive_section::execute() -> void {
     
    auto it = Asembler::section_table.find(this->section_name);

    if(it != Asembler::section_table.end()) {
        throw std::runtime_error("Section already defined!");
    }

    Asembler::section_table[section_name] = {
        Asembler::next_section_idx++,
        section_name // empty relocation table at the start 
    };

    Asembler::current_section = section_name;

    // Also add it as a symbol

    auto it_s = Asembler::symbol_table.find(this->section_name);

        if(it_s != Asembler::symbol_table.end()) {
        throw std::runtime_error("Invalid section name: Symbol already defined!");
    }

     Asembler::symbol_table[section_name] = {
        Asembler::next_symbol_idx++,
        0,
        0,
        SYMBOL_TYPE::SCTN,
        SYMBOL_BIND::LOCAL,
        Asembler::section_table[section_name].section_idx,
        section_name
    };


}

directive_section::~directive_section() {}

// 
//
//              SKIP
//
//

directive_skip::directive_skip(int32_t skip_leap)
    : skip_leap(skip_leap){}

auto directive_skip::execute() -> void {

    // TODO: What is ship is 0?
    for(int i = 0; i < this->skip_leap; i++) {
        Asembler::section_table[Asembler::current_section].binary_data.add_byte(0);
    }
}

directive_skip::~directive_skip(){}

//
//
//       WORD
//
//

directive_word::directive_word(std::vector<std::variant<std::string, int32_t>> symbol_list)
    : symbols_and_literals(symbol_list){}

auto directive_word::execute() -> void {

    auto& section = Asembler::section_table[Asembler::current_section];
    
    for(auto& element : symbols_and_literals) { 
        if(std::holds_alternative<std::string>(element)) {
            // TODO: leave relocation data in for this place
            // TODO: check if it already maybe defined
            section.add_relocation(
                    Asembler::get_section_counter(),
                    RELOCATION_TYPE::ABS32,
                    std::get<std::string>(element),
                    0
            ); 
            section.binary_data.add_instruction(0);
        } else {
            // TODO: should I consider leaving relocation data in here?
            section.binary_data.add_instruction(
                static_cast<uint32_t>(
                    std::get<int32_t>(element)
                )
            );
        }
    }
}

directive_word::~directive_word(){}

//
//
//      ASCII
//
//
directive_ascii::directive_ascii(std::string str)
    : ascii_string(str) {}

auto directive_ascii::execute() -> void {
    
    // remove "
    ascii_string.erase(remove(ascii_string.begin(), ascii_string.end(), '"'), ascii_string.end()); 
    
    auto& section = Asembler::get_current_section();
    for(const auto character : ascii_string) {
        section.binary_data.add_byte(
            static_cast<char>(character)
        );
    }

    section.binary_data.add_byte(
        static_cast<char>('\0')
    );

}

directive_ascii::~directive_ascii(){}


//
//
//      EQU
//
//
directive_equ::directive_equ(std::vector<std::variant<std::string,int32_t>> list)
    : symbols_and_literals(list) {}

auto directive_equ::execute() -> void {
    // TODO:
}

directive_equ::~directive_equ(){}

//
//
//         END 
//
//

directive_end::directive_end(){}

auto directive_end::execute() -> void {
    throw std::runtime_error("END OF THE FILE HIT");
}

directive_end::~directive_end(){}



