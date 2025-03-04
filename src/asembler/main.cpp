#include "../../inc/asembler/parser.hpp"
#include "../../inc/asembler/env.hpp"
#include "../../inc/asembler/asembler.hpp"
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
    // Parser::display_log();
    } catch(...) {
        Parser::display_log();
    }
}




auto main(int argc, char** argv) -> int {
    env env_obj(argc, argv);
    test_parser(env_obj.input_file());

    Asembler asm_control;

    try {
    asm_control.asemble();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    try{
        asm_control.backpatch();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

#ifdef DEBUG
    Asembler::print_section_table();
    Asembler::print_symbol_table();
#endif
    
    Asembler::generate_txt_file(env_obj.output_file());
    Asembler::serialize(env_obj.output_file());

    return 0;
}
