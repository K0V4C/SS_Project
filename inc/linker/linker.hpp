#ifndef _LINKER_HPP
#define _LINKER_HPP

#include "../common/section.hpp"
#include "../common/symbol.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <map>

class Linker {
public:
    static auto init(
        std::map<std::string, uint32_t> places,
        std::vector<std::string> input_files,
        bool hex, bool relocatable
    ) -> void;

    static auto get_instance() -> Linker&;

    auto deserialize(std::string file_name) -> void;
    auto serialize(std::string file_name) -> void;

    struct object_file_descriptor {

        std::map<std::string, symbol_struct> symbol_table;
        std::map<std::string, section_struct> section_table;

    };

    static std::map<std::string, Linker::object_file_descriptor> oft;

    auto load_files() -> void;

    static bool hex;
    static bool relocatable;

    static std::map<std::string, uint32_t> places;
    static std::vector<std::string> input_files;

    auto print_files() -> void;
    auto print_file(std::string file_name) -> void;
    auto print_linker_tables() -> void;

    auto resolve_files() -> void;
    auto resolve_file(std::string&) -> void;
    auto resolve_symbols(std::map<std::string, symbol_struct>, uint32_t, uint32_t, uint32_t)-> void;

    auto produce_output(std::string file_name) -> void;

    static std::map<std::string, section_struct> section_table;
    static std::map<std::string, symbol_struct>  symbol_table;

    static uint32_t next_symbol_idx;
    static uint32_t next_section_idx;

    ~Linker();

private:

    static Linker* instance;

    Linker(
        std::map<std::string, uint32_t> places,
        std::vector<std::string> input_files,
        bool hex, bool relocatable
    );

};

#endif // _LINKER_HPP
