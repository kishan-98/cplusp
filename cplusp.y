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
  char *isval;
  char cval;
  bool bval;
}

// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the %union:
/* Terminal Tokens */

%token TYPE_INT
%token TYPE_FLOAT
%token TYPE_CHAR
%token TYPE_BOOL
%token UNARY
%token BINARY
%token ARITHMATIC
%token LOGICAL
%token ASSIGNMENT
%token TERMINATOR
%token IF
%token ELIF
%token ELSE
%token FOR
%token WHILE

/* Data Tokens */
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token <bval> BOOL
%token <sval> STRING
%token <vval> VARIABLE
%token <isval> INVALID

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:
program:    statement_list;

statement_list: statement_list statement | statement;

statement:      declaration_statement
            |   assignment_statement
            |   expression_statement
            |   control_statement
            |   loops ;

DATA_TYPE:  TYPE_INT | TYPE_FLOAT | TYPE_CHAR | TYPE_BOOL;

VALUE:  INT | FLOAT | CHAR | BOOL;

declaration_statement:      DATA_TYPE VARIABLE TERMINATOR {cout << line_number << ": Declaration without definition" << endl;}
                        |   DATA_TYPE VARIABLE ASSIGNMENT VALUE TERMINATOR {cout << line_number << ": Declaration with definition" << endl;};

control_statement:        IF '(' expression ')' '{' statement_list '}' {cout << line_number << ": IF stmt" << endl;}
                        | IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' {cout << line_number << ": IF ELSE stmt" << endl;}
                        | IF '(' expression ')' '{' statement_list '}' ELIF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}' 
                                {cout << line_number << ": IF ELIF ELSE stmt" << endl;}
                        | '(' expression ')' '?' statement ':' statement {cout << line_number << ": Conditional IF stmt" << endl;};

loops:                    FOR VARIABLE ASSIGNMENT VALUE ',' VALUE '{' statement_list '}' {cout << line_number << ": FOR(v,v) stmt" << endl;}
                        | FOR VARIABLE ASSIGNMENT VALUE ',' VALUE ',' VALUE '{' statement_list '}' {cout << line_number << ": FOR(v,v,v) stmt" << endl;}
                        | WHILE '(' expression ')' '{' statement_list '}' {cout << line_number << ": WHILE stmt" << endl;} ;

operator:               UNARY | BINARY | ARITHMATIC | LOGICAL | ASSIGNMENT;

expression:             expression operator expression | VARIABLE | VALUE;

expression_statement:   expression TERMINATOR;

assignment_statement:   VARIABLE ASSIGNMENT expression TERMINATOR;
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

}

void yyerror(const char *s) {
  cout << "EEK, parse error on line number " << line_number << "!  Message: " << s << endl;
  // might as well halt now:
  exit(-1);
}
