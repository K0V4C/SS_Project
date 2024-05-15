#include "../inc/parser.hpp"
#include "../inc/env.hpp"
#include <iostream>


auto main(int argc, char** argv) -> int {
    env env_obj(argc, argv);

    auto args = env_obj.get();

    for(auto& a : args) {
        std::cout << a << std::endl;
    }

    std::string file_name = std::string("main.s");
    Parser parser(file_name);

    parser.parse();

    return 0;
}
