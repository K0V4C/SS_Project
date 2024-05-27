#include "../../inc/asembler/collector.hpp"

Collector* Collector::instance = nullptr;

Collector::Collector() {}

Collector::~Collector() {}


auto Collector::get_instance() -> Collector& {
    
    if(instance == nullptr)
        instance = new Collector();
    
    return *instance;
}

/*
 *
 *
 *      All the work for symbols and literals
 *
 *
 */

auto Collector::add_symbol_or_literal(sym_or_lit _new) -> void {
    
    this->symbols_and_literals.push_back(_new);

}

auto Collector::get_symbols_and_literals() -> std::vector<sym_or_lit> {
return this->symbols_and_literals;
}

auto Collector::clear_symbols_and_literals() -> void {
    this->symbols_and_literals.clear();
}

/*
 *
 *    WORKING WITH GPRS
 *
 */

auto Collector::append_gpr(uint8_t new_gpr_reg) -> void {
    this->gpr_regs.push_back(new_gpr_reg);
}

auto Collector::get_gpr_list() -> std::vector<uint8_t>  {
    return this->gpr_regs;
}

auto Collector::clear_gpr_list() -> void { this->gpr_regs.clear(); }

