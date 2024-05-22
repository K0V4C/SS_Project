/*
 *      ==============
 *      +INSTRUCTIONS+
 *      ==============
 */

#include "../../inc/asm_actions/instruction_actions.hpp"
#include "../../inc/asembler.hpp"

auto combine
( 
    uint8_t op_code,
    uint8_t mode,
    uint8_t A,
    uint8_t B,
    uint8_t C,
    int32_t displ
)-> int32_t {

    uint32_t to_ret = 0;

    to_ret =    (op_code & 0xf) << 28 |
                (mode & 0xf)    << 24 |
                (A    & 0xf)    << 20 |
                (B    & 0xf)    << 16 | 
                (C    & 0xf)    << 12 |
                (displ & 0xfff);

    return to_ret;

}
//
//
//      INSTRUCTION ADD
//
//

instruction_add::instruction_add(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_add::execute() -> void {
   
    auto& section = Asembler::section_table[Asembler::current_section];

    section.binary_data.raw.push_back(
        combine(
                instruction_add::op_code,
                instruction_add::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_add::~instruction_add() {}


