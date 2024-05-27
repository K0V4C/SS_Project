#ifndef _LINKER_HPP
#define _LINKER_HPP

#include "inc/common/binary_data.hpp"
#include "inc/common/relocation.hpp"
#include "inc/common/section.hpp"
#include "inc/common/symbol.hpp"
#include "inc/common/types.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <map>

#include <fstream>

class Linker {


    static auto get_instance() -> Linker&;
    static auto init(
        std::map<std::string, uint32_t> places,
        std::vector<std::string> input_files,
        bool hex, bool relocatable
    ) -> void;

    auto deserialize(std::string file_name) -> void;

    struct object_file_descriptor {

        std::map<std::string, symbol_struct> symbol_table;
        std::map<std::string, section_struct> section_table;

    };

    static std::map<std::string, object_file_descriptor> ofd;

    static bool hex;
    static bool relocatable;

    static std::map<std::string, uint32_t> places;
    static std::vector<std::string> input_files;

private:

    static Linker* instance;

    Linker(
        std::map<std::string, uint32_t> places,
        std::vector<std::string> input_files,
        bool hex, bool relocatable
    );

};

#endif // _LINKER_HPP