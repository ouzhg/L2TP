/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     S_VARI = 258,
     S_PRED = 259,
     S_FUNC = 260,
     S_NEGA = 261,
     S_CONJ = 262,
     S_DISJ = 263,
     S_IMPL = 264,
     S_UNIV = 265,
     S_EXIS = 266,
     PERIOD = 267,
     LPAREN = 268,
     RPAREN = 269,
     LBRACKET = 270,
     RBRACKET = 271,
     COMMA = 272,
     EQUAL = 273,
     MARCO = 274
   };
#endif
/* Tokens.  */
#define S_VARI 258
#define S_PRED 259
#define S_FUNC 260
#define S_NEGA 261
#define S_CONJ 262
#define S_DISJ 263
#define S_IMPL 264
#define S_UNIV 265
#define S_EXIS 266
#define PERIOD 267
#define LPAREN 268
#define RPAREN 269
#define LBRACKET 270
#define RBRACKET 271
#define COMMA 272
#define EQUAL 273
#define MARCO 274




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 28 "parse.y"

	char* s;
	struct __formula* f;
	struct __term* t;
	struct __terms* ts;



/* Line 2068 of yacc.c  */
#line 97 "../src/parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


