#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <string>
#include <cstdint>

extern FILE* yyin;
extern int yyparse();
extern void yyerror(const char* s);

class Parser {
public:

    Parser(std::string& file_name);

    auto parse() -> int32_t;

    static auto yyerror(const std::string& err) -> void;

    ~Parser();

private:

    FILE *input_file = nullptr;

};


#endif // _PARSER_HPP
