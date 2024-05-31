#include "../../inc/emulator/instruction.hpp"
#include "../../inc/emulator/emulator.hpp"

#include <iomanip>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <array>
#include <stdexcept>
#include <string>

#define debug  1

// Static data
//
//
//
//

Emulator::Emulator(std::string file_name)
    : file_name(file_name) {

        gpr.fill(0);
        csr.fill(0);

        gpr[pc] = 0x40000000;
        gpr[sp] = 0x0;
        
        // memory mapped registers
        for(uint32_t i = 0xffffff00; i < 0xffffffff; i++) {
            memory[i] = 0;
        }
    }


auto Emulator::load_data() -> void {


    std::ifstream hex_file(file_name);

    std::string input;

    while(std::getline(hex_file, input)) {

        std::string _addr = "0x" + input.substr(0,8);

        uint32_t addr = std::stol(_addr, 0, 16);

        std::string _byte;

        for(int i = 0; i < 8; i++) {

            _byte = "0x" + input.substr(10 + 3*i ,2);
            uint8_t byte = std::stol(_byte,0,16);
            memory[addr + i] = byte;

        }
    }
}


auto Emulator::run() -> void {

    try {

        while(running) {
            std::cout << std::hex << (int)read_register(pc) << std::endl;
            
            std::cout <<std::endl;print_registers();std::cout<<std::endl;
            execute_instruction(
                read_instruction()
            );
            // check for interrupts
        }
    } catch (std::runtime_error& e){
        std::cout   << "Emulated processor halted\n"
                    <<  e.what() << std::endl;
    }

    std::cout << "Emulated procesor state\n";

    print_registers();

}


auto Emulator::write_memory(uint32_t addr, uint32_t data) -> void {
    memory[addr]     = (data & 0xff000000) >> 24;
    memory[addr + 1] = (data & 0x00ff0000) >> 16;
    memory[addr + 2] = (data & 0x0000ff00) >> 8;
    memory[addr + 3] = (data & 0x000000ff) >> 0;
}
auto Emulator::read_instruction() -> uint32_t {
    
    uint32_t t_pc = read_register(pc);
    
    uint32_t insturction_raw = memory[t_pc]                 ; t_pc += 1;
    insturction_raw = (insturction_raw << 8 ) | memory[t_pc]; t_pc += 1;
    insturction_raw = (insturction_raw << 8 ) | memory[t_pc]; t_pc += 1;
    insturction_raw = (insturction_raw << 8 ) | memory[t_pc]; t_pc += 1;

    write_register(pc, t_pc);
    
    return insturction_raw;
}

auto Emulator::read_memory(uint32_t addr) -> uint32_t{
    uint32_t insturction_raw = memory[addr]                 ;
    insturction_raw = (insturction_raw << 8 ) | memory[addr];
    insturction_raw = (insturction_raw << 8 ) | memory[addr];
    insturction_raw = (insturction_raw << 8 ) | memory[addr];
    
    return insturction_raw;
}

auto Emulator::write_register(uint8_t reg, uint32_t data) -> void {
    if(reg == 0) return;
    if(reg > 15) throw std::runtime_error("Register doesn not exists: " + std::to_string(reg));
    gpr[reg] = data;
}

auto Emulator::read_register(uint8_t reg) -> uint32_t {
    if(reg > 15) throw std::runtime_error("Register doesn not exists: " + std::to_string(reg));
    return gpr[reg];
}

auto Emulator::write_csr(uint8_t reg, uint32_t data) -> void {
  if(reg > 3) throw std::runtime_error("csr register out of range");
  csr[reg] = data;
}

auto Emulator::read_csr(uint8_t reg) -> uint32_t {
    if(reg > 2) throw std::runtime_error("csr register out of range");
    return csr[reg];
}

auto Emulator::execute_instruction(uint32_t instruction_raw) -> void {

    // I have to move it first

    uint32_t op_and_mode_mask = 0b1111'1111'0000'0000'0000'0000'0000'0000;

    uint8_t A = (instruction_raw & 0x00f00000) >> 20;
    uint8_t B = (instruction_raw & 0x000f0000) >> 16;
    uint8_t C = (instruction_raw & 0x0000f000) >> 12;

    int32_t D = (instruction_raw & 0x00000fff);
    
    if(D & 0x00000800) {
        D |= 0xfffff000;
    }

    switch ((instruction_raw & op_and_mode_mask)) {

        case instruction_type::_halt: {
            throw std::runtime_error("Halt instruction");
            return;
        }

        case instruction_type::_int: {

#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  INT INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            interrupt_register |= Emulator::soft;

            return;
        }

        case instruction_type::_call_12b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  CALL 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            // push pc
            write_register(
                sp, read_register(sp) - 4
            );

            write_memory(
                read_register(sp), read_register(pc)
            );

            // jump
            write_register(
                pc, read_register(A) + read_register(B) + D
            );

            return;
        }

        case instruction_type::_call_32b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  CALL 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            // push pc
            write_register(
                sp, read_register(sp) - 4
            );

            write_memory(
                read_register(sp), read_register(pc)
            );

            // jump
            write_register(
                pc, read_memory(read_register(A) + read_register(B) + D)
            );

            return;
        }

        case instruction_type::_jmp_12b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  JMP 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                pc, read_register(A) + D
            );

            return;
        }

        case instruction_type::_jmp_32b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  JMP 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                    pc, read_memory(read_register(A) + D)
            );

            return;
        }

        case instruction_type::_beq_12b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BEQ 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(read_register(B) == read_register(C)) {
                write_register(
                    pc, read_register(A) + D
                );
            }

            return;
        }

        case instruction_type::_beq_32b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BEQ 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(read_register(B) == read_register(C)) {
                write_register(
                    pc, read_memory(read_register(A) + D)
                );
            }

            return;
        }

        case instruction_type::_bne_12b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BNE 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(read_register(B) != read_register(C)) {
                write_register(
                    pc, read_register(A) + D
                );
            }

            return;
        }

        case instruction_type::_bne_32b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BNE 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(read_register(B) != read_register(C)) {
                write_register(
                    pc, read_memory(read_register(A) + D)
                );
            }

            return;
        }

        case instruction_type::_bgt_12b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BGT 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(static_cast<int32_t>(read_register(B)) > static_cast<int32_t>(read_register(C))) {
                write_register(
                    pc, read_register(A) + D
                );
            }

            return;
        }

        case instruction_type::_bgt_32b: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  BGT 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            if(static_cast<int32_t>(read_register(B)) > static_cast<int32_t>(read_register(C))) {
                write_register(
                    pc, read_memory(read_register(A) + D)
                );
            }


            return;
        }

        case instruction_type::_push: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  PUSH INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(A) + D
            );

            write_memory(
                read_register(A), read_register(C)
            );

            return;
        }

        case instruction_type::_pop: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  POP INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_memory(read_register(B))
            );

            write_register(
                B,  read_register(B) + D
            );

            return;
        }

        case instruction_type::_pop_csr: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  POP CSR INSTRUCION  " 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_csr(
                 A, read_memory(read_register(B))
             );

             write_register(
                 B,  read_register(B) + D
             );

            return;
        }

        case instruction_type::_xchg: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  XCHG INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            uint32_t tmp = read_register(B);
            write_register(B, read_register(C));
            write_register(C, tmp);

            return;
        }

        case instruction_type::_add: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  ADD INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) + read_register(C)
            );

            return;
        }

        case instruction_type::_sub: {

#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  SUB INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) - read_register(C)
            );

            return;
        }

        case instruction_type::_mul: {
    
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  MUL INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) * read_register(C)
            );

            return;
        }

        case instruction_type::_div: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  DIV INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) / read_register(C)
            );

            return;
        }

        case instruction_type::_not: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  NOT INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, ~read_register(B)
            );

            return;
        }

        case instruction_type::_and: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  AND INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) & read_register(C)
            );

            return;
        }

        case instruction_type::_or: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  OR INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) | read_register(C)
            );

            return;
        }

        case instruction_type::_xor: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  XOR INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) ^ read_register(C)
            );

            return;
        }

        case instruction_type::_shl: {
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  SHL INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) << read_register(C)
            );

            return;
        }

        case instruction_type::_shr: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  SHR INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) >> read_register(C)
            );

            return;
        }

        case instruction_type::_csrrd: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  CSRRD INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_csr(B)
            );

            return;
        }

        case instruction_type::_csrwr: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  CSRWR INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif
            
            write_csr(
                A, read_register(B)
            );
            return;
        }

        case instruction_type::_ld_12b: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  LD 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_register(B) +  D
            );

            return;
        }

        case instruction_type::_ld_32b: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  LD 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_register(
                A, read_memory(read_register(B) + read_register(C) +  D)
            );

            return;
        }

        case instruction_type::_st_12b: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  ST 12b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_memory(
                read_register(A) + read_register(B) + D
                , read_register(C)
            );

            return;
        }

        case instruction_type::_st_32b: {
            
            
#if debug      
            std::cout   << "PC VALUE : "  << std::hex << read_register(pc)
                        << "  RAW INSTRUCTION : " << std::hex << instruction_raw 
                        << "  ST 32b INSTRUCION" 
                        << "  register(" << std::hex << (int)A << ") = " << std::hex << (int)read_register(A)
                        << "  register(" << std::hex << (int)B << ") = " << std::hex << (int)read_register(B)
                        << "  register(" << std::hex << (int)C << ") = " << std::hex << (int)read_register(C)
                        << "  displacement = " << std::hex << (int)D
                        << std::endl;
#endif

            write_memory(
                read_memory(read_register(A) + read_register(B) + D)
                , read_register(C)
            );

            return;
        }
    }

    // Invalid op_code

    interrupt_register |= il_ins;

    return;
}


auto Emulator::check_interrupts() -> void {

    if(interrupt_register != 0) {

        if(interrupt_register & Emulator::il_ins) {
            
            write_csr(cause, Emulator::cause_il_ins);
            interrupt_register ^= Emulator::il_ins;

        } else if(interrupt_register & Emulator::soft) {

            write_csr(cause, Emulator::cause_soft);
            interrupt_register ^= Emulator::soft;

        } else if(interrupt_register & Emulator::term
                    and !(read_csr(status) & Emulator::interrupt_gl)
                    and !(read_csr(status) & Emulator::terminal_mask)) {

            write_csr(cause, Emulator::cause_term);
            interrupt_register ^= Emulator::term;

        } else if(interrupt_register & Emulator::timer
                    and !(read_csr(status) & Emulator::interrupt_gl) 
                    and !(read_csr(status) & Emulator::timer_mask)) {

            write_csr(cause, Emulator::cause_timer);
            interrupt_register ^= Emulator::timer;

        }

        write_register(
            sp, read_register(sp) - 4
        );

        write_memory(
            read_register(sp), read_register(cause)
        );

        write_register(
            sp, read_register(sp) - 4
        );

        write_memory(
            read_register(sp), read_register(pc)
        );

        write_csr(
            status, read_csr(status) & ~Emulator::interrupt_gl
        );

        write_register(
            pc, read_csr(handler)
        );


    }

}

auto Emulator::print_registers() -> void {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout   << "r" << std::setw(2) << std::setfill(' ') << std::left << std::dec << i*4+j
                        << "=0x" << std::setw(8) << std::setfill('0') << std::hex << gpr[i*4+j]
                        << "        ";
        }
        std::cout << std::endl;
    }
}
