#ifndef _COLLECTOR_HPP
#define _COLLECTOR_HPP

#include <string>
#include <vector>

class Collector {
public:
 
    static auto get_instance() -> Collector&;

    auto append_symbol(std::string) -> void;
    auto get_symbol_list() -> std::vector<std::string>;
    auto clear_symbol_list() -> void;

    Collector(Collector&) = delete;
    void operator=(Collector&) = delete;

    ~Collector();
protected:
    
    static Collector* instance;

    std::vector<std::string> symbol_list;

    Collector();

};

#endif // _COLLECTOR_HPP
