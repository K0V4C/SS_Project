#include "../../inc/linker/linker.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

//     STATIC INIT

Linker* Linker::instance = nullptr;

bool Linker::hex = false;
bool Linker::relocatable = false;
std::map<std::string, uint32_t> Linker::places = {};
std::vector<std::string> Linker::input_files = {};
std::map<std::string, Linker::object_file_descriptor> Linker::oft = {};

std::map<std::string, symbol_struct> Linker::symbol_table = std::map<std::string, symbol_struct>();
std::map<std::string, section_struct> Linker::section_table = std::map<std::string, section_struct>();

uint32_t Linker::next_symbol_idx = 0;
uint32_t Linker::next_section_idx = 0;

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

        Linker::symbol_table["NULL"] = {
            Linker::next_symbol_idx++, 0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
        };

        Linker::section_table["NULL"] = {
            Linker::next_section_idx++, "NULL"
        };
}

auto Linker::load_files() -> void {
    for(auto file_name: input_files) {
        Linker::deserialize(file_name);
    }
}

auto Linker::resolve_symbols(
    std::map<std::string, symbol_struct> symbol_table,
    uint32_t ndx,
    uint32_t new_ndx,
    uint32_t offset) -> void {

    for(auto& symbol_pair : symbol_table) {
        auto& symbol = symbol_pair.second;

        if(symbol.symbol_bind == SYMBOL_BIND::LOCAL){
            continue;
        }

        auto it = Linker::symbol_table.find(symbol.symbol_name);
        if(it != Linker::symbol_table.end()) {
            throw std::runtime_error(
              "Symbol alreadt defined" + symbol.symbol_name
            );
        }

        if(symbol.ndx == ndx) {
            Linker::symbol_table[symbol.symbol_name] = {
                Linker::next_symbol_idx++,
                symbol.value + offset,
                symbol.size,
                symbol.symbol_type,
                symbol.symbol_bind,
                new_ndx,
                symbol.symbol_name
            };
        }
    }
}

auto Linker::resolve_file(std::string& file_name) -> void {

    // We have to it this way so I can gurantee the order of section and simbols
    auto& file_sections = oft[file_name].section_table;
    auto& file_symbols = oft[file_name].symbol_table;

    // Iterate over all sections and resolve them
    for(auto& section_pair : file_sections) {
        auto& section = section_pair.second;
        auto it = Linker::section_table.find(section.name);

        if(it == Linker::section_table.end()) {
            // Section does not exist
            Linker::section_table[section.name] = {
                Linker::next_section_idx++,
                section.name
            };

            Linker::symbol_table[section.name] = {
                Linker::next_symbol_idx++,
                0,
                0,
                SYMBOL_TYPE::SCTN,
                SYMBOL_BIND::LOCAL,
                Linker::section_table[section.name].section_idx,
                section.name
            };

        }
        
        // Section exists
        uint32_t binary_size = Linker::section_table[section.name].binary_data.raw.size();

        // Add all relocations moved by this amount
        for(auto& relocation : section.relocations) {
            Linker::section_table[section.name].add_relocation(
                relocation.offset + binary_size,
                relocation.type,
                relocation.symbol_name,
                relocation.addend
            );
        }

        // Concat binary data
        auto& data = Linker::section_table[section.name].binary_data.raw;
        auto& to_add_data = section.binary_data.raw;
        data.insert(data.end(), to_add_data.begin(), to_add_data.end());

        // Now we resolve symbols for each section
        resolve_symbols(
            file_symbols,
            section.section_idx,
            Linker::section_table[section.name].section_idx,
            binary_size
        );
    }
}

auto Linker::resolve_files() -> void {
    for(auto& file_name : input_files) {
        resolve_file(file_name);
    }
}

auto Linker::produce_output(std::string file_name) -> void {
    
    if(Linker::hex and Linker::relocatable) {
        throw std::runtime_error("Can't be hex and relocatable");
    }
    
    if(Linker::relocatable) {
        serialize(file_name);
    }
    
    if(Linker::hex) {
        // Solve all relocations 
        // Serilise binary data to output todo   
    }
}

auto Linker::serialize(std::string file_name) -> void {
        
    std::ofstream binary_file(file_name, std::ios::out | std::ios::binary);
    const uint32_t symbol_table_size = Linker::symbol_table.size() - 1; // -1 for the NULL
    binary_file.write(reinterpret_cast<const char*>(&symbol_table_size), sizeof(symbol_table_size));

    for(auto e : Linker::symbol_table) {
        if(e.second.symbol_name == "NULL") continue;
        e.second.serialize(binary_file);
    }

    const uint32_t section_table_size = Linker::section_table.size() - 1; // -1 for te NULL
    binary_file.write(reinterpret_cast<const char*>(&section_table_size), sizeof(section_table_size));

    for(auto e : Linker::section_table) {
        if(e.second.name == "NULL") continue;
        e.second.serialize(binary_file);
    }
}


auto Linker::deserialize(std::string file_name) -> void {

    std::ifstream binary_file(file_name, std::ios::out | std::ios::binary);

    Linker::oft[file_name] = {};

    uint32_t symbol_table_size;
    binary_file.read(reinterpret_cast<char*>(&symbol_table_size), sizeof(symbol_table_size));

    for(int i = 0; i < symbol_table_size; i+=1) {
        auto symbol =  symbol_struct::deserialize(binary_file);
        Linker::oft[file_name].symbol_table[symbol.symbol_name] = symbol;
    }

    uint32_t section_table_size;
    binary_file.read(reinterpret_cast<char*>(&section_table_size), sizeof(section_table_size));

    for(int i = 0; i < section_table_size; i++) {
        auto section = section_struct::deserialize(binary_file);
        Linker::oft[file_name].section_table[section.name] = section;
    }
}



auto Linker::print_files() -> void {
    for(auto file : input_files) {
        Linker::print_file(file);
    }
}


auto Linker::print_file(std::string file_name) -> void {

    auto symtab = Linker::oft[file_name].symbol_table;

    std::cout << "Symbol table:\n";

    {
        std::cout   << std::setw(5)     << std::left << "idx"   << " | "
                    << std::setw(20)    << std::left << "value" << " | "
                    << std::setw(5)     << std::left << "size"  << " | "
                    << std::setw(10)    << std::left << "type"  << " | "
                    << std::setw(10)    << std::left << "bind"  << " | "
                    << std::setw(5)     << std::left << "ndx"   << " | "
                    << std::setw(36)    << std::left << "name"  << " |\n";

    }

    for(auto& e: symtab) {
        std::cout << e.second << std::endl;
    }

    // Printing section table
    auto sectab = Linker::oft[file_name].section_table;

    std::cout << "\n\nSection Table:\n";
    {
        std::cout << std::setw(40) << std::left    << "section name"        << " | "
                    << std::setw(20) << std::left    << "section idx "        << " | "
                    << std::setw(43) << std::left    << "Rel Under Section"   << " |\n";
    }

    {
        for(auto& e : sectab) {
            std::cout << e.second << std::endl;
        }
    }

}

auto Linker::print_linker_tables() -> void {

    auto symtab = Linker::symbol_table;

    std::cout << "Symbol table:\n";

    {
        std::cout   << std::setw(5)     << std::left << "idx"   << " | "
                    << std::setw(20)    << std::left << "value" << " | "
                    << std::setw(5)     << std::left << "size"  << " | "
                    << std::setw(10)    << std::left << "type"  << " | "
                    << std::setw(10)    << std::left << "bind"  << " | "
                    << std::setw(5)     << std::left << "ndx"   << " | "
                    << std::setw(36)    << std::left << "name"  << " |\n";

    }

    for(auto& e: symtab) {
        std::cout << e.second << std::endl;
    }

    // Printing section table
    auto sectab = Linker::section_table;

    std::cout << "\n\nSection Table:\n";
    {
        std::cout << std::setw(40) << std::left    << "section name"        << " | "
                    << std::setw(20) << std::left    << "section idx "        << " | "
                    << std::setw(43) << std::left    << "Rel Under Section"   << " |\n";
    }

    {
        for(auto& e : sectab) {
            std::cout << e.second << std::endl;
        }
    }
}

Linker::~Linker(){}
