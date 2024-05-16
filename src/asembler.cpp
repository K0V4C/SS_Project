#include "../inc/asembler.hpp"
#include <memory>
#include <iostream>
#include <vector>

std::vector<std::unique_ptr<action>> Asembler::file_actions = std::vector<std::unique_ptr<action>>();

Asembler::Asembler() {}

auto Asembler::asemble() -> void {
    
    auto& all_actions = Asembler::file_actions;

    for(auto& action : all_actions ) {
        action->execute();
    }
}

Asembler::~Asembler() {}
