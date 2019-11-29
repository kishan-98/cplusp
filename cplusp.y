%{
  #include "cplusp.h"
  using namespace std;

  // Declare stuff from Flex that Bison needs to know about:
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  extern int line_number;
  program_node *root;
  void yyerror(const char *s);
  map<string, pair<int, int> > data_groups;
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
  char cval;
  bool bval;
  char *sval;
  char *vval;
  char *inval;
  char *datatype;
  program_node *prog;
  statement_list_node *stmt_list;
  statement_node *stmt;
  expression_node *exp_node;
  assignment_statement_node *assign_node;
  value_node *val_node;
  declaration_statement_node *decl_node;
  control_statement_node *ctrl_node;
  std::list<assignment_statement_node *> *asgn_list;
  if_statement_node *if_node;
  loop_statement_node *loop_node;
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

/* Terminal Data Tokens */
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token <bval> BOOL
%token <sval> STRING
%token <vval> VARIABLE
%token <inval> INVALID
%token <datatype> TYPE_INT
%token <datatype> TYPE_FLOAT
%token <datatype> TYPE_CHAR
%token <datatype> TYPE_BOOL
%token <datatype> TYPE_FILE

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
%left   UMINUS
%nonassoc '!' '~'
%left   '['']'
%left   '(' ')'

/* Non-Terminal Typed tokens */
%type <prog>            program
%type <stmt_list>       statement_list
%type <stmt>            statement_block         statement
%type <exp_node>        expression
%type <assign_node>     assignment_statement
%type <val_node>        value
%type <decl_node>       declaration_statement
%type <ctrl_node>       control_statement       tertiary_statement
%type <datatype>        data_type
%type <if_node>         else_statement
%type <loop_node>       loops

%start program /* Used to describe which is the initial grammar state */
%error-verbose

%%
// This is the real grammar that bison will parse:

statement_list:             statement_list statement_block { $1->insert_statement($2); $$ = $1; }
                        |   statement_block { $$ = new statement_list_node(new std::list<statement_node *>(1, $1)); }
                        |   statement_list error { $$ = $1; yyclearin; }
                        ;

statement_block:            assignment_statement TERMINATOR { $1->setTerminatorChar("\n"); $1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ $$ = $1; }
                        |   declaration_statement TERMINATOR { $1->setTerminatorChar("\n"); /*$1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ $$ = $1; }
                        |   tertiary_statement TERMINATOR { $1->setTerminatorChar("\n"); /*$1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ $$ = $1; }
                        |   control_statement { $1->setTerminatorChar("\n"); /*$1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ $$ = $1; }
                        |   loops { $1->setTerminatorChar("\n"); /*$1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ $$ = $1; }
                        |   expression TERMINATOR { $$ = new expression_statement_node($1); $$->setTerminatorChar("\n"); /*$1->setPrintStatement(true); /*$1->print(); $1->evaluate();*/ }
                        ;
                        /* |   function_declaration
                        |   function_call TERMINATOR
                        |   break_statement TERMINATOR
                        |   return_statement TERMINATOR */

statement:                  assignment_statement { $$ = $1; }
                        |   control_statement { $$ = $1; }
                        |   expression { $$ = new expression_statement_node($1); }
                        ;
                        /* |   function_call
                        |   break_statement
                        |   return_statement */

data_type:                  TYPE_INT
                        |   TYPE_FLOAT
                        |   TYPE_CHAR
                        |   TYPE_BOOL
                        |   TYPE_FILE
                        ;

value:                      INT { $$ = new value_node($1); }
                        |   FLOAT { $$ = new value_node($1); }
                        |   CHAR { $$ = new value_node($1); }
                        |   BOOL { $$ = new value_node($1); }
                        ;
assignment_statement:       VARIABLE ASSIGNMENT expression {std::cout << line_number << ": Assignment statement" << std::endl; $$ = new assignment_statement_node($1, $3); } ;
                        /* |   VARIABLE ASSIGNMENT expression ',' assignment_statement {std::cout << line_number << ": Assignment statement" << std::endl; $$ = new assignment_statement_node($1, $3); } ; */
                        |   VARIABLE '[' expression ']' ASSIGNMENT expression {std::cout << line_number << ": Array Assignment statement" << std::endl; $$ = new assignment_statement_node($1, $3, $6); }
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT expression {std::cout << line_number << ": Array Assignment statement" << std::endl; $$ = new assignment_statement_node($1, $3, $6, $9); }
                        ;
                        /* |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' ',' assignment_statement {std::cout << line_number << ": Array declaration with definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' assignment_statement {std::cout << line_number << ": Matrix declaration with definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' {std::cout << line_number << ": Matrix declaration with definition" << std::endl; free($1);} */

declaration_statement:      data_type VARIABLE ASSIGNMENT expression {std::cout << line_number << ": Declaration statement with assignment" << std::endl; $$ = new declaration_statement_node($1, $2, $4); }
                        |   data_type VARIABLE {std::cout << line_number << ": Declaration statement without assignment" << std::endl; $$ = new declaration_statement_node($1, $2, new expression_node()); }
                        |   data_type VARIABLE '[' INT ']' ASSIGNMENT '{' expression '}' {std::cout << line_number << ": Array1 Declaration statement with assignment" << std::endl; $$ = new declaration_statement_node($1, $2, $4, $8); }
                        |   data_type VARIABLE '[' INT ']' {std::cout << line_number << ": Array1 Declaration statement without assignment" << std::endl; $$ = new declaration_statement_node($1, $2, $4, new expression_node()); }
                        |   data_type VARIABLE '[' INT ']' '[' INT ']' ASSIGNMENT '{' expression '}' {std::cout << line_number << ": Array2 Declaration statement with assignment" << std::endl; $$ = new declaration_statement_node($1, $2, $4, $7, $11); }
                        |   data_type VARIABLE '[' INT ']' '[' INT ']' {std::cout << line_number << ": Array2 Declaration statement without assignment" << std::endl; $$ = new declaration_statement_node($1, $2, $4, $7, new expression_node()); }
                        ;

program:                    statement_list { $$ = new program_node($1, true, "\n"); root = $$; }
                        |   {std::cout << "Empty file" << std::endl; $$ = new program_node(new statement_list_node(new std::list<statement_node*>), true, "\n"); root = $$; }
                        ;

/* variable_list:              VARIABLE ',' variable_list {std::cout << line_number << ": Variable declaration without definition" << std::endl; free($1);}
                        |   VARIABLE {std::cout << line_number << ": Variable declaration without definition" << std::endl; free($1);}
                        |   VARIABLE ASSIGNMENT expression ',' variable_list {std::cout << line_number << ": Variable declaration with definition" << std::endl; free($1);}
                        |   VARIABLE ASSIGNMENT expression {std::cout << line_number << ": Variable declaration with definition" << std::endl; free($1);}
                        ; */
                        /* |   VARIABLE '[' expression ']' ',' variable_list {std::cout << line_number << ": Array declaration without definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' {std::cout << line_number << ": Array declaration without definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {std::cout << line_number << ": Array declaration with definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' ASSIGNMENT '{' expression '}' {std::cout << line_number << ": Array declaration with definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ',' variable_list {std::cout << line_number << ": Matrix declaration without definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' {std::cout << line_number << ": Matrix declaration without definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' ',' variable_list {std::cout << line_number << ": Matrix declaration with definition" << std::endl; free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' ASSIGNMENT '{' expression '}' {std::cout << line_number << ": Matrix declaration with definition" << std::endl; free($1);} */

tertiary_statement:         '(' expression ')' '?' statement ':' statement {std::cout << line_number << ": Tertiary Statement" << std::endl; $$ = new tertiary_statement_node($2, $5, $7); }
                        ;

control_statement:          IF '(' expression ')' '{' statement_list '}' else_statement {std::cout << line_number << ": If Statement" << std::endl; $8->insert_condition(" IF ", $3, $6); /*$8->print();*/ $$ = $8; }
                        ;

else_statement:                 ELSE '{' statement_list '}' {std::cout << line_number << ": Else Statement" << std::endl; $$ = new if_statement_node(new std::list<string>(1, "ELSE"), new std::list<expression_node*>(1, new value_node(true)), new std::list<statement_list_node*>(1, $3)); }
                            |   ELIF '(' expression ')' '{' statement_list '}' else_statement {std::cout << line_number << ": Else if Statement" << std::endl; $8->insert_condition("ELIF", $3, $6); $$ = $8; }
                            |   { $$ = new if_statement_node(new std::list<string>, new std::list<expression_node*>, new std::list<statement_list_node*>); }
                            ;


loops:                      WHILE '(' expression ')' '{' statement_list '}' {std::cout << line_number << ": WHILE Statement" << std::endl; $$ = new while_statement_node($3, $6); $$->setTerminatorChar("\n"); }
                        |   FOR assignment_statement ',' expression ',' assignment_statement '{' statement_list '}' {std::cout << line_number << ": FOR(v,v,v) Statement" << std::endl; $$ = new for_statement_node($2, $4, $6, $8); $$->setTerminatorChar("\n"); }
                        /* |   FOR VARIABLE ASSIGNMENT expression ',' expression '{' statement_list '}' {std::cout << line_number << ": FOR(v,v) Statement" << std::endl; $$ = new for_statement_node($2, $4, $6, $8); } */
                        ;

/* break_statement:            BREAK {std::cout << line_number << ": BREAK" << std::endl;} ; */

/* return_statement:           RETURN expression   {std::cout << line_number << ": RETURN expression" << std::endl;}
                        |   RETURN              {std::cout << line_number << ": RETURN without expression" << std::endl;}
                        ; */

/* operator:                   ASSIGNMENT | LOGOR | LOGAND | '|' | '^' | '&' | EQ | NE | GT | GE | LT | LE | LSHIFT | RSHIFT | '+' | '-' | '*' | '/' | '%'; */

expression:                 expression LOGOR        expression  { $$ = new operator_node($1, "||", $3); }
                        |   expression LOGAND       expression  { $$ = new operator_node($1, "&&", $3); }
                        |   expression '|'          expression  { $$ = new operator_node($1, "|", $3); }
                        |   expression '^'          expression  { $$ = new operator_node($1, "^", $3); }
                        |   expression '&'          expression  { $$ = new operator_node($1, "&", $3); }
                        |   expression EQ           expression  { $$ = new operator_node($1, "==", $3); }
                        |   expression NE           expression  { $$ = new operator_node($1, "!=", $3); }
                        |   expression GT           expression  { $$ = new operator_node($1, ">", $3); }
                        |   expression GE           expression  { $$ = new operator_node($1, ">=", $3); }
                        |   expression LT           expression  { $$ = new operator_node($1, "<", $3); }
                        |   expression LE           expression  { $$ = new operator_node($1, "<=", $3); }
                        |   expression LSHIFT       expression  { $$ = new operator_node($1, "<<", $3); }
                        |   expression RSHIFT       expression  { $$ = new operator_node($1, ">>", $3); }
                        |   expression '+'          expression  { $$ = new operator_node($1, "+", $3); }
                        |   expression '-'          expression  { $$ = new operator_node($1, "-", $3); }
                        |   expression '*'          expression  { $$ = new operator_node($1, "*", $3); }
                        |   expression '/'          expression  { $$ = new operator_node($1, "/", $3); }
                        |   expression '%'          expression  { $$ = new operator_node($1, "%", $3); }
                        |   '(' expression ')'                  { $$ = $2; }
                        |   '-' expression    %prec UMINUS      { $$ = new unary_minus_node($2); }
                        |   '!' expression                      { $$ = new unary_not_node($2); }
                        |   '~' expression                      { $$ = new unary_complement_node($2); }
                        |   VARIABLE { $$ = new variable_node($1); }
                        /* |   VARIABLE '[' expression ']' {free($1);}
                        |   VARIABLE '[' expression ']' '[' expression ']' {free($1);} */
                        |   value { $$ = $1; }
                        ;
                        /* |   STRING {free($1);}
                        |   function_call
                        |   assignment_statement
                        ; */


/* parameter_list:             data_type VARIABLE ','                  parameter_list  {free($2);}
                        |   data_type VARIABLE                                      {free($2);}
                        |   data_type VARIABLE '[' ']' ','          parameter_list  {free($2);}
                        |   data_type VARIABLE '[' ']'                              {free($2);}
                        |   data_type VARIABLE '[' ']' '[' ']' ','  parameter_list  {free($2);}
                        |   data_type VARIABLE '[' ']' '[' ']'                      {free($2);}
                        |
                        ; */

/* argument_list:              expression ',' argument_list
                        |   expression
                        |
                        ; */

/* function_name:              VARIABLE    {std::cout << line_number << ": User-defined function" << std::endl; free($1);}
                        |   data_type   {std::cout << line_number << ": Predefined function" << std::endl;}
                        ; */

/* function_declaration:       data_type                   function_name '(' parameter_list ')' '{' statement_list '}' {std::cout << line_number << ": Function declaration with parameters" << std::endl;}
                        |   data_type '[' ']'           function_name '(' parameter_list ')' '{' statement_list '}' {std::cout << line_number << ": Function declaration with parameters" << std::endl;}
                        |   data_type '[' ']' '[' ']'   function_name '(' parameter_list ')' '{' statement_list '}' {std::cout << line_number << ": Function declaration with parameters" << std::endl;}
                        |   TYPE_VOID                   function_name '(' parameter_list ')' '{' statement_list '}' {std::cout << line_number << ": Function declaration with parameters" << std::endl;}
                        ; */

/* function_call:              function_name '(' argument_list ')' {std::cout << line_number << ": Function call with arguments" << std::endl;}
                        ; */
%%

int parse_status = 0;

int main(int, char**) {
    // Initialize the data groups
    data_groups[""]             =   make_pair(0, 0);
    data_groups["TYPE_BOOL"]    =   make_pair(1, 0);
    data_groups["TYPE_CHAR"]    =   make_pair(1, 1);
    data_groups["TYPE_INT"]     =   make_pair(1, 2);
    data_groups["TYPE_FLOAT"]   =   make_pair(1, 3);
    // Open a file handle to a particular file:
    /* FILE *myfile = fopen("a.cplusp.file", "r"); */
    // Make sure it is valid:
    /* if (!myfile) {
        std::cout << "I can't open a.cplusp.file!" << std::endl;
    return -1;
    } */
    // Set Flex to read from it instead of defaulting to STDIN:
    /* yyin = myfile; */

    parse_status = 0;

    // Parse through the input:
    std::cout << line_number << "> ";
    yyparse();
    std::cout << std::endl;
    root->evaluate();
    root->generate();
    return parse_status;
}

void yyerror(const char *s) {
  std::cout << "OOPs, " << s << " on line number " << line_number << "!" << std::endl;
  parse_status = 251;
  // might as well halt now:
  /* return line_number; */
  /* exit(line_number); */
}
