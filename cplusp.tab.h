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
     TYPE_VOID = 258,
     UNARY = 259,
     BINARY = 260,
     ARITHMATIC = 261,
     LOGICAL = 262,
     ASSIGNMENT = 263,
     TERMINATOR = 264,
     IF = 265,
     ELIF = 266,
     ELSE = 267,
     FOR = 268,
     WHILE = 269,
     RETURN = 270,
     BREAK = 271,
     INT = 272,
     FLOAT = 273,
     CHAR = 274,
     BOOL = 275,
     STRING = 276,
     VARIABLE = 277,
     INVALID = 278,
     TYPE_INT = 279,
     TYPE_FLOAT = 280,
     TYPE_CHAR = 281,
     TYPE_BOOL = 282,
     TYPE_FILE = 283
   };
#endif
/* Tokens.  */
#define TYPE_VOID 258
#define UNARY 259
#define BINARY 260
#define ARITHMATIC 261
#define LOGICAL 262
#define ASSIGNMENT 263
#define TERMINATOR 264
#define IF 265
#define ELIF 266
#define ELSE 267
#define FOR 268
#define WHILE 269
#define RETURN 270
#define BREAK 271
#define INT 272
#define FLOAT 273
#define CHAR 274
#define BOOL 275
#define STRING 276
#define VARIABLE 277
#define INVALID 278
#define TYPE_INT 279
#define TYPE_FLOAT 280
#define TYPE_CHAR 281
#define TYPE_BOOL 282
#define TYPE_FILE 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "cplusp.y"
{
  int ival;
  float fval;
  char *sval;
  char *vval;
  char *inval;
  char *dtype;
  char cval;
  bool bval;
}
/* Line 1529 of yacc.c.  */
#line 116 "cplusp.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

