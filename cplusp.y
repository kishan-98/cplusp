%{
  #include <cstdio>
  #include <iostream>
  using namespace std;

  // Declare stuff from Flex that Bison needs to know about:
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  extern int line_number;
  void yyerror(const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  Initially (by default), yystype
// is merely a typedef of "int", but for non-trivial projects, tokens could
// be of any arbitrary data type.  So, to deal with that, the idea is to
// override yystype's default typedef to be a C union instead.  Unions can
// hold all of the types of tokens that Flex could return, and this this means
// we can return ints or floats or strings cleanly.  Bison implements this
// mechanism with the %union directive:
%union {
  int ival;
  float fval;
  char *sval;
  char *vval;
  char *inval;
  char *dtype;
  char cval;
  bool bval;
}

// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the %union:

/* Terminal Tokens */
%token TYPE_VOID
%token ASSIGNMENT
%token LSHIFT
%token RSHIFT
%token LT
%token GT
%token EQ
%token NE
%token LE
%token GE
%token LOGAND
%token LOGOR
%token TERMINATOR
%token IF
%token ELIF
%token ELSE
%token FOR
%token WHILE
%token RETURN
%token BREAK

/* Data Tokens */
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token <bval> BOOL
%token <sval> STRING
%token <vval> VARIABLE
%token <inval> INVALID
%token <dtype> TYPE_INT
%token <dtype> TYPE_FLOAT
%token <dtype> TYPE_CHAR
%token <dtype> TYPE_BOOL
%token <dtype> TYPE_FILE

/* Operator Associativity */
%right  ASSIGNMENT
%left   LOGOR
%left   LOGAND
%left   '|'
%left   '^'
%left   '&'
%left   EQ NE
%left   GT GE LT LE
%left   LSHIFT RSHIFT
%left   '+' '-'
%left   '*' '/' '%'
%left   '(' ')'

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:
program:                    statement_list ;

statement_list:             statement statement_list
                        |   statement ;

statement:                  declaration_statement
                        |   assignment_statement    TERMINATOR
                        |   expression              TERMINATOR
                        |   function_call           TERMINATOR
                        |   assignment_statement    TERMINATOR
                        |   control_statement
                        |   function_declaration
                        |   function_call           TERMINATOR
                        |   loops
                        |   break_statement         TERMINATOR
                        |   return_statement        TERMINATOR ;

DATA_TYPE:                  TYPE_INT {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1)}
                        |   TYPE_FLOAT {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1)}
                        |   TYPE_CHAR {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1)}
                        |   TYPE_BOOL {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1)}
                        |   TYPE_FILE {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1)} ;

VALUE:                      INT | FLOAT | CHAR | BOOL ;

declaration_statement:      DATA_TYPE variable_list TERMINATOR;

variable_list:              VARIABLE ',' variable_list {cout << line_number << ": Variable declaration without definition" << endl;}
                        |   VARIABLE {cout << line_number << ": Variable declaration without definition" << endl;}

                        |   VARIABLE ASSIGNMENT expression ',' variable_list {cout << line_number << ": Variable declaration with definition" << endl;}
                        |   VARIABLE ASSIGNMENT expression {cout << line_number << ": Variable declaration with definition" << endl;}
                        |   VARIABLE ASSIGNMENT function_call ',' variable_list {cout << line_number << ": Variable declaration with definition" << endl;}
                        |   VARIABLE ASSIGNMENT function_call {cout << line_number << ": Variable declaration with definition" << endl;}
                        |   VARIABLE ASSIGNMENT assignment_statement ',' variable_list {cout << line_number << ": Variable declaration with definition" << endl;}
                        |   VARIABLE ASSIGNMENT assignment_statement {cout << line_number << ": Variable declaration with definition" << endl;}

                        |   VARIABLE '[' expression ']' ',' variable_list {cout << line_number << ": Array declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' {cout << line_number << ": Array declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' ',' variable_list {cout << line_number << ": Array declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' {cout << line_number << ": Array declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ',' variable_list {cout << line_number << ": Array declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' {cout << line_number << ": Array declaration without definition" << endl;}

                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Array declaration with definition" << endl;}

                        |   VARIABLE '[' expression ']' '[' expression ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' {cout << line_number << ": Matrix declaration without definition" << endl;}

                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' function_call '}' {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl;}
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']' ASSIGNMENT '{' assignment_statement '}' {cout << line_number << ": Matrix declaration with definition" << endl;};

control_statement:          IF '(' expression ')' '{' statement_list '}' {cout << line_number << ": IF stmt" << endl;}
                        |   IF '(' function_call ')' '{' statement_list '}' {cout << line_number << ": IF stmt" << endl;}
                        |   IF '(' assignment_statement ')' '{' statement_list '}' {cout << line_number << ": IF stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELSE stmt" << endl;}
                        |   IF '(' function_call ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELSE stmt" << endl;}
                        |   IF '(' assignment_statement ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELSE stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELIF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELIF '(' function_call ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELIF '(' assignment_statement ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' function_call ')' '{' statement_list '}' ELIF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' function_call ')' '{' statement_list '}' ELIF '(' function_call ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' function_call ')' '{' statement_list '}' ELIF '(' assignment_statement ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' assignment_statement ')' '{' statement_list '}' ELIF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' assignment_statement ')' '{' statement_list '}' ELIF '(' function_call ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   IF '(' assignment_statement ')' '{' statement_list '}' ELIF '(' assignment_statement ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   '(' expression ')' '?' statement ':' statement {cout << line_number << ": Conditional IF stmt" << endl;}
                        |   '(' function_call ')' '?' statement ':' statement {cout << line_number << ": Conditional IF stmt" << endl;}
                        |   '(' assignment_statement ')' '?' statement ':' statement {cout << line_number << ": Conditional IF stmt" << endl;} ;

loops:                      FOR VARIABLE ASSIGNMENT expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}

                        |   FOR VARIABLE ASSIGNMENT expression ',' expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' expression ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' expression ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' function_call ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' function_call ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' function_call ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' assignment_statement ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' assignment_statement ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT expression ',' assignment_statement ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' expression ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' expression ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' function_call ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' function_call ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' function_call ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' assignment_statement ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' assignment_statement ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT function_call ',' assignment_statement ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' expression ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' expression ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' function_call ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' function_call ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' function_call ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' assignment_statement ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' assignment_statement ',' function_call '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        |   FOR VARIABLE ASSIGNMENT assignment_statement ',' assignment_statement ',' assignment_statement '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}

                        |   WHILE '(' expression ')' '{' statement_list '}' {cout << line_number << ": WHILE stmt" << endl;}
                        |   WHILE '(' function_call ')' '{' statement_list '}' {cout << line_number << ": WHILE stmt" << endl;}
                        |   WHILE '(' assignment_statement ')' '{' statement_list '}' {cout << line_number << ": WHILE stmt" << endl;} ;

break_statement:            BREAK {cout << line_number << ": BREAK" << endl;} ;

return_statement:           RETURN expression   {cout << line_number << ": RETURN expression" << endl;}
                        |   RETURN function_call   {cout << line_number << ": RETURN expression" << endl;}
                        |   RETURN assignment_statement   {cout << line_number << ": RETURN expression" << endl;}
                        |   RETURN              {cout << line_number << ": RETURN without expression" << endl;} ;

/* operator:                   ASSIGNMENT | LOGOR | LOGAND | '|' | '^' | '&' | EQ | NE | GT | GE | LT | LE | LSHIFT | RSHIFT | '+' | '-' | '*' | '/' | '%'; */

expression:                 expression ASSIGNMENT   expression
                        |   expression LOGOR        expression
                        |   expression LOGAND       expression
                        |   expression '|'          expression
                        |   expression '^'          expression
                        |   expression '&'          expression
                        |   expression EQ           expression
                        |   expression NE           expression
                        |   expression GT           expression
                        |   expression GE           expression
                        |   expression LT           expression
                        |   expression LE           expression
                        |   expression LSHIFT       expression
                        |   expression RSHIFT       expression
                        |   expression '+'          expression
                        |   expression '-'          expression
                        |   expression '*'          expression
                        |   expression '/'          expression
                        |   expression '%'          expression
                        |   expression ASSIGNMENT   function_call
                        |   expression LOGOR        function_call
                        |   expression LOGAND       function_call
                        |   expression '|'          function_call
                        |   expression '^'          function_call
                        |   expression '&'          function_call
                        |   expression EQ           function_call
                        |   expression NE           function_call
                        |   expression GT           function_call
                        |   expression GE           function_call
                        |   expression LT           function_call
                        |   expression LE           function_call
                        |   expression LSHIFT       function_call
                        |   expression RSHIFT       function_call
                        |   expression '+'          function_call
                        |   expression '-'          function_call
                        |   expression '*'          function_call
                        |   expression '/'          function_call
                        |   expression '%'          function_call
                        |   expression ASSIGNMENT   assignment_statement
                        |   expression LOGOR        assignment_statement
                        |   expression LOGAND       assignment_statement
                        |   expression '|'          assignment_statement
                        |   expression '^'          assignment_statement
                        |   expression '&'          assignment_statement
                        |   expression EQ           assignment_statement
                        |   expression NE           assignment_statement
                        |   expression GT           assignment_statement
                        |   expression GE           assignment_statement
                        |   expression LT           assignment_statement
                        |   expression LE           assignment_statement
                        |   expression LSHIFT       assignment_statement
                        |   expression RSHIFT       assignment_statement
                        |   expression '+'          assignment_statement
                        |   expression '-'          assignment_statement
                        |   expression '*'          assignment_statement
                        |   expression '/'          assignment_statement
                        |   expression '%'          assignment_statement
                        |   function_call ASSIGNMENT   expression
                        |   function_call LOGOR        expression
                        |   function_call LOGAND       expression
                        |   function_call '|'          expression
                        |   function_call '^'          expression
                        |   function_call '&'          expression
                        |   function_call EQ           expression
                        |   function_call NE           expression
                        |   function_call GT           expression
                        |   function_call GE           expression
                        |   function_call LT           expression
                        |   function_call LE           expression
                        |   function_call LSHIFT       expression
                        |   function_call RSHIFT       expression
                        |   function_call '+'          expression
                        |   function_call '-'          expression
                        |   function_call '*'          expression
                        |   function_call '/'          expression
                        |   function_call '%'          expression
                        |   function_call ASSIGNMENT   function_call
                        |   function_call LOGOR        function_call
                        |   function_call LOGAND       function_call
                        |   function_call '|'          function_call
                        |   function_call '^'          function_call
                        |   function_call '&'          function_call
                        |   function_call EQ           function_call
                        |   function_call NE           function_call
                        |   function_call GT           function_call
                        |   function_call GE           function_call
                        |   function_call LT           function_call
                        |   function_call LE           function_call
                        |   function_call LSHIFT       function_call
                        |   function_call RSHIFT       function_call
                        |   function_call '+'          function_call
                        |   function_call '-'          function_call
                        |   function_call '*'          function_call
                        |   function_call '/'          function_call
                        |   function_call '%'          function_call
                        |   function_call ASSIGNMENT   assignment_statement
                        |   function_call LOGOR        assignment_statement
                        |   function_call LOGAND       assignment_statement
                        |   function_call '|'          assignment_statement
                        |   function_call '^'          assignment_statement
                        |   function_call '&'          assignment_statement
                        |   function_call EQ           assignment_statement
                        |   function_call NE           assignment_statement
                        |   function_call GT           assignment_statement
                        |   function_call GE           assignment_statement
                        |   function_call LT           assignment_statement
                        |   function_call LE           assignment_statement
                        |   function_call LSHIFT       assignment_statement
                        |   function_call RSHIFT       assignment_statement
                        |   function_call '+'          assignment_statement
                        |   function_call '-'          assignment_statement
                        |   function_call '*'          assignment_statement
                        |   function_call '/'          assignment_statement
                        |   function_call '%'          assignment_statement
                        |   assignment_statement ASSIGNMENT   expression
                        |   assignment_statement LOGOR        expression
                        |   assignment_statement LOGAND       expression
                        |   assignment_statement '|'          expression
                        |   assignment_statement '^'          expression
                        |   assignment_statement '&'          expression
                        |   assignment_statement EQ           expression
                        |   assignment_statement NE           expression
                        |   assignment_statement GT           expression
                        |   assignment_statement GE           expression
                        |   assignment_statement LT           expression
                        |   assignment_statement LE           expression
                        |   assignment_statement LSHIFT       expression
                        |   assignment_statement RSHIFT       expression
                        |   assignment_statement '+'          expression
                        |   assignment_statement '-'          expression
                        |   assignment_statement '*'          expression
                        |   assignment_statement '/'          expression
                        |   assignment_statement '%'          expression
                        |   assignment_statement ASSIGNMENT   function_call
                        |   assignment_statement LOGOR        function_call
                        |   assignment_statement LOGAND       function_call
                        |   assignment_statement '|'          function_call
                        |   assignment_statement '^'          function_call
                        |   assignment_statement '&'          function_call
                        |   assignment_statement EQ           function_call
                        |   assignment_statement NE           function_call
                        |   assignment_statement GT           function_call
                        |   assignment_statement GE           function_call
                        |   assignment_statement LT           function_call
                        |   assignment_statement LE           function_call
                        |   assignment_statement LSHIFT       function_call
                        |   assignment_statement RSHIFT       function_call
                        |   assignment_statement '+'          function_call
                        |   assignment_statement '-'          function_call
                        |   assignment_statement '*'          function_call
                        |   assignment_statement '/'          function_call
                        |   assignment_statement '%'          function_call
                        |   assignment_statement ASSIGNMENT   assignment_statement
                        |   assignment_statement LOGOR        assignment_statement
                        |   assignment_statement LOGAND       assignment_statement
                        |   assignment_statement '|'          assignment_statement
                        |   assignment_statement '^'          assignment_statement
                        |   assignment_statement '&'          assignment_statement
                        |   assignment_statement EQ           assignment_statement
                        |   assignment_statement NE           assignment_statement
                        |   assignment_statement GT           assignment_statement
                        |   assignment_statement GE           assignment_statement
                        |   assignment_statement LT           assignment_statement
                        |   assignment_statement LE           assignment_statement
                        |   assignment_statement LSHIFT       assignment_statement
                        |   assignment_statement RSHIFT       assignment_statement
                        |   assignment_statement '+'          assignment_statement
                        |   assignment_statement '-'          assignment_statement
                        |   assignment_statement '*'          assignment_statement
                        |   assignment_statement '/'          assignment_statement
                        |   assignment_statement '%'          assignment_statement

                        |   '(' expression ')'
                        |   '(' function_call ')'
                        |   '(' assignment_statement ')'
                        |   '-' expression
                        |   '-' function_call
                        |   '-' assignment_statement
                        |   VARIABLE
                        |   VARIABLE '[' expression ']'
                        |   VARIABLE '[' function_call ']'
                        |   VARIABLE '[' assignment_statement ']'
                        |   VARIABLE '[' expression ']' '[' expression ']'
                        |   VARIABLE '[' expression ']' '[' function_call ']'
                        |   VARIABLE '[' expression ']' '[' assignment_statement ']'
                        |   VARIABLE '[' function_call ']' '[' expression ']'
                        |   VARIABLE '[' function_call ']' '[' function_call ']'
                        |   VARIABLE '[' function_call ']' '[' assignment_statement ']'
                        |   VARIABLE '[' assignment_statement ']' '[' expression ']'
                        |   VARIABLE '[' assignment_statement ']' '[' function_call ']'
                        |   VARIABLE '[' assignment_statement ']' '[' assignment_statement ']'
                        |   VALUE
                        |   STRING {free($1);} ;

assignment_statement:       VARIABLE ASSIGNMENT expression {cout << line_number << ": Assignment statement" << endl;}
                        |   VARIABLE ASSIGNMENT function_call {cout << line_number << ": Assignment statement" << endl;}
                        |   VARIABLE ASSIGNMENT assignment_statement {cout << line_number << ": Assignment statement" << endl;} ;

parameter_list:             DATA_TYPE VARIABLE ','                  parameter_list
                        |   DATA_TYPE VARIABLE
                        |   DATA_TYPE VARIABLE '[' ']' ','          parameter_list
                        |   DATA_TYPE VARIABLE '[' ']'
                        |   DATA_TYPE VARIABLE '[' ']' '[' ']' ','  parameter_list
                        |   DATA_TYPE VARIABLE '[' ']' '[' ']' ;

argument_list:              expression ',' argument_list
                        |   expression
                        |   function_call ',' argument_list
                        |   function_call
                        |   assignment_statement ',' argument_list
                        |   assignment_statement ;

function_name:              VARIABLE    {cout << line_number << ": User-defined function" << endl;}
                        |   DATA_TYPE   {cout << line_number << ": Predefined function" << endl;} ;

function_declaration:       DATA_TYPE                   function_name '(' parameter_list    ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   DATA_TYPE                   function_name '('                   ')' '{' statement_list '}' {cout << line_number << ": Function declaration without parameters" << endl;}
                        |   DATA_TYPE '[' ']'           function_name '(' parameter_list    ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   DATA_TYPE '[' ']'           function_name '('                   ')' '{' statement_list '}' {cout << line_number << ": Function declaration without parameters" << endl;}
                        |   DATA_TYPE '[' ']' '[' ']'   function_name '(' parameter_list    ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   DATA_TYPE '[' ']' '[' ']'   function_name '('                   ')' '{' statement_list '}' {cout << line_number << ": Function declaration without parameters" << endl;}
                        |   TYPE_VOID                   function_name '(' parameter_list    ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   TYPE_VOID                   function_name '('                   ')' '{' statement_list '}' {cout << line_number << ": Function declaration without parameters" << endl;} ;

function_call:              function_name '(' argument_list ')' {cout << line_number << ": Function call with arguments" << endl;}
                        |   function_name '(' ')' {cout << line_number << ": Function call without arguments" << endl;} ;
%%

int main(int, char**) {
  // Open a file handle to a particular file:
  /* FILE *myfile = fopen("a.cplusp.file", "r"); */
  // Make sure it is valid:
  /* if (!myfile) {
    cout << "I can't open a.cplusp.file!" << endl;
    return -1;
  } */
  // Set Flex to read from it instead of defaulting to STDIN:
  /* yyin = myfile; */

  // Parse through the input:
  yyparse();
  return 0;

}

void yyerror(const char *s) {
  cout << "EEK, parse error on line number " << line_number << "!  Message: " << s << endl;
  // might as well halt now:
  /* return line_number; */
  exit(line_number);
}
