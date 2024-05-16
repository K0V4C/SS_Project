#ifndef _ASEMBLER_HPP
#define _ASEMBLER_HPP

#include "asm_actions/actions.hpp"

#include <memory>
#include <vector>
class Asembler {
public:

    static std::vector<std::unique_ptr<action>> file_actions;

    Asembler();

    auto asemble() -> void;

    ~Asembler();

protected:

private:

};


#endif
