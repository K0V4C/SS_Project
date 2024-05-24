#include "../inc/asembler.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <iomanip>

/*
 *
 *      STATIC INITIALISATION
 *
 */
std::string Asembler::current_section = "";

std::map<std::string, Asembler::section_struct> Asembler::section_table = std::map<std::string, Asembler::section_struct>();

std::vector<std::unique_ptr<action>> Asembler::file_actions = std::vector<std::unique_ptr<action>>();

std::map<std::string, Asembler::symbol_struct> Asembler::symbol_table = std::map<std::string, Asembler::symbol_struct>();

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
        0, 0, SYMBOL_TYPE::NOTYP, SYMBOL_BIND::LOCAL, 0, "NULL"
    };

    // Reserve first section 
    
    Asembler::section_table["NULL"] = {"NULL"};

}

auto Asembler::asemble() -> void {
    
    auto& all_actions = Asembler::file_actions;

    for(auto& action : all_actions ) {
        action->execute();
    }
}

/*
 *
 *
 *      BINRAY DATA
 *
 *
 *
 */

auto Asembler::binary_data::add_byte(uint8_t byte) -> void{
    raw.push_back(byte);
}

// TODO: fix this
auto Asembler::binary_data::add_instruction(uint32_t instr) -> void {
    raw.push_back(instr & 0xff);
    raw.push_back(
            (instr & 0xff00) >> 8 
    );
    raw.push_back (
            (instr & 0xff0000) >> 16
    );
    raw.push_back (
            (instr & 0xff000000) >> 24
    );
}

Asembler::~Asembler() {}

/*
 *
 *      SECTION TABLE 
 *
 *
 *
 */

int32_t Asembler::section_struct::next_section_idx = 0;

Asembler::section_struct::section_struct ( std::string name) 
    :   name(name),
        section_idx(section_struct::next_section_idx++),
        relocations({}),
        binary_data({}){}

auto Asembler::section_struct::add_realocation(
        uint32_t offset,
        RELOCATION_TYPE type,
        std::string symbol,
        int32_t addened) -> void {
    
    Asembler::get_current_section().relocations.push_back({offset, type, symbol, addened});
}

auto Asembler::get_section_counter() -> uint32_t {
    if (Asembler::current_section == "") return 0;

    return Asembler::section_table[Asembler::current_section].binary_data.raw.size();
}

auto Asembler::get_current_section() -> section_struct& {
    return Asembler::section_table[Asembler::current_section];
}

std::ostream& operator<<(std::ostream& os, const Asembler::section_struct& obj) {

    os  << std::left << std::setw(40) << obj.name        << " | " 
        << std::left << std::setw(20) << obj.section_idx << " | " 
        << std::left << std::setw(43) << "vvvvvvvvvv"    << " |\n";
    
    for(int i = 0; i < 111; i++ ) os << "_";
    
    os  << "\nRelocation Table:\n";

    os  << std::setw(25) << std::left << "offset"           << " | "
        << std::setw(25) << std::left << "relocation type"  << " | "
        << std::setw(25) << std::left << "symbol name"      << " | "
        << std::setw(25) << std::left << "addend"           << " | \n";
   

    for(auto& rel : obj.relocations) {
        os << rel << std::endl;
    }

      
    for(int i = 0; i < 111; i++ ) os << "^";
    os << std::endl;

    return os;
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
            e.second.relocations.push_back({});
            std::cout << e.second << std::endl;
        }
    }
};


/*
 *
 *
 *      RELOCATION TABLE
 *
 *
 */

Asembler::relocation_struct::relocation_struct()
    : offset(0), type(RELOCATION_TYPE::ABS32), symbol_name(""), addend(0) {}

Asembler::relocation_struct::relocation_struct
(
    uint32_t offset, RELOCATION_TYPE type, std::string symbol_name, int32_t addend
) : offset(offset), type(type), symbol_name(symbol_name), addend(addend){};


std::ostream& operator<<(std::ostream& os, const Asembler::relocation_struct& obj) {

    {
        os  << std::setw(25) << std::left << obj.offset                             << " | "
            << std::setw(25) << std::left << relocation_type_2_string(obj.type)     << " | "
            << std::setw(25) << std::left << obj.symbol_name                        << " | "
            << std::setw(25) << std::left << obj.addend                             << " | ";
    }


    return os;
}

/*
 *
 *
 *      SYMBOL TABLE
 *
 *
 */

int32_t Asembler::symbol_struct::next_symbol_idx = 0;

Asembler::symbol_struct::symbol_struct
(      
    uint32_t value, int32_t size, SYMBOL_TYPE symbol_type,
    SYMBOL_BIND symbol_bind, int32_t ndx, std::string symbol_name 
) : symbol_idx(symbol_struct::next_symbol_idx++),
    value(value), size(size), symbol_type(symbol_type),
    symbol_bind(symbol_bind), ndx(ndx), symbol_name(symbol_name){};

std::ostream& operator<<(std::ostream& os, const Asembler::symbol_struct& obj) {
    
    {
        std::cout   << std::setw(5)     << std::left << obj.symbol_idx                          << " | "
                    << std::setw(20)    << std::left << obj.value                               << " | "
                    << std::setw(5)     << std::left << obj.size                                << " | "
                    << std::setw(10)    << std::left << symbol_type_2_string(obj.symbol_type)   << " | "
                    << std::setw(10)    << std::left << symbol_bind_2_string(obj.symbol_bind)   << " | "
                    << std::setw(5)     << std::left << obj.ndx                                 << " | "
                    << std::setw(36)    << std::left << obj.symbol_name                         << " | ";
    }

    return os;    
}


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
