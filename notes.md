#### Notes for bison and flex

- General notes
- yyerror is called every time parser finds and error
- yytext is currently processed string
- yylval -> used to store the number corespoding to token -> defined by union

- this function is called each time at end of lexer and is used to wrap up

- Driver function (main) is not needed if it works with bison
- Same goes for bison it can be called from asembler
