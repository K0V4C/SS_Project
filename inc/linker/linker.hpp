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

    // Creating Linker unit
    //
    //
    //
    //


    static auto init(
        std::map<std::string, uint32_t> places,
        std::vector<std::string> input_files,
        bool hex, bool relocatable
    ) -> void;

    static auto get_instance() -> Linker&;


    // Static data
    //
    //
    //
    //

    static std::map<std::string, section_struct> section_table;
    static std::map<std::string, symbol_struct>  symbol_table;

    static uint32_t next_symbol_idx;
    static uint32_t next_section_idx;

    static bool hex;
    static bool relocatable;

    static std::map<std::string, uint32_t> places;
    static std::vector<std::string> input_files;

    static std::map<uint32_t, uint8_t> whole_file;

    // Linker internals
    //
    // : loading files
    // : printing files
    //

    auto deserialize(std::string file_name) -> void;
    auto serialize(std::string file_name) -> void;

    struct object_file_descriptor {

        std::map<std::string, symbol_struct> symbol_table;
        std::map<std::string, section_struct> section_table;

    };

    static std::map<std::string, Linker::object_file_descriptor> oft;

    // Linker main job
    //
    // : helper -> concat_binary
    // : helper -> get_symbol_value
    // : helper -> section_exists
    // : helper -> symbol_exits

    auto symbol_exists(std::string) -> bool;
    auto section_exits(std::string) -> bool;

    auto concat_binary(section_struct& linker_section, section_struct& new_section) -> void;
    auto get_symbol_value(std::string) -> uint32_t;

    auto load_files() -> void;
    auto resolve_files() -> void;
    auto maping(std::string file_name) -> void;
    auto resolve_symbols(std::string file_name) -> void;
    auto resolve_everything() -> void;


    // Output
    //
    //
    //
    //
    auto produce_output(std::string file_name) -> void;
    auto print_files() -> void;
    auto print_file(std::string file_name) -> void;
    auto print_linker_tables() -> void;

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
