#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP

#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <iostream>

class Terminal {
public:
    Terminal() ;
    
    auto write_val(uint32_t) -> void;
    
    auto read_val() -> uint8_t;
    
    ~Terminal() ;

private:
    struct termios old_termios;
    struct termios new_termios;
    
};


#endif