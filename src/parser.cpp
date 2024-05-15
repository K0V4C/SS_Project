#include "../inc/parser.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>


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

auto Parser::yyerror(const std::string& err) -> void {

    std::cerr << "Parser error! -> " << err <<std::endl;  
    exit(-1);
}

Parser::~Parser() {
    fclose(this->input_file);
}
