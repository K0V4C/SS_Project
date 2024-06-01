#ifndef _EMULATOR_HPP
#define _EMULATOR_HPP

#include "terminal.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <array>

const uint8_t pc = 15;
const uint8_t sp = 14;

const uint8_t status    = 0;
const uint8_t handler   = 1;
const uint8_t cause     = 2;

class Emulator {
public:
    Emulator(std::string);

    static constexpr uint32_t num_of_regs = 16;

    static constexpr uint32_t timer_mask    = (0x00'00'00'01);
    static constexpr uint32_t terminal_mask = (0x00'00'00'02);
    static constexpr uint32_t interrupt_gl  = (0x00'00'00'04);
    
    static constexpr uint32_t cause_il_ins  = 0x00'00'00'01;
    static constexpr uint32_t cause_timer   = 0x00'00'00'02;
    static constexpr uint32_t cause_term    = 0x00'00'00'03;
    static constexpr uint32_t cause_soft    = 0x00'00'00'04;
    
    static constexpr uint32_t il_ins  = 0x00'00'00'01;
    static constexpr uint32_t timer   = 0x00'00'00'02;
    static constexpr uint32_t term    = 0x00'00'00'04;
    static constexpr uint32_t soft    = 0x00'00'00'08;
    
    uint32_t interrupt_register         = 0;
    


    auto load_data() -> void;
    auto run() -> void;

    auto write_memory(uint32_t, uint32_t) -> void;
    auto read_instruction() -> uint32_t;
    auto read_memory(uint32_t) -> uint32_t;

    auto execute_instruction(uint32_t) -> void;
    auto check_interrupts() -> void;
    
    // Use this to interact with registers
    auto write_register(uint8_t, uint32_t) -> void;
    auto read_register(uint8_t) -> uint32_t;
    auto write_csr(uint8_t, uint32_t) -> void;
    auto read_csr(uint8_t) -> uint32_t;
    
    // Memmory mapped registers
    auto read_tim_cfg() -> uint32_t;
    
    auto write_term_in(uint32_t) -> void;
    
    auto print_registers() -> void;

    bool running;
    
    std::array<uint32_t, Emulator::num_of_regs> gpr;
    std::array<uint32_t, 3> csr;
    
    Terminal _terminal;
    
    bool die = false;
    
private:
    std::string file_name;
    std::map<uint32_t, uint8_t> memory;
    
};

#endif
