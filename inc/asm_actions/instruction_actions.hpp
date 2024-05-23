#ifndef _INSTRUCTION_ACTIONS_HPP
#define _INSTRUCTION_ACTIONS_HPP

#include "actions.hpp"
#include <cstdint>
#include <sys/types.h>

// Used as template for further

auto combine( 
    uint8_t op_code,
    uint8_t mode,
    uint8_t A,
    uint8_t B,
    uint8_t C,
    int32_t dipls
) -> uint32_t;

//
//
//      INSTRUCTION HALT
//
//
//

struct instruction_halt : public action {
    
    instruction_halt();

    virtual auto execute() -> void override;

    virtual ~instruction_halt();

    static constexpr uint8_t op_code = 0b000;
    static constexpr uint8_t mode    = 0b000;

};

//
//
//      INSTRUCTION XCHG
//
//
struct instruction_xchg : public action {
    
    instruction_xchg(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_xchg();

    static constexpr uint8_t op_code = 0b1000;
    static constexpr uint8_t mode    = 0b0000;

private:
    uint8_t gpr_s, gpr_d;   
};


//
//
//      INSTRUCTION ADD
//
//
struct instruction_add : public action {
    
    instruction_add(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
    
       
    virtual ~instruction_add();

    static constexpr uint8_t op_code = 0b0101;
    static constexpr uint8_t mode    = 0b0000;

private:

    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION SUB
//
//

struct instruction_sub : public action {
    
    instruction_sub(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_sub();

    static constexpr uint8_t op_code = 0b0101;
    static constexpr uint8_t mode    = 0b0001;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION MUL
//
//

struct instruction_mul : public action {
    
    instruction_mul(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_mul();

    static constexpr uint8_t op_code = 0b0101;
    static constexpr uint8_t mode    = 0b0010;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION DIV
//
//

struct instruction_div : public action {
    
    instruction_div(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_div();

    static constexpr uint8_t op_code = 0b0101;
    static constexpr uint8_t mode    = 0b0011;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION NOT
//
//

struct instruction_not : public action {
    
    instruction_not(uint8_t gpr_s);

    virtual auto execute() -> void override;
 
    virtual ~instruction_not();

    static constexpr uint8_t op_code = 0b0110;
    static constexpr uint8_t mode    = 0b0000;

private:
    uint8_t gpr_s;   
};

//
//
//      INSTRUCTION AND
//
//

struct instruction_and : public action {
    
    instruction_and(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_and();

    static constexpr uint8_t op_code = 0b0110;
    static constexpr uint8_t mode    = 0b0001;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION OR
//
//

struct instruction_or : public action {
    
    instruction_or(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_or();

    static constexpr uint8_t op_code = 0b0110;
    static constexpr uint8_t mode    = 0b0010;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION XOR
//
//

struct instruction_xor : public action {
    
    instruction_xor(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_xor();

    static constexpr uint8_t op_code = 0b0110;
    static constexpr uint8_t mode    = 0b0011;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION SHL
//
//

struct instruction_shl : public action {
    
    instruction_shl(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_shl();

    static constexpr uint8_t op_code = 0b0111;
    static constexpr uint8_t mode    = 0b0000;

private:
    uint8_t gpr_s, gpr_d;   
};

//
//
//      INSTRUCTION SHR
//
//

struct instruction_shr : public action {
    
    instruction_shr(uint8_t gpr_s, uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_shr();

    static constexpr uint8_t op_code = 0b0111;
    static constexpr uint8_t mode    = 0b0001;

private:
    uint8_t gpr_s, gpr_d;   
};


//
//
//      INSTRUCTION CSRRD
//
//

struct instruction_csrrd : public action {
    
    instruction_csrrd(uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_csrrd();

private:
    uint8_t gpr_d;   
};

//
//
//      INSTRUCTION CSRRW
//
//

struct instruction_csrwr : public action {
    
    instruction_csrwr(uint8_t gpr_d);

    virtual auto execute() -> void override;
 
    virtual ~instruction_csrwr();

private:
    uint8_t gpr_d;   
};

#endif // _INSTRUCTION_ACTIONS_HPP
