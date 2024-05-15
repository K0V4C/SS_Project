#include "../include/env.hpp"
#include <cstdint>
#include <string>
#include <vector>


env::env(int32_t argc, char** argv) {

    for(int i = 0; i < argc ; i++) { 
        this->cmd_args.emplace_back(argv[i]);
    }

}


auto env::get() const -> std::vector<std::string> {
    return this->cmd_args ;
}
