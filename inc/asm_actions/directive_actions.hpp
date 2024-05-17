#ifndef _DIRECTIVE_ACTIONS_HPP
#define _DIRECTIVE_ACTIONS_HPP

#include "actions.hpp"
#include <cstdint>

struct directive_global_and_extern : public action {
    
    directive_global_and_extern(std::vector<std::string> symbol_list);

    virtual auto execute() -> void override;
    
    virtual ~directive_global_and_extern();

private:
    
    std::vector<std::string> symbol_list;

};

struct directive_section : public action {

    directive_section(std::string section_name);

    virtual auto execute() -> void override;

    virtual ~directive_section();

private:
    std::string section_name;
};

struct directive_skip : public action {

    directive_skip(int32_t);

    virtual auto execute() -> void override;

    virtual ~directive_skip();

private:
    int32_t skip_leap;
};

#endif // _DIRECTIVE_ACTIONS_HPP
