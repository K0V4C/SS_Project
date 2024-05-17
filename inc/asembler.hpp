#ifndef _ASEMBLER_HPP
#define _ASEMBLER_HPP

#include "asm_actions/actions.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <vector>
class Asembler {
public:

    static int32_t line_counter;

    static std::vector<std::unique_ptr<action>> file_actions;
    
    struct section {
        std::string name;
        
    };

    static std::map<std::string, section> section_list;

    Asembler();

    auto asemble() -> void;

    ~Asembler();

protected:

private:

};


#endif
