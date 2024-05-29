#include "../../inc/linker/env.hpp"

#include <cstdint>
#include <iostream>
/*

        STATIC INITIALIZATION

*/

bool env::hex = false;
bool env::relocatable = false;

env::env(int32_t argc, char** argv) {
    for(int i = 0; i < argc; i++) {
        if(i == 0) { continue; }

        if(std::string(argv[i]) == "-o") {
            // Name of file
            file_name = argv[++i];
            continue;
        }

        if(std::string(argv[i]) == "-hex") {
            env::hex = true;
            continue;
        }

        if(std::string(argv[i]) == "-relocatable") {
            env::relocatable = true;
            continue;
        }


        // TODO: test this
        if(std::string(argv[i]).substr(0,6) == std::string("-place")) {

            auto pos = std::string(argv[i]).find("@");
            auto section = std::string(argv[i]).substr(7,pos - 7);

            auto number  = std::stoi(
                std::string(argv[i]).substr(pos+1)
            );

            std::cout << "PLACE  " << section << "   " << number;

            places[section] = number;
            continue;
        }

        // Only thing left is tat is a file:
        input_files.push_back(argv[i]);

    }
}

auto env::get_file_name() const -> std::string {
    return file_name;
}

auto env::get_places() const -> std::map<std::string, uint32_t>{
    return places;
}

auto env::get_input_files() const -> std::vector<std::string> {
    return input_files;
}

env::~env(){}
