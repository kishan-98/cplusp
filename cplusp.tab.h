/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     TYPE_INT = 258,
     TYPE_FLOAT = 259,
     TYPE_CHAR = 260,
     TYPE_BOOL = 261,
     UNARY = 262,
     BINARY = 263,
     ARITHMATIC = 264,
     LOGICAL = 265,
     ASSIGNMENT = 266,
     TERMINATOR = 267,
     IF = 268,
     ELIF = 269,
     ELSE = 270,
     FOR = 271,
     WHILE = 272,
     INT = 273,
     FLOAT = 274,
     CHAR = 275,
     BOOL = 276,
     STRING = 277,
     VARIABLE = 278,
     INVALID = 279
   };
#endif
/* Tokens.  */
#define TYPE_INT 258
#define TYPE_FLOAT 259
#define TYPE_CHAR 260
#define TYPE_BOOL 261
#define UNARY 262
#define BINARY 263
#define ARITHMATIC 264
#define LOGICAL 265
#define ASSIGNMENT 266
#define TERMINATOR 267
#define IF 268
#define ELIF 269
#define ELSE 270
#define FOR 271
#define WHILE 272
#define INT 273
#define FLOAT 274
#define CHAR 275
#define BOOL 276
#define STRING 277
#define VARIABLE 278
#define INVALID 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "cplusp.y"
{
  int ival;
  float fval;
  char *sval;
  char *vval;
  char *isval;
  char cval;
  bool bval;
}
/* Line 1529 of yacc.c.  */
#line 107 "cplusp.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

