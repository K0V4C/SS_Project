#include "../inc/types.hpp"

#include <map>

const char* symbol_type_2_string(SYMBOL_TYPE s) {

    const std::map<SYMBOL_TYPE, const char*> enum_strings
    {
        {SYMBOL_TYPE::NOTYP, "NOTYP"},
        {SYMBOL_TYPE::SCTN,  "SCTN"}
    };
   
    auto it  = enum_strings.find(s);
    return it == enum_strings.end() ? "Out of range" : it->second;
}

const char* symbol_bind_2_string(SYMBOL_BIND s) {

    const std::map<SYMBOL_BIND, const char*> enum_strings
    {
        {SYMBOL_BIND::LOCAL, "LOCAL"},
        {SYMBOL_BIND::EXTERN,  "EXTERN"}
    };
   
    auto it  = enum_strings.find(s);
    return it == enum_strings.end() ? "Out of range" : it->second;
}

const char* relocation_type_2_string(RELOCATION_TYPE s){

    const std::map<RELOCATION_TYPE, const char*> enum_strings
    {
        {RELOCATION_TYPE::ABS32, "ABS32"}
    };
   
    auto it  = enum_strings.find(s);
    return it == enum_strings.end() ? "Out of range" : it->second;
}
