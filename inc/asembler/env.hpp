#ifndef _ENV_HPP
#define _ENV_HPP

#include<vector>
#include<string>

#include<cstdint>

// used for interaction with env 
struct env {

    env(int32_t argc, char** argv);

    auto get() const -> std::vector<std::string>;

    auto input_file() const -> std::string;

    auto output_file() const -> std::string;

private:
    std::vector<std::string> cmd_args;

};

#endif //_ENV_HPP
