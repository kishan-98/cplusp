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
program:                    statement_list
                        |   {cout << "Empty file" << endl;} ;

statement_list:             statement statement_list
                        |   statement ;

statement:                  declaration_statement
                        |   assignment_statement TERMINATOR
                        |   expression TERMINATOR
                        |   control_statement
                        |   function_declaration
                        |   function_call TERMINATOR
                        |   loops
                        |   break_statement TERMINATOR
                        |   return_statement TERMINATOR ;

DATA_TYPE:                  TYPE_INT {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1);}
                        |   TYPE_FLOAT {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1);}
                        |   TYPE_CHAR {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1);}
                        |   TYPE_BOOL {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1);}
                        |   TYPE_FILE {cout << line_number << ": " << $1 << " Data Type Declaration" << endl; free($1);} ;

VALUE:                      INT | FLOAT | CHAR | BOOL ;

declaration_statement:      DATA_TYPE variable_list TERMINATOR;

variable_list:              VARIABLE ',' variable_list {cout << line_number << ": Variable declaration without definition" << endl; free($1);}
                        |   VARIABLE {cout << line_number << ": Variable declaration without definition" << endl; free($1);}
                        |   VARIABLE ASSIGNMENT expression ',' variable_list {cout << line_number << ": Variable declaration with definition" << endl; free($1);}
                        |   VARIABLE ASSIGNMENT expression {cout << line_number << ": Variable declaration with definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' ',' variable_list {cout << line_number << ": Array declaration without definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' {cout << line_number << ": Array declaration without definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Array declaration with definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Array declaration with definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ',' variable_list {cout << line_number << ": Matrix declaration without definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' {cout << line_number << ": Matrix declaration without definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {cout << line_number << ": Matrix declaration with definition" << endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' {cout << line_number << ": Matrix declaration with definition" << endl; free($1);} ;

control_statement:          IF '(' expression ')' '{' statement_list '}' {cout << line_number << ": IF stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELSE stmt" << endl;}
                        |   IF '(' expression ')' '{' statement_list '}' ELIF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        |   '(' expression ')' '?' statement ':' statement {cout << line_number << ": Conditional IF stmt" << endl;} ;

loops:                      FOR VARIABLE ASSIGNMENT expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl; free($2);}
                        |   FOR VARIABLE ASSIGNMENT expression ',' expression ',' expression '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl; free($2);}
                        |   WHILE '(' expression ')' '{' statement_list '}' {cout << line_number << ": WHILE stmt" << endl;} ;

break_statement:            BREAK {cout << line_number << ": BREAK" << endl;} ;

return_statement:           RETURN expression   {cout << line_number << ": RETURN expression" << endl;}
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
                        |   '(' expression ')'
                        |   '-' expression
                        |   VARIABLE {free($1);}
                        |   VARIABLE '[' expression ']' {free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' {free($1);}
                        |   VALUE
                        |   STRING {free($1);}
                        |   function_call
                        |   assignment_statement ;

assignment_statement:       VARIABLE ASSIGNMENT expression {cout << line_number << ": Assignment statement" << endl;} ;

parameter_list:             DATA_TYPE VARIABLE ','                  parameter_list  {free($2);}
                        |   DATA_TYPE VARIABLE                                      {free($2);}
                        |   DATA_TYPE VARIABLE '[' ']' ','          parameter_list  {free($2);}
                        |   DATA_TYPE VARIABLE '[' ']'                              {free($2);}
                        |   DATA_TYPE VARIABLE '[' ']' '[' ']' ','  parameter_list  {free($2);}
                        |   DATA_TYPE VARIABLE '[' ']' '[' ']'                      {free($2);}
                        |   ;

argument_list:              expression ',' argument_list
                        |   expression
                        |   ;

function_name:              VARIABLE    {cout << line_number << ": User-defined function" << endl; free($1);}
                        |   DATA_TYPE   {cout << line_number << ": Predefined function" << endl;} ;

function_declaration:       DATA_TYPE                   function_name '(' parameter_list ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   DATA_TYPE '[' ']'           function_name '(' parameter_list ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   DATA_TYPE '[' ']' '[' ']'   function_name '(' parameter_list ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;}
                        |   TYPE_VOID                   function_name '(' parameter_list ')' '{' statement_list '}' {cout << line_number << ": Function declaration with parameters" << endl;} ;

function_call:              function_name '(' argument_list ')' {cout << line_number << ": Function call with arguments" << endl;} ;
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
