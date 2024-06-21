#include "../../inc/emulator/terminal.hpp"
#include <cstdio>

Terminal::Terminal() {
    // Get current terminal settings
    if (tcgetattr(STDIN_FILENO, &old_termios) == -1) {
        std::cerr << "Failure in getting temrinal atributes";
        exit(EXIT_FAILURE);
    }

    // Copy settings to modify
    new_termios = old_termios;

    // Set terminal to raw mode
    new_termios.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    new_termios.c_cc[VMIN] = 1; // Minimum number of characters to read
    new_termios.c_cc[VTIME] = 0; // No timeout

    // Set new attributes
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1) {
        std::cerr << "Failure in seting terminal attributes";
        exit(EXIT_FAILURE);
    }
}
    
auto Terminal::write_val(uint32_t val) -> void {
    uint8_t out = (val & 0x0ff);
    if (write(STDIN_FILENO, &val, 1) != 1) {
        std::cerr << "Error writing charater";
        exit(EXIT_FAILURE);
    }
}

auto Terminal::read_val() -> uint8_t {
    char ch;
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        std::cerr << "Error reading char from terminal";
        exit(EXIT_FAILURE);
    }
    return ch;
}
    
    

Terminal::~Terminal() {
    // Restore original terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_termios) == -1) {
        std::cerr << "Failure in Terminal destructor"; 
    }
}