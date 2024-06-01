#include "../../inc/emulator/external.hpp"

#include <chrono>
#include <cstdint>
#include <exception>
#include <unistd.h>
#include <termios.h>

#include "../../inc/emulator/emulator.hpp"

using namespace std::chrono;

auto terminal_thread_body(Emulator *emu) -> void {
    
    while(!emu->die) {
        uint8_t val = emu->_terminal.read_val();
        emu->write_term_in(val << 24);
        
        emu->interrupt_register |= Emulator::timer;
    }
    
}

auto timer_thread_body(Emulator *emu) -> void {
    
    auto delta_time = 500ms;
    
    auto time = emu->read_tim_cfg();
    
    switch (time) {
        case 0x1: 
            delta_time = 1000ms;
            break;
        case 0x2: 
            delta_time = 1500ms;
            break; 
        case 0x3: 
            delta_time = 2000ms;
            break;
        case 0x4: 
            delta_time =5000ms;
            break;
        case 0x5: 
            delta_time = 10s;
            break;
        case 0x6: 
            delta_time = 30s;
            break;
        case 0x7: 
            delta_time = 60s;
            break;
        case 0x0:
        default:
            delta_time = 500ms;
            break;
    }
    
    
    auto start_time = std::chrono::steady_clock::now();
    
    while(!emu->die) {
        
        auto end_time = std::chrono::steady_clock::now(); 
        
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time); 
        
        if(elapsed > delta_time) {
            emu->interrupt_register |= Emulator::timer;
            start_time = std::chrono::steady_clock::now();
        }
    }   

}