#include "../../inc/linker/linker.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

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

std::map<uint32_t, uint8_t> Linker::whole_file = std::map<uint32_t, uint8_t>();

//      Everything else

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

auto Linker::concat_binary(section_struct& linker_section, section_struct& next_section) -> void {

    auto& linker_binary = linker_section.binary_data.raw;
    auto& next_binary   = next_section.binary_data.raw;

    linker_binary.insert(linker_binary.end(), next_binary.begin(), next_binary.end());
}

auto Linker::section_exits(std::string sctn) -> bool {
    auto it = section_table.find(sctn);
    return it != section_table.end();
}

auto Linker::symbol_exists(std::string sym) -> bool {
    auto it = symbol_table.find(sym);
    return it != symbol_table.end();
}

auto Linker::maping(std::string file_name) -> void {

    for(auto& section_pair : oft[file_name].section_table) {

        auto& next_section = section_pair.second;

        if(section_table.find(next_section.name) == section_table.end()) {
            // Section does not exist in linker right now

            section_table[next_section.name] = {
                Linker::next_section_idx++,
                next_section.name // empty relocation table at the start
            };

            symbol_table[next_section.name] = {
                Linker::next_symbol_idx++,
                0,
                0,
                SYMBOL_TYPE::SCTN,
                SYMBOL_BIND::LOCAL,
                section_table[next_section.name].section_idx,
                next_section.name
            };
        }

        // Remember binary size for later
        uint32_t binary_size = section_table[next_section.name].binary_data.raw.size();

        // Section exists so we need to concat binary to it
        concat_binary(section_table[next_section.name], next_section);

        for(auto& symbol_pair : oft[file_name].symbol_table) {

            auto& symbol = symbol_pair.second;

            // Check if it is in the same section
            if(symbol.ndx == next_section.section_idx) {
                symbol.value += binary_size;
                symbol.ndx    = section_table[next_section.name].section_idx;
            }
        }

        for(auto& relocation : next_section.relocations) {
            section_table[next_section.name].relocations.push_back(
                {
                    relocation.offset + binary_size,
                    relocation.type,
                    relocation.symbol_name,
                    relocation.addend
                }
            );
        }
    }
}

auto Linker::resolve_symbols(std::string file_name) -> void {

    for(auto& symbol_pair : oft[file_name].symbol_table) {
        auto& symbol = symbol_pair.second;

        if(symbol.symbol_bind == SYMBOL_BIND::LOCAL) {
            continue; // Ignore local symbols
        }

        if(symbol_exists(symbol.symbol_name)) {
            throw std::runtime_error("Linker phase 2 : symbol already defined");
        }

        symbol_table[symbol.symbol_name] = {
            Linker::next_symbol_idx++,
            symbol.value,
            0,
            symbol.symbol_type,
            symbol.symbol_bind,
            static_cast<uint32_t>(symbol.ndx),
            symbol.symbol_name
        };

    }
}

auto Linker::get_symbol_value(std::string symbol_name) -> uint32_t {

    auto it = symbol_table.find(symbol_name);

    if(it == symbol_table.end()) {
        throw std::runtime_error("Linker phase 3 :  relocation symbol not found");
    }

    return symbol_table[symbol_name].value;
}

auto Linker::resolve_everything() -> void {

    // This has to produce singular exe file
    // Goes through all section and concats them
    // While going through them updates binary size offset and value

    uint32_t binary_size = 0;
    
    std::vector<std::string> placed_sections;
    
    for(auto& place : places) {
        
        std::string section_name = place.first;
        uint32_t            addr = place.second;
        
        if(whole_file.find(addr) != whole_file.end()) {
            throw std::runtime_error(" place options overlap!");
        }
        
        placed_sections.push_back(section_name);
        
        auto& section = section_table[section_name];
        
        auto& raw = section.binary_data.raw;
        
        // New to placed section
        
        binary_size = addr;
        
        // Concat into singular file
        for(int i = 0; i < raw.size(); i++) {
            whole_file[i + binary_size] = raw[i];
        }
        
        // Update symbol table if symbol is inside this section
        for(auto& symbol_pair : symbol_table){
            auto& symbol = symbol_pair.second;
            if(symbol.symbol_name == "NULL") continue;
            symbol.value += binary_size;
        }
        
        binary_size += raw.size();
    }
    

    // Set addres for the last jumbled up section
    for(auto& section : section_table) {

        // Jump over null and already places sections
        if(section.second.name == "NULL") continue;
        
        if(std::count(placed_sections.begin(), placed_sections.end(), section.second.name) != 0) {
            continue;
        }
        
        auto& raw = section.second.binary_data.raw;

        // Concat into singular file
        for(int i = 0; i < raw.size(); i++) {
            whole_file[i + binary_size] = raw[i];
        }

        // Update symbol table if symbol is inside this section
        for(auto& symbol_pair : symbol_table){
            
            auto& symbol = symbol_pair.second;
            
            if(symbol.symbol_name == "NULL") continue;
            if(symbol.ndx != section.second.section_idx) continue;

            symbol.value += binary_size;
        }

        binary_size += raw.size();
    }
    
    // Resolve relocations

     for(auto& section : section_table) {

         uint32_t resolved = 0;
         
         uint32_t section_offset = get_symbol_value(section.second.name);

         for(auto& relocation : section.second.relocations) {
                
             whole_file[relocation.offset + section_offset] = whole_file[
                    get_symbol_value(relocation.symbol_name) + relocation.addend
                 ];
             whole_file[relocation.offset + section_offset + 1] = whole_file[
                    get_symbol_value(relocation.symbol_name) + relocation.addend + 1
                ];
             whole_file[relocation.offset + section_offset + 2] = whole_file[
                    get_symbol_value(relocation.symbol_name) + relocation.addend + 2
                ];
             whole_file[relocation.offset + section_offset + 3] = whole_file[
                    get_symbol_value(relocation.symbol_name) + relocation.addend + 3
                ];

             resolved += 1;
         }

         if(section.second.relocations.size() != resolved) {
             throw std::runtime_error("Linker error phase hex : relocation not resolved");
         }
     }
}



auto Linker::resolve_files() -> void {
    for(auto& file_name : input_files) {
       maping(file_name);
    }

    for(auto& file_name : input_files) {
       resolve_symbols(file_name);
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
        // This concats all sections and produces singular binary section
        // On top of that resolve all relocations, if some stays unresoled it throws exception
        resolve_everything();

        std::ofstream hex_file(file_name);
        
        std::map<uint32_t, uint8_t>::iterator it;
        
        for(it = whole_file.begin(); it != whole_file.end();) {
            
            auto addr = it->first;
            
            hex_file << std::setw(8) << std::setfill('0') << std::hex << (addr / 8) * 8 << ": ";
            
            if((addr % 8) != 0 ) { 
                for(int i = 0; i < (addr % 8); i+=1) {
                    hex_file << "00 ";
                }
            }
            
            uint32_t remain = 8 - (addr % 8);
            uint32_t start_addr_block = (addr / 8) * 8;
            for(int i = 0 ; i < 8 - (addr % 8) and it != whole_file.end(); i+= 1) {
                
                uint32_t new_addr_block = (it->first / 8) * 8;
                
                if(start_addr_block != new_addr_block) {
                    break;
                }
                
                hex_file << std::setw(2) << std::setfill('0') << std::hex << (int)it->second << " ";
                remain-=1;
                it++;
            }
            
            while(remain != 0) {
                hex_file << "00 ";
                remain-=1;
            }
            
            hex_file << '\n';
        }
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
