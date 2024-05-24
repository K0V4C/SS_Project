#ifndef _TYPES_HPP
#define _TYPES_HPP


enum class SYMBOL_TYPE {
    NOTYP,
    SCTN
};

const char* symbol_type_2_string(SYMBOL_TYPE s); 

enum class SYMBOL_BIND {
    LOCAL,
    EXTERN
};

const char* symbol_bind_2_string(SYMBOL_BIND s);

enum class RELOCATION_TYPE {
    ABS32
};

const char* relocation_type_2_string(RELOCATION_TYPE s);

enum class REGISTERS {
    PC = 15,
    SP = 14
};

#endif // _TYPES_HPP 
