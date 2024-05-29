#ifndef _ENV_LINKER_HPP
#define _ENV_LINKER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct env {

    env(int32_t argc, char** argv);

    auto get() const -> std::vector<std::string>;

    auto get_file_name() const -> std::string;
    auto get_input_files() const-> std::vector<std::string>;
    auto get_places() const -> std::map<std::string, uint32_t>;

    static bool hex;
    static bool relocatable;

    ~env();

private:
    std::vector<std::string> input_files;
    std::map<std::string, uint32_t> places;
    std::string file_name = "";

};
#endif
