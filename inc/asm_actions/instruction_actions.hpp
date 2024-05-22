#ifndef _INSTRUCTION_ACTIONS_HPP
#define _INSTRUCTION_ACTIONS_HPP

#include "actions.hpp"
#include <cstdint>

// Used as template for further

auto combine( 
    uint8_t op_code,
    uint8_t mode,
    uint8_t A,
    uint8_t B,
    uint8_t C,
    int32_t dipls
) -> int32_t;

struct _instruction : public action {
    
    _instruction();

    virtual auto execute() -> void override;
    
    virtual ~_instruction();

private:
    
};
//
//    add_instruction
//|   sub_instrucion
//|   mul_instruction
//|   div_instruction
//|   not_instruction
//|   and_instruction
//|   or_instruction
//|   xor_instruction
//|   shl_instruction
//|   shr_instruction

struct instruction_add : public action {
    
    instruction_add(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
    
       
    virtual ~instruction_add();

    static constexpr uint8_t op_code = 0b1010;
    static constexpr uint8_t mode    = 0b0000;

private:

    uint8_t gpr_s, gpr_d;   
};





#endif // _INSTRUCTION_ACTIONS_HPP
