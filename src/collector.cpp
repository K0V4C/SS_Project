#include "../inc/collector.hpp"

Collector* Collector::instance = nullptr;

Collector::Collector() {}

Collector::~Collector() {}


auto Collector::get_instance() -> Collector& {
    
    if(instance == nullptr)
        instance = new Collector();
    
    return *instance;
}

auto Collector::append_symbol(std::string symbol) -> void {
   this->symbol_list.push_back(symbol); 
}

auto Collector::get_symbol_list() -> std::vector<std::string> {
    return this->symbol_list;
}
auto Collector::clear_symbol_list() -> void {
    this->symbol_list.clear();
}
