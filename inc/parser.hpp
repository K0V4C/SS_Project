#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <string>
#include <cstdint>
#include <vector>

extern FILE* yyin;
extern int yyparse();
extern void yyerror(const char* s);

class Parser {
public:

    Parser(std::string& file_name);

    auto parse() -> int32_t;

    static auto yyerror(const std::string& err) -> void;

    // These are used for  logging lexer phase
    static auto log_token(std::string) -> void;
    static auto display_log() -> void;

    ~Parser();

private:
    
    static std::vector<std::string> token_log;

    FILE *input_file = nullptr;

};


#endif // _PARSER_HPP
