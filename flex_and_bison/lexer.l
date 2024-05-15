%{
/* definitions */
#include <stdio.h>    
#include "parser.tab.hpp"

%}

/* rules for tokens */

%option nodefault
%option noyywrap
%option nounput

symbol     [a-zA-Z_][a-zA-Z0-9_]*
whitespace [ \t\r]*
new_line   [\n]
reg        %r([0-9]|1[0-5])
string     \".*\"

number_10  [0-9]*
number_16  0x[0-9A-F]*

%%

{whitespace} ; // Whitespace

#.*        {printf("COMMENT ");}

";"         {printf("SEMYCOLON ");}
":"         {printf("COLON ");}
","         {printf("COMMA ");}


\.global    {printf("GLOBAL ");}
\.extern    {printf("EXTERN ");}
\.section   {printf("SECTION ");}
\.word      {printf("WORD ");}
\.skip      {printf("SKIP ");}
\.ascii     {printf("ASCII ");} // TODO
\.equ       {printf("EQU ");}
\.end       {printf("END ");}

halt        {printf("HALT ");}

int         {printf("INT ");}
iret        {printf("IRET ");}

call        {printf("CALL ");}
ret         {printf("RET ");}

jmp         {printf("JMP ");}
beq         {printf("BEQ ");}
bne         {printf("BNE ");}
bgt         {printf("BGT ");}

push        {printf("PUSH ");}
pop         {printf("POP ");}

xchg        {printf("XCHG ");}

add         {printf("ADD ");}
sub         {printf("SUB ");}
mul         {printf("MUL ");}
div         {printf("DIV ");}

not         {printf("NOT ");}
and         {printf("AND ");}
or          {printf("OR ");}
xor         {printf("XOR ");}

shl         {printf("SHL ");}
shr         {printf("SHR ");}

ld          {printf("LD ");}
st          {printf("ST ");}

csrrd       {printf("CSRRD ");}
csrrw       {printf("CSRRW ");}

"+"         {printf("PLUS ");}
"-"         {printf("MINUS ");}
"*"         {printf("MULTIPLY ");}
"/"         {printf("DEVIDE ");}
"["         {printf("LEFT_BRACKET ");}
"]"         {printf("RIGHT_BRACKET ");}
"$"         {printf("DOLLAR_SIGN ");}

{symbol}    {printf("IDENTIFIER ");}
{new_line}  {printf("\n");}
{reg}       {printf("REGISTER ");}
%status     {printf("STATUS_REG ");}
%handler    {printf("HANDLER_REG ");}
%cause      {printf("CAUSE_REG ");}
%sp         {printf("SP_REG ");}

{number_10} {printf("DECIMAL_NUM ");}
{number_16} {printf("HEX_NUM ");}

{string}    {printf("STRING ");}

.         {printf("ERROR ");}

%%
/* C code and functions here */
/* Driver function used fot testing, uncomment if u only want to use Lexer */
//int main() { yylex();}
