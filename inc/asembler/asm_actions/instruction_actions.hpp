#ifndef _INSTRUCTION_ACTIONS_HPP
#define _INSTRUCTION_ACTIONS_HPP

#include "inc/common/types.hpp"

#include "actions.hpp"
#include <cstdint>
#include <variant>
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

auto add_leap() -> void;

auto reserve_4B(uint32_t) -> void;

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

    static constexpr uint8_t op_code = 0b0000;
    static constexpr uint8_t mode    = 0b0000;

};

//
//
//      INSTRUCTION INT
//
//
//

struct instruction_int : public action {
    
    instruction_int();

    virtual auto execute() -> void override;

    virtual ~instruction_int();

    static constexpr uint8_t op_code = 0b0001;
    static constexpr uint8_t mode    = 0b0000;

};

//
//
//      INSTRUCTION IRET
//
//
//

struct instruction_iret : public action {
    
    instruction_iret();

    virtual auto execute() -> void override;

    virtual ~instruction_iret();

    static constexpr uint8_t op_code     = 0b1001;
    static constexpr uint8_t pop_status  = 0b0111;
    static constexpr uint8_t pop_pc      = 0b0011;
};

//
//
//      INSTRUCTION CALL
//
//
//

// TODO: check if this is correct
struct instruction_call : public action {
    
    instruction_call(std::variant<std::string, int32_t> operand);

    virtual auto execute() -> void override;

    virtual ~instruction_call();

    static constexpr uint8_t op_code        = 0b0010;
    static constexpr uint8_t mode_displ     = 0b0000;
    static constexpr uint8_t mode_not_displ = 0b0001;
private:
    std::variant<std::string, int32_t> operand;
};

//
//
//      INSTRUCTION RET
//
//
//

struct instruction_ret : public action {
    
    instruction_ret();

    virtual auto execute() -> void override;

    virtual ~instruction_ret();

    static constexpr uint8_t op_code     = 0b1001;
    static constexpr uint8_t mode        = 0b0011;
};


//
//
//      BRANCH INSTRUCTIONS
//
//

struct branch : public action {

    branch(std::variant<std::string, int32_t>, uint8_t, uint8_t);

    auto execute_branch(uint8_t branch_displ, uint8_t branch_not_displ) -> void;

    ~branch();

    static constexpr uint8_t op_code        = 0b0011;

protected:
    std::variant<std::string, int32_t> value;
    uint8_t gpr_b;
    uint8_t gpr_c;

};

// TODO:
//
//
//      INSTRUCTION JMP
//
//
//

struct instruction_jmp : public branch {
    
    instruction_jmp(std::variant<std::string, int32_t>);

    virtual auto execute() -> void override;

    virtual ~instruction_jmp();

    static constexpr uint8_t mode_displ     = 0b0000;
    static constexpr uint8_t mode_not_displ = 0b1000;
};

//
//
//      INSTRUCTION BEQ
//
//
//

struct instruction_beq : public branch {
    
    instruction_beq(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c);

    virtual auto execute() -> void override;

    virtual ~instruction_beq();

    static constexpr uint8_t mode_displ     = 0b0001;
    static constexpr uint8_t mode_not_displ = 0b1001;
};

//
//
//      INSTRUCTION BNE
//
//
//

struct instruction_bne : public branch {
    
    instruction_bne(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c);

    virtual auto execute() -> void override;

    virtual ~instruction_bne();

    static constexpr uint8_t mode_displ     = 0b0010;
    static constexpr uint8_t mode_not_displ = 0b1010;

};

//
//
//      INSTRUCTION BGT
//
//
//

struct instruction_bgt : public branch {
    
    instruction_bgt(std::variant<std::string, int32_t> value, uint8_t gpr_b, uint8_t gpr_c);

    virtual auto execute() -> void override;

    virtual ~instruction_bgt();

    static constexpr uint8_t mode_displ     = 0b0001;
    static constexpr uint8_t mode_not_displ = 0b0011;
};

//
//
//      INSTRUCTION PUSH
//
//
//

struct instruction_push : public action {
    
    instruction_push(uint8_t reg);

    virtual auto execute() -> void override;

    virtual ~instruction_push();

    static constexpr uint8_t op_code     = 0b1000;
    static constexpr uint8_t mode        = 0b0001;
private:
    uint8_t reg;
};

//
//
//      INSTRUCTION POP
//
//
//

struct instruction_pop : public action {
    
    instruction_pop(uint8_t reg);

    virtual auto execute() -> void override;

    virtual ~instruction_pop();

    static constexpr uint8_t op_code     = 0b1001;
    static constexpr uint8_t mode        = 0b0011;
private:
    uint8_t reg;
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
    
    instruction_csrrd(uint8_t gpr_d, REGISTERS csr);

    virtual auto execute() -> void override;
 
    virtual ~instruction_csrrd();

    static uint8_t constexpr op_code = 0b1001;
    static uint8_t constexpr mode    = 0b0000;  


private:
    uint8_t gpr_d;   
    REGISTERS csr;
};

//
//
//      INSTRUCTION CSRRW
//
//

struct instruction_csrwr : public action {
    
    instruction_csrwr(uint8_t gpr_s, REGISTERS csr);

    virtual auto execute() -> void override;
 
    virtual ~instruction_csrwr();

    static uint8_t constexpr op_code = 0b1001;
    static uint8_t constexpr mode    = 0b0100;  

private:
    uint8_t gpr_s;   
    REGISTERS csr;
};


//
//
//      INSTRUCTION LD
//
//

struct instruction_ld : public action {
    
    instruction_ld(
        std::variant<std::string, int32_t> symbol_or_literal,
        uint8_t gpr_s,
        uint8_t gpr_d,
        OPERANDS op_type
    );

    virtual auto execute() -> void override;
 
    virtual ~instruction_ld();

    // TODO:
    static uint8_t constexpr op_code = 0b1001;
    static uint8_t constexpr mode    = 0b0100;  

private:
        std::variant<std::string, int32_t> symbol_or_literal;
        uint8_t gpr_s;
        uint8_t gpr_d;
        OPERANDS op_type;
};

//
//
//      INSTRUCTION ST
//
//

struct instruction_st : public action {
    
    instruction_st(
        std::variant<std::string, int32_t> symbol_or_literal,
        uint8_t gpr_s,
        uint8_t gpr_d,
        OPERANDS op_type
    );

    virtual auto execute() -> void override;
 
    virtual ~instruction_st();

    // TODO:
    static uint8_t constexpr op_code = 0b1001;
    static uint8_t constexpr mode    = 0b0100;  

private:
        std::variant<std::string, int32_t> symbol_or_literal;
        uint8_t gpr_s;
        uint8_t gpr_d;
        OPERANDS op_type;
};



#endif // _INSTRUCTION_ACTIONS_HPP
