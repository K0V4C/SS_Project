/*
 *      ==============
 *      +INSTRUCTIONS+
 *      ==============
 */

#include "inc/asembler/asm_actions/instruction_actions.hpp"
#include "inc/asembler/asembler.hpp"

#include <cmath>

auto combine
( 
    uint8_t op_code,
    uint8_t mode,
    uint8_t A,
    uint8_t B,
    uint8_t C,
    int32_t displ
)-> uint32_t {
    
    // TODO: figure out if this works
    // This hould store it in big endina into to_ret and then writing it to mem in little endian

    uint32_t to_ret = 0;

    uint32_t nibble_little_endian = 0                     |
                                    ((displ & 0xf))       | 
                                    ((displ & 0xf0))      |
                                    ((displ & 0xf00))     ;


    to_ret =    (op_code & 0xf) << 28               |
                (mode & 0xf)    << 24               |
                (A    & 0xf)    << 20               |
                (B    & 0xf)    << 16               | 
                (C    & 0xf)    << 12               |
                (nibble_little_endian & 0xfff);


    return to_ret;
}

auto add_leap() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction (
        combine(
            branch::op_code,
            instruction_jmp::mode_displ,
            static_cast<uint8_t>(REGISTERS::PC),
            0,
            0,
            4
        )
    );
}

auto reserve_4B(uint32_t instr) -> void {

    auto& section = Asembler::get_current_section();
    section.binary_data.add_instruction(instr);
    
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
//      INSTRUCTION INT
//
//

instruction_int::instruction_int(){}

auto instruction_int::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_int::op_code,
                instruction_int::mode,
                0,
                0,
                0,
                0
            )
    );
 
}

instruction_int::~instruction_int() {}

//
//
//      INSTRUCTION IRET
//
//

instruction_iret::instruction_iret(){}

auto instruction_iret::execute() -> void {
   
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_iret::op_code,
                instruction_iret::pop_pc,
                static_cast<uint8_t>(REGISTERS::PC),
                static_cast<uint8_t>(REGISTERS::SP),
                0,
                -4
            )
    );

    section.binary_data.add_instruction(
        combine(
                instruction_iret::op_code,
                instruction_iret::pop_status,
                static_cast<uint8_t>(REGISTERS::STATUS),
                static_cast<uint8_t>(REGISTERS::SP),
                0,
                -4
            )
    );
}

instruction_iret::~instruction_iret() {}

//
//
//      INSTRUCTION CALL
//
//

instruction_call::instruction_call(std::variant<std::string, int32_t> operand)
    : operand(operand){}

auto instruction_call::execute() -> void {

    std::cout << "ovde smo" << std::endl;return;

    auto& section = Asembler::get_current_section();

    uint8_t gpr_b = 0;
    uint8_t gpr_c = static_cast<uint8_t>(REGISTERS::PC);

    // If passed operand is a literal

    if(std::holds_alternative<int32_t>(operand)) {

        auto literal = std::get<int32_t>(operand);

        // This is if literal can be placed inside displacement
        if(literal > std::pow(-2, 11) and literal < std::pow(2, 11)) {
            section.binary_data.add_instruction (
                combine(
                    instruction_call::op_code,
                    instruction_call::mode_displ, 
                    static_cast<uint8_t>(REGISTERS::PC),
                    gpr_b,
                    gpr_c,
                    literal < 0 ? literal | 0x8000 : literal
                )
            );
            return;
        }

        // If it cannot be placed inside displacement

        section.binary_data.add_instruction (
            combine(
                instruction_call::op_code,
                instruction_call::mode_not_displ, 
                static_cast<uint8_t>(REGISTERS::PC),
                gpr_b,
                gpr_c,
                4
            )
        );

        add_leap();
        reserve_4B(literal);
        return;
    }


    // Now the symbol part

    auto _symbol = std::get<std::string>(operand);
    auto it = Asembler::symbol_table.find(_symbol);

    // There is only one case when we can leave displacement for symbol
    // That is is if is defined, in the same section and < 2^12

    if  ( 
            it != Asembler::symbol_table.end() and 
            Asembler::symbol_table[_symbol].ndx == Asembler::get_current_section().section_idx and 
            (Asembler::get_section_counter() - Asembler::symbol_table[_symbol].value) < std::pow(2,12)
        ) {    

        // SYMBOL EXISTS AND IS DEFINED

        section.binary_data.add_instruction (
            combine(
                instruction_call::op_code,
                instruction_call::mode_displ, 
                static_cast<uint8_t>(REGISTERS::PC),
                gpr_b,
                gpr_c,
                (Asembler::symbol_table[_symbol].value - Asembler::get_section_counter()) | 0x8000 // should always be negative?
            )
        );

        return;
    }


    // Symbol not defined, we have to leave reloaciton data

    section.binary_data.add_instruction (
        combine(
            instruction_call::op_code,
            instruction_call::mode_not_displ, 
            static_cast<uint8_t>(REGISTERS::PC),
            gpr_b,
            gpr_c,
            4
        )
    );

    add_leap();
    section.add_relocation(
        Asembler::get_section_counter(),
        RELOCATION_TYPE::ABS32,
        _symbol,
        0
    );
    reserve_4B(0);
    return;
}

instruction_call::~instruction_call() {}


//
//
//      INSTRUCTION ret
//
//

instruction_ret::instruction_ret(){}

auto instruction_ret::execute() -> void {

    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_ret::op_code,
                instruction_ret::mode,
                static_cast<uint8_t>(REGISTERS::PC),
                static_cast<uint8_t>(REGISTERS::SP),
                0,
                -4
            )
    );

}

instruction_ret::~instruction_ret() {}

// 
//
//      BRANCH
//
//


branch::branch(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c)
    : value(value), gpr_b(gpr_b), gpr_c(gpr_c){}

auto branch::execute_branch(uint8_t branch_displ, uint8_t branch_not_displ) -> void {

    auto& section = Asembler::get_current_section();

    // If passed operand is a literal

    if(std::holds_alternative<int32_t>(value)) {

        auto literal = std::get<int32_t>(value);

        // This is if literal can be placed inside displacement
        if(literal > std::pow(-2, 11) and literal < std::pow(2, 11)) {
            section.binary_data.add_instruction (
                combine(
                    branch::op_code,
                    branch_displ, 
                    static_cast<uint8_t>(REGISTERS::PC),
                    gpr_b,
                    gpr_c,
                    literal < 0 ? literal | 0x8000 : literal
                )
            );
            return;
        }

        // If it cannot be placed inside displacement

        section.binary_data.add_instruction (
            combine(
                branch::op_code,
                branch_not_displ, 
                static_cast<uint8_t>(REGISTERS::PC),
                gpr_b,
                gpr_c,
                4
            )
        );

        add_leap();
        reserve_4B(literal);
        return;
    }


    // Now the symbol part

    auto _symbol = std::get<std::string>(value);
    auto it = Asembler::symbol_table.find(_symbol);

    // There is only one case when we can leave displacement for symbol
    // That is is if is defined, in the same section and < 2^12

    if  ( 
            it != Asembler::symbol_table.end() and 
            Asembler::symbol_table[_symbol].ndx == Asembler::get_current_section().section_idx and 
            (Asembler::get_section_counter() - Asembler::symbol_table[_symbol].value) < std::pow(2,12)
        ) {    

        // SYMBOL EXISTS AND IS DEFINED

        section.binary_data.add_instruction (
            combine(
                branch::op_code,
                branch_displ, 
                static_cast<uint8_t>(REGISTERS::PC),
                gpr_b,
                gpr_c,
                (Asembler::symbol_table[_symbol].value - Asembler::get_section_counter()) | 0x8000 // should always be negative?
            )
        );

        return;
    }


    // Symbol not defined, we have to leave reloaciton data

    section.binary_data.add_instruction (
        combine(
            branch::op_code,
            branch_not_displ, 
            static_cast<uint8_t>(REGISTERS::PC),
            gpr_b,
            gpr_c,
            4
        )
    );

    add_leap();
    section.add_relocation(
        Asembler::get_section_counter(),
        RELOCATION_TYPE::ABS32,
        _symbol,
        0
    );
    reserve_4B(0);
    return;
 
}

branch::~branch() {}

// 
//
//      INSTRUCTION JMP
//
//

instruction_jmp::instruction_jmp(std::variant<std::string, int32_t> value)
    : branch(value,0,0) {}

auto instruction_jmp::execute() -> void {
    execute_branch(instruction_jmp::mode_displ, instruction_jmp::mode_not_displ);
}

instruction_jmp::~instruction_jmp() {}

// 
//
//      INSTRUCTION BEQ
//
//

instruction_beq::instruction_beq(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c)
    : branch(value,gpr_b,gpr_c){}

auto instruction_beq::execute() -> void {
    execute_branch(instruction_beq::mode_displ, instruction_beq::mode_not_displ);
}

instruction_beq::~instruction_beq() {}

// 
//
//      INSTRUCTION BNE
//
//

instruction_bne::instruction_bne(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c)
    : branch(value,gpr_b,gpr_c){}

auto instruction_bne::execute() -> void {
    execute_branch(instruction_bne::mode_displ, instruction_bne::mode_not_displ);
}

instruction_bne::~instruction_bne() {}

// 
//
//      INSTRUCTION BGT
//
//

instruction_bgt::instruction_bgt(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c)
    : branch(value,gpr_b,gpr_c){}

auto instruction_bgt::execute() -> void {
    execute_branch(instruction_bgt::mode_displ, instruction_bgt::mode_not_displ);
}

instruction_bgt::~instruction_bgt() {}

//
//
//      INSTRUCTION PUSH
//
//

instruction_push::instruction_push(uint8_t reg)
    : reg(reg){}

auto instruction_push::execute() -> void {

    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_push::op_code,
                instruction_push::mode,
                static_cast<uint8_t>(REGISTERS::SP),
                0,
                reg,
                4
            )
    );

}

instruction_push::~instruction_push() {}

//
//
//      INSTRUCTION POP
//
//

instruction_pop::instruction_pop(uint8_t reg)
    : reg(reg){}

auto instruction_pop::execute() -> void {

    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_pop::op_code,
                instruction_pop::mode,
                reg,
                static_cast<uint8_t>(REGISTERS::SP),
                0,
                -4
            )
    );

}

instruction_pop::~instruction_pop() {}

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

instruction_csrrd::instruction_csrrd(uint8_t gpr_d, REGISTERS csr)
    : gpr_d(gpr_d), csr(csr){}

auto instruction_csrrd::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_csrrd::op_code,
                instruction_csrrd::mode,
                static_cast<uint8_t>(csr),
                static_cast<uint8_t>(csr),
                gpr_d,
                0
            )
    );
}

instruction_csrrd::~instruction_csrrd() {}

//
//
//      INSTRUCTION csrwr
//

instruction_csrwr::instruction_csrwr(uint8_t gpr_s, REGISTERS csr)
    : gpr_s(gpr_s), csr(csr){}

auto instruction_csrwr::execute() -> void {
    auto& section = Asembler::get_current_section();

    section.binary_data.add_instruction(
        combine(
                instruction_csrwr::op_code,
                instruction_csrwr::mode,
                gpr_s,
                gpr_s,
                static_cast<uint8_t>(csr),
                0
            )
    );
}

instruction_csrwr::~instruction_csrwr() {}

//
//
//      INSTRUCTION ld
//

instruction_ld::instruction_ld(
        std::variant<std::string, int32_t> symbol_or_literal,
        uint8_t gpr_s,
        uint8_t gpr_d,
        OPERANDS op_type
): symbol_or_literal(symbol_or_literal), gpr_s(gpr_s), gpr_d(gpr_d), op_type(op_type){}

auto instruction_ld::execute() -> void {

    auto& section = Asembler::get_current_section();

    switch (op_type)
    {
    case OPERANDS::D_SYM:
        {
            auto _symbol = std::get<std::string>(symbol_or_literal);
            auto it = Asembler::symbol_table.find(_symbol);

            // There is only one case when we can leave displacement for symbol
            // That is is if is defined, in the same section and < 2^12

            if  ( 
                    it != Asembler::symbol_table.end() and 
                    Asembler::symbol_table[_symbol].ndx == Asembler::get_current_section().section_idx and 
                    (Asembler::get_section_counter() - Asembler::symbol_table[_symbol].value) < std::pow(2,12)
                ) {    

                // SYMBOL EXISTS AND IS DEFINED

                auto disp = Asembler::symbol_table[_symbol].value - Asembler::get_section_counter();

                section.binary_data.add_instruction (
                    combine(
                        instruction_ld::op_code,
                        instruction_ld::reg_disp, 
                        gpr_d,
                        static_cast<uint8_t>(REGISTERS::PC),
                        0,
                        disp | 0x8000 // should always be negative?
                    )
                );

                return;
            }


            // Symbol not defined, we have to leave reloaciton data

            section.binary_data.add_instruction (
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_disp, 
                    gpr_d,
                    static_cast<uint8_t>(REGISTERS::PC),
                    0,
                    4
                )
            );

            add_leap();
            section.add_relocation(
                Asembler::get_section_counter(),
                RELOCATION_TYPE::ABS32,
                _symbol,
                0
            );
            reserve_4B(0);
            return;
        }
        break;
    case OPERANDS::D_LIT:
        {
            auto literal = std::get<int32_t>(symbol_or_literal);

            // This is if literal can be placed inside displacement
            if(literal > std::pow(-2, 11) and literal < std::pow(2, 11)) {
                section.binary_data.add_instruction (
                    combine(
                        instruction_ld::op_code,
                        instruction_ld::reg_disp, 
                        gpr_d,
                        0,
                        0,
                        literal < 0 ? literal | 0x8000 : literal
                    )
                );
                return;
            }

            // If it cannot be placed inside displacement

            section.binary_data.add_instruction (
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_ind_disp, 
                    gpr_d,
                    static_cast<uint8_t>(REGISTERS::PC),
                    0,
                    4
                )
            );

            add_leap();
            reserve_4B(literal);
            return;

        }
        break;   
    case OPERANDS::SYM:
       {
            auto _symbol = std::get<std::string>(symbol_or_literal);

            // First I need to get symbol into my register so I could dobule inderect
            // After I have it in register I can go mem[reg] and do the final part
            // Getting symbol to register might require pool so get_my_symbol can couse that

            instruction_ld get_my_symbol = {
                    std::variant<std::string, int32_t>(_symbol),
                    0,
                    gpr_d,
                    OPERANDS::D_SYM
            };

            get_my_symbol.execute();

            // We have symbol in reg_d

            section.binary_data.add_instruction (
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_ind_disp, 
                    gpr_d,
                    static_cast<uint8_t>(REGISTERS::PC),
                    0,
                    4
                )
            );
            return;
        }
        break;
    case OPERANDS::LIT:
        {
            auto literal = std::get<int32_t>(symbol_or_literal);

            if (literal < 0) {
                throw std::runtime_error("ld_err: literal can't be a negative address");
            }

            // TODO; maybe delete this
            // This is if literal can be placed inside displacement
            if(literal < std::pow(2, 11)) {
                section.binary_data.add_instruction (
                    combine(
                        instruction_ld::op_code,
                        instruction_ld::reg_ind_disp, 
                        gpr_d,
                        0,
                        0,
                        literal
                    )
                );
                return;
            }

            // If it cannot be placed inside displacement

            // We have to add 2 isntruction becouse we dont have mem[mem[]]

            instruction_ld get_my_literal = {
                std::variant<std::string, int32_t>(literal),
                0,
                gpr_d,
                OPERANDS::D_LIT
            };

            get_my_literal.execute();

            section.binary_data.add_instruction (
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_ind_disp, 
                    gpr_d,
                    gpr_d,
                    0,
                    0
                )
            );

            return;
        }
        break;  
    case OPERANDS::REG:
        {
            section.binary_data.add_instruction(
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_disp,
                    gpr_d,
                    gpr_s,
                    0,
                    0
                )

            );
        }
        break;
    case OPERANDS::REG_IND:
        {
            section.binary_data.add_instruction(
                combine(
                    instruction_ld::op_code,
                    instruction_ld::reg_ind_disp,
                    gpr_d,
                    gpr_s,
                    0,
                    0
                )

            );
        }
        break;  
    case OPERANDS::REG_IND_DISP_LIT:
        {
            auto literal = std::get<int32_t>(this->symbol_or_literal);

            if(abs(literal) >= std::pow(2,12)) {
                throw std::runtime_error("ld instruction literal too big!");
            }


            section.binary_data.add_instruction(
                combine (
                    instruction_ld::op_code,
                    instruction_ld::reg_ind_disp,
                    gpr_d,
                    gpr_s,
                    0,
                    literal < 0 ? literal | 0x800 : literal
                )
            );
        }
        break;
    case OPERANDS::REG_IND_DISP_SYM:
        {

            auto _symbol = std::get<std::string>(symbol_or_literal);
            auto it = Asembler::symbol_table.find(_symbol);

            // There is only one case when we can leave displacement for symbol
            // That is is if is defined, in the same section and < 2^12
            
            if  ( 
                it != Asembler::symbol_table.end() and 
                Asembler::symbol_table[_symbol].symbol_bind == SYMBOL_BIND::ABSOLUTE
            ) {    
                // TODO:
                return;
            }

            throw std::runtime_error("ld instruciton: symbol not absolute"); 

        }
        break;
    default:
        throw std::runtime_error("LD INSTR ERROR");
        break;
    }

}

instruction_ld::~instruction_ld() {}


//
//
//      INSTRUCTION st
//

instruction_st::instruction_st(
        std::variant<std::string, int32_t> symbol_or_literal,
        uint8_t gpr_s,
        uint8_t gpr_d,
        OPERANDS op_type
): symbol_or_literal(symbol_or_literal), gpr_s(gpr_s), gpr_d(gpr_d), op_type(op_type){}

auto instruction_st::execute() -> void {

        auto& section = Asembler::get_current_section();

    switch (op_type)
    {
    case OPERANDS::D_LIT:
        { 
            throw std::runtime_error("st error: illegal addressing lit, D_LIT");
        }
        break;
    case OPERANDS::D_SYM:
        { 
            throw std::runtime_error("st error: illegal addressing sym, D_LIT");
        }
        break;
    case OPERANDS::LIT:
        {
            auto literal = std::get<int32_t>(symbol_or_literal);

            // This is if literal can be placed inside displacement
            if(literal < std::pow(2, 11)) {
                section.binary_data.add_instruction (
                    combine(
                        instruction_ld::op_code,
                        instruction_ld::reg_ind_disp, 
                        0,
                        0,
                        gpr_s,
                        literal
                    )
                );
                return;
            }

            // We have to add 2 isntruction becouse we dont have mem[mem[]]

            instruction_ld get_my_literal = {
                std::variant<std::string, int32_t>(literal),
                0,
                gpr_d,
                OPERANDS::D_LIT
            };

            get_my_literal.execute();

            // Literal value is inside reg_d now (at least should be)

            section.binary_data.add_instruction (
                combine(
                    instruction_st::op_code,
                    instruction_st::mem_mem_gpr, 
                    gpr_d,
                    0,
                    gpr_s,
                    0
                )
            );
            return;
        }
        break;
    case OPERANDS::SYM:
        {
            auto _symbol = std::get<std::string>(symbol_or_literal);

            // First I need to get symbol into my register so I could dobule inderect
            // After I have it in register I can go mem[reg] and do the final part
            // Getting symbol to register might require pool so get_my_symbol can couse that

            instruction_ld get_my_symbol = {
                    std::variant<std::string, int32_t>(_symbol),
                    0,
                    gpr_d,
                    OPERANDS::D_SYM
            };

            get_my_symbol.execute();

            // We have symbol in reg_d

            section.binary_data.add_instruction (
                combine(
                    instruction_st::op_code,
                    instruction_st::mem_gpr, 
                    gpr_d,
                    0,
                    gpr_s,
                    0
                )
            );
            return;
        }
        break;
    case OPERANDS::REG:
        {
            section.binary_data.add_instruction (
                combine(
                    instruction_st::op_code,
                    instruction_st::mem_gpr, 
                    gpr_d,
                    0,
                    gpr_s,
                    0
                )
            );
        }
        break;
    case OPERANDS::REG_IND:
        {
            section.binary_data.add_instruction (
                combine(
                    instruction_st::op_code,
                    instruction_st::mem_mem_gpr, 
                    gpr_d,
                    0,
                    gpr_s,
                    0
                )
            );
        }
        break;
    case OPERANDS::REG_IND_DISP_LIT:
        {
            auto literal = std::get<int32_t>(this->symbol_or_literal);

            if(abs(literal) >= std::pow(2,12)) {
                throw std::runtime_error("ld instruction literal too big!");
            }


            section.binary_data.add_instruction(
                combine (
                    instruction_st::op_code,
                    instruction_st::mem_mem_gpr,
                    gpr_d,
                    0,
                    gpr_s,
                    literal < 0 ? literal | 0x800 : literal
                )
            );
        }
        break;
    case OPERANDS::REG_IND_DISP_SYM:
        {
            auto _symbol = std::get<std::string>(symbol_or_literal);
            auto it = Asembler::symbol_table.find(_symbol);

            // There is only one case when we can leave displacement for symbol
            // That is is if is defined, in the same section and < 2^12
            
            if  ( 
                it != Asembler::symbol_table.end() and 
                Asembler::symbol_table[_symbol].symbol_bind == SYMBOL_BIND::ABSOLUTE and 
                (Asembler::get_section_counter() - Asembler::symbol_table[_symbol].value) < std::pow(2,12)
            ) {    
                // TODO:
                return;
            }

            throw std::runtime_error("ld instruciton: symbol not absolute"); 
        }
        break;
    default:
        throw std::runtime_error("ST INSTR ERROR");
        break;
    }

}

instruction_st::~instruction_st() {}

