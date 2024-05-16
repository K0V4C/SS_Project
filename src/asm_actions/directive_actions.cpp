
/*
 *      ============
 *      +DIRECTIVES+
 *      ============
 */

#include "../../inc/asm_actions/directive_actions.hpp"
#include <iostream>

// EXTERN 
// This should init struct needed for extern
directive_extern::directive_extern(std::vector<std::string> symbol_list)
    : symbol_list(symbol_list) {}

auto directive_extern::execute() -> void {
    std::cout << "EXTERN BABY!" << std::endl;
}

directive_extern::~directive_extern(){}
