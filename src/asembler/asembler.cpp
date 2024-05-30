#include "../../inc/asembler/asembler.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <fstream>

std::vector<std::unique_ptr<action>> Asembler::file_actions = std::vector<std::unique_ptr<action>>();

std::map<std::string, Asembler::forward_struct> Asembler::forward_table = std::map<std::string, Asembler::forward_struct>();

std::map<std::string, symbol_struct> Asembler::symbol_table = std::map<std::string, symbol_struct>();

/*
 *
 *
 *          BASE INIT FOR ASEMBLER CLASS
 *
 *
 *
 */

Asembler::Asembler() {
    // Init first entry in symbol_table
    Asembler::symbol_table["NULL"] = {
        Asembler::next_symbol_idx++,
        0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
    };

    // Reserve first section
    Asembler::section_table["NULL"] = {
        Asembler::next_section_idx++,
        "NULL"
    };

}

auto Asembler::asemble() -> void {

    auto& all_actions = Asembler::file_actions;

    for(auto& action : all_actions ) {
        action->execute();
    }
}

auto Asembler::symbol_exists(std::string symbol_name) -> bool {

    for(auto& symbol_pair : symbol_table) {

        auto& symbol = symbol_pair.second;

        if(symbol.symbol_name == symbol_name) return true;
    }

    return false;
}

auto Asembler::get_symbol(std::string symbol_name) -> symbol_struct {

    for(auto& symbol_pair : symbol_table) {

           auto& symbol = symbol_pair.second;

           if(symbol.symbol_name == symbol_name) {
               return symbol;
           }
    }

    return {
        0, 0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
    };
}

auto Asembler::section_exists(uint32_t ndx) -> bool {

    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        if(section.section_idx == ndx) return true;
    }

    return false;
}

auto Asembler::get_section(uint32_t ndx) -> section_struct {
    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        if(section.section_idx == ndx) return section;
    }

    return {
        0, "NULL"
    };
}

auto Asembler::backpatch() -> void {

    // There are only 2 jobs right now
    // First one is to prepare relocations
    // Second one is optional to resolve relocaitons local to one section


    // Prepare relocations

    for(auto& section_pair : section_table) {

        auto& section = section_pair.second;

        for(auto& relocation : section.relocations) {
            
            if(!symbol_exists(relocation.symbol_name)) {
                throw std::runtime_error(" Backpatching relocations symbol does not exits");
            }

            auto symbol = get_symbol(relocation.symbol_name);

            if( symbol.symbol_bind == SYMBOL_BIND::GLOBAL or symbol.symbol_bind == SYMBOL_BIND::EXTERN) {
                continue;
            }

            if(symbol.symbol_bind == SYMBOL_BIND::LOCAL and symbol.ndx == 0) {
                throw std::runtime_error("Backpatching Undefined symbol");
            }

            if(!section_exists(symbol.ndx)) {
                throw std::runtime_error(" Backpatching relocations section does not exist");
            }

            auto section = get_section(symbol.ndx);

            relocation.symbol_name = section.name;
            relocation.addend = symbol.value;
        }
    }

    // TODO: resolve in same section relocations
    // This is eaisly done now


}

auto Asembler::serialize(std::string file_name) -> void {

    std::ofstream binary_file(file_name, std::ios::out | std::ios::binary);
    const uint32_t symbol_table_size = symbol_table.size() - 1; // -1 for the NULL
    binary_file.write(reinterpret_cast<const char*>(&symbol_table_size), sizeof(symbol_table_size));

    for(auto e : symbol_table) {
         if(e.second.symbol_name == "NULL") continue;
        e.second.serialize(binary_file);
    }

    const uint32_t section_table_size = section_table.size() - 1; // -1 for te NULL
    binary_file.write(reinterpret_cast<const char*>(&section_table_size), sizeof(section_table_size));

    for(auto e : section_table) {
        if(e.second.name == "NULL") continue;
        e.second.serialize(binary_file);
    }

}

Asembler::~Asembler() {}

/*
 *
 *      FORWARD TABLE
 *
 *
 *
 */

Asembler::forward_struct::forward_struct(){}

// This should be called on every place that is using a symbol
auto Asembler::link_symbol(std::string symbol, uint32_t location) -> void {

    // Check if it is defined
    // If it defined we don't do anything anymore everything will be resolved at the end

    auto it = Asembler::symbol_table.find(symbol);
    if(it != Asembler::symbol_table.end()) {
        return;
    }

    // If it is not defined add it to table

    auto new_entry = Asembler::forward_table.find(symbol);

    if(new_entry == Asembler::forward_table.end()) {
        Asembler::forward_table[symbol] = {};
    }

    Asembler::forward_table[symbol].symbol_name = symbol;
    Asembler::forward_table[symbol].locations.push_back(location);

}

std::ostream& operator<<(std::ostream& os, const Asembler::forward_struct& obj) {

     {
        os      << std::setw(20)    << obj.symbol_name << "  | ";

        auto i = 0;

        for (auto e : obj.locations) {

            if(i++ >= 9) {  os << " ... "; break;   }

            os  << std::setw(10)    << e    << "| ";
        }
    }

    return os;
}


auto Asembler::print_forward_table() -> void {

    std::cout << "\n\nForward Table:\n";
    {
        std::cout << std::setw(20) << std::left    << "Symbol"        << "  | "
                  << std::setw(85) << std::left    << "Values"         << " | \n";
    }


    {
        for(auto& e : Asembler::forward_table) {
            std::cout << e.second << std::endl;
        }
    }
}


Asembler::forward_struct::~forward_struct(){}



/*
 *
 *      SECTION TABLE
 *
 *
 *
 */

std::string Asembler::current_section = "";

uint32_t Asembler::next_section_idx = 0;

std::map<std::string, section_struct> Asembler::section_table = std::map<std::string, section_struct>();

auto Asembler::get_section_counter() -> uint32_t {
    if (Asembler::current_section == "") return 0;

    return Asembler::section_table[Asembler::current_section].binary_data.raw.size();
}

auto Asembler::get_current_section() -> section_struct& {
    return Asembler::section_table[Asembler::current_section];
}


auto Asembler::print_section_table() -> void {

    std::cout << "\n\nSection Table:\n";
    {
        std::cout << std::setw(40) << std::left    << "section name"        << " | "
                  << std::setw(20) << std::left    << "section idx "        << " | "
                  << std::setw(43) << std::left    << "Rel Under Section"   << " |\n";
    }

    {
        for(auto& e : Asembler::section_table) {
            std::cout << e.second << std::endl;
        }
    }
};

/*

    SYMBOL TABLE

*/

uint32_t Asembler::next_symbol_idx = 0;

auto Asembler::print_symbol_table() -> void {

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

    for(auto& e: Asembler::symbol_table) {
        std::cout << e.second << std::endl;
    }

}
