#ifndef _INSTRUCTION_EMULTAR_HPP
#define _INSTRUCTION_EMULTAR_HPP

#include <cstdint>
struct instruction_type {
    
    static constexpr uint32_t _halt             = 0b0000'0000'0000'0000'0000'0000'0000'0000;
    
    static constexpr uint32_t _int              = 0b0000'0000'0000'0000'0000'0000'0001'0000;
 
    static constexpr uint32_t _call_12b         = 0b0000'0000'0000'0000'0000'0000'0010'0000;
    static constexpr uint32_t _call_32b         = 0b0000'0000'0000'0000'0000'0000'0010'0001;
    
    static constexpr uint32_t _jmp_12b          = 0b0000'0000'0000'0000'0000'0000'0011'0000;
    static constexpr uint32_t _jmp_32b          = 0b0000'0000'0000'0000'0000'0000'0011'1000;
    
    static constexpr uint32_t _beq_12b          = 0b0000'0000'0000'0000'0000'0000'0011'0001;
    static constexpr uint32_t _beq_32b          = 0b0000'0000'0000'0000'0000'0000'0011'1001;
    
    static constexpr uint32_t _bne_12b          = 0b0000'0000'0000'0000'0000'0000'0011'0010; 
    static constexpr uint32_t _bne_32b          = 0b0000'0000'0000'0000'0000'0000'0011'1010;
    
    static constexpr uint32_t _bgt_12b          = 0b0000'0000'0000'0000'0000'0000'0011'0011;
    static constexpr uint32_t _bgt_32b          = 0b0000'0000'0000'0000'0000'0000'0011'1011;
    
    static constexpr uint32_t _push             = 0b0000'0000'0000'0000'0000'0000'1000'0001;
    static constexpr uint32_t _pop              = 0b0000'0000'0000'0000'0000'0000'1001'0011;
    static constexpr uint32_t _pop_csr          = 0b0000'0000'0000'0000'0000'0000'1001'0111;
    
    static constexpr uint32_t _xchg             = 0b0000'0000'0000'0000'0000'0000'0100'0000;
    
    static constexpr uint32_t _add              = 0b0000'0000'0000'0000'0000'0000'0101'0000;
    static constexpr uint32_t _sub              = 0b0000'0000'0000'0000'0000'0000'0101'0001;
    static constexpr uint32_t _mul              = 0b0000'0000'0000'0000'0000'0000'0101'0010;
    static constexpr uint32_t _div              = 0b0000'0000'0000'0000'0000'0000'0101'0011;
    
    static constexpr uint32_t _not              = 0b0000'0000'0000'0000'0000'0000'0110'0000;
    static constexpr uint32_t _and              = 0b0000'0000'0000'0000'0000'0000'0110'0001;
    static constexpr uint32_t _or               = 0b0000'0000'0000'0000'0000'0000'0110'0010;
    static constexpr uint32_t _xor              = 0b0000'0000'0000'0000'0000'0000'0110'0011;
    
    static constexpr uint32_t _shl              = 0b0000'0000'0000'0000'0000'0000'0111'0000;
    static constexpr uint32_t _shr              = 0b0000'0000'0000'0000'0000'0000'0111'0001;
    
    static constexpr uint32_t _csrrd            = 0b0000'0000'0000'0000'0000'0000'1001'0000;
    static constexpr uint32_t _csrwr            = 0b0000'0000'0000'0000'0000'0000'1001'0100;
    
    static constexpr uint32_t _ld_12b           = 0b0000'0000'0000'0000'0000'0000'1001'0001;
    static constexpr uint32_t _ld_32b           = 0b0000'0000'0000'0000'0000'0000'1001'0010;
    static constexpr uint32_t _csr_from_stack   = 0b0000'0000'0000'0000'0000'0000'1001'0110;
    
    static constexpr uint32_t _st_mem           = 0b0000'0000'0000'0000'0000'0000'1000'0000;
    static constexpr uint32_t _st_mem_mem       = 0b0000'0000'0000'0000'0000'0000'1000'0010;  
};

#endif //_INSTRUCTION_EMULTARO_HPP