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
)-> uint32_t {
    
    // TODO: fix this

    uint32_t to_ret = 0;

    uint32_t nibble_little_endian = 0                     |
                                    ((displ & 0xf) << 8)  | 
                                    ((displ & 0xf0) << 12)|
                                    ((displ & 0xf00) >>8) ;

    to_ret =    (op_code & 0xf) << 28               |
                (mode & 0xf)    << 24               |
                (A    & 0xf)    << 20               |
                (B    & 0xf)    << 16               | 
                (C    & 0xf)    << 12               |
                (nibble_little_endian & 0xfff);

    return to_ret;

}

// 
//
//      INSTRUCTION HALT
//
//

instruction_halt::instruction_halt(){}

auto instruction_halt::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_halt::op_code,
                instruction_halt::mode,
                0,
                0,
                0,
                0
            )
    );
 
}

instruction_halt::~instruction_halt() {}

//
//
//      INSTRUCTION XCHG
//
//

instruction_xchg::instruction_xchg(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_xchg::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_xchg::op_code,
                instruction_xchg::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_xchg::~instruction_xchg() {}

//
//
//      INSTRUCTION ADD
//
//

instruction_add::instruction_add(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_add::execute() -> void {
   
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
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

//
//
//      INSTRUCTION SUB
//
//

instruction_sub::instruction_sub(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_sub::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_sub::op_code,
                instruction_sub::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_sub::~instruction_sub() {}

//
//
//      INSTRUCTION MUL
//
//

instruction_mul::instruction_mul(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_mul::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_mul::op_code,
                instruction_mul::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_mul::~instruction_mul() {}

//
//
//      INSTRUCTION DIV
//
//

instruction_div::instruction_div(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_div::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_div::op_code,
                instruction_div::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );
 
}

instruction_div::~instruction_div(){}

//
//
//      INSTRUCTION NOT
//
//

instruction_not::instruction_not(uint8_t gpr_s)
    : gpr_s(gpr_s){}

auto instruction_not::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_not::op_code,
                instruction_not::mode,
                gpr_s,
                gpr_s,
                0, // C does not exist
                0
            )
    );
  
}

instruction_not::~instruction_not() {}

//
//
//      INSTRUCTION AND
//
//

instruction_and::instruction_and(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_and::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_and::op_code,
                instruction_and::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );
 
}

instruction_and::~instruction_and() {}

//
//
//      INSTRUCTION OR
//
//

instruction_or::instruction_or(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_or::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_or::op_code,
                instruction_or::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );
 
}

instruction_or::~instruction_or() {}

//
//
//      INSTRUCTION XOR
//
//

instruction_xor::instruction_xor(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_xor::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_xor::op_code,
                instruction_xor::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_xor::~instruction_xor() {}

//
//
//      INSTRUCTION SHL
//
//

instruction_shl::instruction_shl(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_shl::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_shl::op_code,
                instruction_shl::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_shl::~instruction_shl() {}

//
//
//      INSTRUCTION SHR
//
//

instruction_shr::instruction_shr(uint8_t gpr_s, uint8_t gpr_d)
    : gpr_s(gpr_s), gpr_d(gpr_d){}

auto instruction_shr::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_shr::op_code,
                instruction_shr::mode,
                gpr_d,
                gpr_d,
                gpr_s,
                0
            )
    );

}

instruction_shr::~instruction_shr() {}


//
//
//      INSTRUCTION CSSRD
//
//

instruction_csrrd::instruction_csrrd(uint8_t gpr_d)
    : gpr_d(gpr_d){}

auto instruction_csrrd::execute() -> void {
    // TODO:
}

instruction_csrrd::~instruction_csrrd() {}

//
//
//      INSTRUCTION csrwr
//

instruction_csrwr::instruction_csrwr(uint8_t gpr_d)
    : gpr_d(gpr_d){}

auto instruction_csrwr::execute() -> void {
    // TODO:
}

instruction_csrwr::~instruction_csrwr() {}
