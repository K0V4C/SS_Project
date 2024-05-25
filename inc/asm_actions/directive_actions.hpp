#ifndef _DIRECTIVE_ACTIONS_HPP
#define _DIRECTIVE_ACTIONS_HPP

#include "actions.hpp"
#include <cstdint>
#include <variant>

struct directive_global : public action {
    
    directive_global(std::vector<std::string> symbol_list);

    virtual auto execute() -> void override;
    
    virtual ~directive_global();

private:
    
    std::vector<std::string> symbol_list;

};

struct directive_extern : public action {
    
    directive_extern(std::vector<std::string> symbol_list);

    virtual auto execute() -> void override;
    
    virtual ~directive_extern();

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


// TODO SECTION START 
// ALSO EDIT BISON DONT FORGOR


struct directive_word: public action {

    directive_word(std::vector<std::variant<std::string,int32_t>>);

    virtual auto execute() -> void override;

    virtual ~directive_word();

private:

    std::vector<std::variant<std::string,int32_t>> symbols_and_literals;

};

struct directive_ascii: public action {

    directive_ascii(std::string);

    virtual auto execute() -> void override;

    virtual ~directive_ascii();

private:

    std::string ascii_string;

};

struct directive_equ: public action {

    directive_equ(std::vector<std::variant<std::string,int32_t>>);

    virtual auto execute() -> void override;

    virtual ~directive_equ();

private:

    std::vector<std::variant<std::string,int32_t>> symbols_and_literals;
};


struct directive_end: public action {

    directive_end();

    virtual auto execute() -> void override;

    virtual ~directive_end();

private:
};

#endif // _DIRECTIVE_ACTIONS_HPP
