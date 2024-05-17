#include "../inc/asembler.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <iostream>
#include <vector>

int32_t Asembler::line_counter = 0;
std::map<std::string, Asembler::section> Asembler::section_list = std::map<std::string, Asembler::section>();
std::vector<std::unique_ptr<action>> Asembler::file_actions = std::vector<std::unique_ptr<action>>();

Asembler::Asembler() {}

auto Asembler::asemble() -> void {
    
    auto& all_actions = Asembler::file_actions;

    for(auto& action : all_actions ) {
        action->execute();
    }
}

Asembler::~Asembler() {}
