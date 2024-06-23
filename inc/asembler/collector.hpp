#ifndef _COLLECTOR_HPP
#define _COLLECTOR_HPP

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

using sym_or_lit = std::variant<std::string, int32_t> ;

class Collector {
public:
 
    static auto get_instance() -> Collector&;

    auto append_gpr(uint8_t) -> void;
    auto get_gpr_list() -> std::vector<uint8_t>;
    auto clear_gpr_list() -> void;

    auto add_symbol_or_literal(sym_or_lit) -> void;
    auto get_symbols_and_literals() -> std::vector<sym_or_lit>;
    auto clear_symbols_and_literals() -> void;
    
    auto append_operation(std::string) ->void;
    auto get_operation_list() -> std::vector<std::string>&;
    auto clear_operations() -> void;


    Collector(Collector&) = delete;
    void operator=(Collector&) = delete;

    ~Collector();
protected:
    
    static Collector* instance;

    sym_or_lit branch_operand;

    int32_t literal;

    uint8_t csr_reg; // TODO:
    std::string string_token; // TODO:
                             
    std::vector<uint8_t> gpr_regs;
    std::vector<sym_or_lit> symbols_and_literals;
    std::vector<std::string> operations;


    Collector();

};

#endif // _COLLECTOR_HPP
