#include "../../inc/emulator/emulator.hpp"

auto main(int argc, char** argv) -> int {
    
    if(argc < 2) {
        throw std::runtime_error("File name not given!");
    }
    
    Emulator emu(argv[1]);
    
    emu.load_data();
    emu.run();
    
    
    return 0;
}