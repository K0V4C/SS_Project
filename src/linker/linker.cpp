#include "inc/linker/linker.hpp"

//     STATIC INIT

Linker* Linker::instance = nullptr;

bool Linker::hex = false;
bool Linker::relocatable = false;
std::map<std::string, uint32_t> Linker::places = {};
std::vector<std::string> Linker::input_files = {};
std::map<std::string, Linker::object_file_descriptor> Linker::ofd = {};

//     struct object_file_descriptor {

//         std::map<std::string, symbol_struct> symbol_table;
//         std::map<std::string, section_struct> section_table;

//     };


Linker::Linker(
    std::map<std::string, uint32_t> places,
    std::vector<std::string> input_files,
    bool hex, bool relocatable
) {
    Linker::places = places;
    Linker::input_files = input_files;
    Linker::hex = hex;
    Linker::relocatable = relocatable;
}

auto Linker::get_instance() -> Linker& {

    return *Linker::instance;
}

auto Linker::init(
    std::map<std::string, uint32_t> places,
    std::vector<std::string> input_files,
    bool hex, bool relocatable) -> void {

        Linker::instance = new Linker (
            places, input_files, hex, relocatable
        );
}

auto Linker::deserialize(std::string file_name) -> void {
    // TODO:
} 


Linker::~Linker() {};
