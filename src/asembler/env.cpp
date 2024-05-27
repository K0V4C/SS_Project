#include "../../inc/asembler/env.hpp"
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>


env::env(int32_t argc, char** argv) {

    for(int i = 0; i < argc ; i++) { 
        this->cmd_args.emplace_back(argv[i]);
    }

}

auto env::get() const -> std::vector<std::string> {
    return this->cmd_args ;
}

auto env::input_file() const -> std::string {
    if(cmd_args.size() == 1) {
        throw std::runtime_error("INPUT FILE NOT DEFINED");
    }

    return cmd_args[cmd_args.size() - 1];
}

auto env::output_file() const -> std::string {

    if(cmd_args.size() == 1) {
        throw std::runtime_error("INPUT FILE NOT DEFINED");
    }

    if(cmd_args.size() == 2) {
        return cmd_args[1] + ".o";
    }

    if(cmd_args.size() != 4 or (cmd_args.size() == 4 and cmd_args[1] != std::string("-o")) ) {
        throw std::runtime_error("INPROPER ARGUMENTS");
    }


    return cmd_args[2];

}
