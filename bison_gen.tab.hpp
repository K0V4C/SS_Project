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
    GLOBAL = 262,                  /* GLOBAL  */
    EXTERN = 263,                  /* EXTERN  */
    SECTION = 264,                 /* SECTION  */
    WORD = 265,                    /* WORD  */
    SKIP = 266,                    /* SKIP  */
    ASCII = 267,                   /* ASCII  */
    EQU = 268,                     /* EQU  */
    END = 269,                     /* END  */
    HALT = 270,                    /* HALT  */
    INT = 271,                     /* INT  */
    IRET = 272,                    /* IRET  */
    CALL = 273,                    /* CALL  */
    RET = 274,                     /* RET  */
    JMP = 275,                     /* JMP  */
    BEQ = 276,                     /* BEQ  */
    BNE = 277,                     /* BNE  */
    BGT = 278,                     /* BGT  */
    PUSH = 279,                    /* PUSH  */
    POP = 280,                     /* POP  */
    XCHG = 281,                    /* XCHG  */
    ADD = 282,                     /* ADD  */
    SUB = 283,                     /* SUB  */
    MUL = 284,                     /* MUL  */
    DIV = 285,                     /* DIV  */
    NOT = 286,                     /* NOT  */
    AND = 287,                     /* AND  */
    OR = 288,                      /* OR  */
    XOR = 289,                     /* XOR  */
    SHL = 290,                     /* SHL  */
    SHR = 291,                     /* SHR  */
    LD = 292,                      /* LD  */
    ST = 293,                      /* ST  */
    CSRRD = 294,                   /* CSRRD  */
    CSRRW = 295,                   /* CSRRW  */
    PLUS = 296,                    /* PLUS  */
    MINUS = 297,                   /* MINUS  */
    MULTIPLY = 298,                /* MULTIPLY  */
    DEVIDE = 299,                  /* DEVIDE  */
    LEFT_BRACKER = 300,            /* LEFT_BRACKER  */
    RIGHT_BRACKET = 301,           /* RIGHT_BRACKET  */
    DOLLAR_SIGN = 302,             /* DOLLAR_SIGN  */
    IDENTIFIER = 303,              /* IDENTIFIER  */
    REGISTER = 304,                /* REGISTER  */
    STATUS_REG = 305,              /* STATUS_REG  */
    HANDLER_REG = 306,             /* HANDLER_REG  */
    CAUSE_REG = 307,               /* CAUSE_REG  */
    SP_REG = 308,                  /* SP_REG  */
    DECIMAL_NUM = 309,             /* DECIMAL_NUM  */
    HEX_NUM = 310,                 /* HEX_NUM  */
    STRING = 311,                  /* STRING  */
    END_OF_FILE = 312              /* END_OF_FILE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "misc/bison_gen.ypp"


    int32_t num;
    char sym;

#line 127 "bison_gen.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BISON_GEN_TAB_HPP_INCLUDED  */
