#ifndef _LABEL_ACTIONS_HPP
#define _LABEL_ACTIONS_HPP

#include "actions.hpp"

struct label : public action {
    
    label(std::string name);

    virtual auto execute() -> void override;
    
    virtual ~label();

private:
    
    std::string symbol_name;
};



#endif // _LABEL_ACTIONS_HPP
