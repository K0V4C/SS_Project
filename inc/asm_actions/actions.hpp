#ifndef _ACTIONS_HPP
#define _ACTIONS_HPP

// purely abstract
#include <string>
#include <vector>
struct action {
   
    action();

    virtual auto execute() -> void = 0;
    
    ~action();
};

#endif
