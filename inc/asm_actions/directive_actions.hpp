#ifndef _DIRECTIVE_ACTIONS_HPP
#define _DIRECTIVE_ACTIONS_HPP

#include "actions.hpp"

struct directive_extern : public action {
    
    directive_extern(std::vector<std::string> symbol_list);

    virtual auto execute() -> void override;
    
    virtual ~directive_extern();

private:
    
    std::vector<std::string> symbol_list;

};

#endif // _DIRECTIVE_ACTIONS_HPP