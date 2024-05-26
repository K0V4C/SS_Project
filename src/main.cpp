#include "../inc/parser.hpp"
#include "../inc/env.hpp"
#include "../inc/asembler.hpp"
#include <iostream>
#include <stdexcept>

auto test_command_line_args(env& obj) -> void {
     
    auto args = obj.get();

    for(auto& a : args) {
        std::cout << a << std::endl;
    }

}

auto test_parser(std::string file_name) -> void {
    Parser parser(file_name);
    try {
    parser.parse();
    Parser::display_log();
    } catch(...) {
        Parser::display_log();
    }
}




auto main(int argc, char** argv) -> int {
    env env_obj(argc, argv);
    std::cout << "OVDE";
    test_parser(env_obj.get()[1]);
    
    try {
    Asembler asm_control;
    asm_control.asemble();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    } 

    Asembler::print_section_table();
    Asembler::print_symbol_table();
    Asembler::print_forward_table();

    Asembler::serialize("output.bin");

    return 0;
}
