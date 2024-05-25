#ifndef _TYPES_HPP
#define _TYPES_HPP


enum class SYMBOL_TYPE {
    NOTYP,
    SCTN
};

const char* symbol_type_2_string(SYMBOL_TYPE s); 

enum class SYMBOL_BIND {
    LOCAL,
    EXTERN,
    GLOBAL
};

const char* symbol_bind_2_string(SYMBOL_BIND s);

enum class RELOCATION_TYPE {
    ABS32
};

const char* relocation_type_2_string(RELOCATION_TYPE s);

enum class REGISTERS {
    PC = 15,
    SP = 14,
    STATUS  = 0,
    HANDLER = 1,
    CAUSE   = 2
};

enum class OPERANDS {

    D_LIT,
    D_SYM,
    LIT,
    SYM,
    REG,
    REG_IND,
    REG_IND_DISP_SYM,
    REG_IND_DISP_LIT

};
#endif // _TYPES_HPP 
