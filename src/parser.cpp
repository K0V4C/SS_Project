#include "../inc/parser.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>


std::vector<std::string> Parser::token_log = std::vector<std::string>();

void yyerror(const char *s) {
  Parser::yyerror(std::string(s));
}

Parser::Parser(std::string& file_name) {
    this->input_file = fopen(file_name.c_str(), "r");
}


auto Parser::parse() -> int32_t {

    if(!this->input_file) {
        std::cerr << "Can't open input file";
        return -1;
    }

    yyin = this->input_file;

    yyparse();
    
    return 0;
}


auto Parser::log_token(std::string s) -> void {
    token_log.push_back(s);
}

auto Parser::display_log() -> void {

    std::cout << "==========PARSED FILE LOG============" << std::endl;

    for(auto& token : token_log) {
        std::cout << token + " ";
        if(token == std::string("NEW_LINE")) std::cout << std::endl;
    }

    std::cout << "\n\n===============<END>=================" << std::endl;

}

auto Parser::yyerror(const std::string& err) -> void {

    Parser::display_log();

    std::cerr << "Parser error! -> " << err <<std::endl;  
    exit(-1);
}

Parser::~Parser() {
    fclose(this->input_file);
}
