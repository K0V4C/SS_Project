/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BISON_GEN_TAB_HPP_INCLUDED
# define YY_YY_BISON_GEN_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    COMMENT = 258,                 /* COMMENT  */
    SEMYCOLON = 259,               /* SEMYCOLON  */
    COLON = 260,                   /* COLON  */
    COMMA = 261,                   /* COMMA  */
    NEW_LINE = 262,                /* NEW_LINE  */
    GLOBAL = 263,                  /* GLOBAL  */
    EXTERN = 264,                  /* EXTERN  */
    SECTION = 265,                 /* SECTION  */
    WORD = 266,                    /* WORD  */
    SKIP = 267,                    /* SKIP  */
    ASCII = 268,                   /* ASCII  */
    EQU = 269,                     /* EQU  */
    END = 270,                     /* END  */
    HALT = 271,                    /* HALT  */
    INT = 272,                     /* INT  */
    IRET = 273,                    /* IRET  */
    CALL = 274,                    /* CALL  */
    RET = 275,                     /* RET  */
    JMP = 276,                     /* JMP  */
    BEQ = 277,                     /* BEQ  */
    BNE = 278,                     /* BNE  */
    BGT = 279,                     /* BGT  */
    PUSH = 280,                    /* PUSH  */
    POP = 281,                     /* POP  */
    XCHG = 282,                    /* XCHG  */
    ADD = 283,                     /* ADD  */
    SUB = 284,                     /* SUB  */
    MUL = 285,                     /* MUL  */
    DIV = 286,                     /* DIV  */
    NOT = 287,                     /* NOT  */
    AND = 288,                     /* AND  */
    OR = 289,                      /* OR  */
    XOR = 290,                     /* XOR  */
    SHL = 291,                     /* SHL  */
    SHR = 292,                     /* SHR  */
    LD = 293,                      /* LD  */
    ST = 294,                      /* ST  */
    CSRRD = 295,                   /* CSRRD  */
    CSRWR = 296,                   /* CSRWR  */
    PLUS = 297,                    /* PLUS  */
    MINUS = 298,                   /* MINUS  */
    MULTIPLY = 299,                /* MULTIPLY  */
    DIVISION = 300,                /* DIVISION  */
    LEFT_BRACKET = 301,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 302,           /* RIGHT_BRACKET  */
    DOLLAR_SIGN = 303,             /* DOLLAR_SIGN  */
    IDENTIFIER = 304,              /* IDENTIFIER  */
    REGISTER = 305,                /* REGISTER  */
    STATUS_REG = 306,              /* STATUS_REG  */
    HANDLER_REG = 307,             /* HANDLER_REG  */
    CAUSE_REG = 308,               /* CAUSE_REG  */
    SP_REG = 309,                  /* SP_REG  */
    DECIMAL_NUM = 310,             /* DECIMAL_NUM  */
    HEX_NUM = 311,                 /* HEX_NUM  */
    STRING = 312,                  /* STRING  */
    END_OF_FILE = 313              /* END_OF_FILE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "misc/bison_gen.ypp"

    int32_t num;
    char* sym;

#line 127 "bison_gen.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BISON_GEN_TAB_HPP_INCLUDED  */
