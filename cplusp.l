%{
  #include "cplusp.h"       // for AST
  #include "cplusp.tab.h"   // to get the token types from Bison
  /* #include <iostream>
  using namespace std;
  extern int yylex(); */
  int line_number = 1;
%}

%%
[ \t]         ;
[\n]                    {   line_number++;  }
[0-9]+                  {   yylval.ival = atoi(yytext); return INT;}
[0-9]*\.[0-9]+          {   yylval.fval = atof(yytext); return FLOAT;}
\'[a-zA-Z0-9]\'         {   yylval.cval = yytext[1]; return CHAR;}
\'\0\'                  {   yylval.cval = '\0'; return CHAR;}
true                    {   yylval.bval = true; return BOOL;}
True                    {   yylval.bval = true; return BOOL;}
false                   {   yylval.bval = false; return BOOL;}
False                   {   yylval.bval = false; return BOOL;}
int                     {   yylval.datatype = strdup("TYPE_INT"); return TYPE_INT;}
float                   {   yylval.datatype = strdup("TYPE_FLOAT"); return TYPE_FLOAT;}
char                    {   yylval.datatype = strdup("TYPE_CHAR"); return TYPE_CHAR;}
bool                    {   yylval.datatype = strdup("TYPE_BOOL"); return TYPE_BOOL;}
File                    {   yylval.datatype = strdup("TYPE_FILE"); return TYPE_FILE;}
void                    {   return TYPE_VOID;}
=                       {   return ASSIGNMENT; }
[!~+\-*/%&|^]           {   return yytext[0];}
"<<"                    {   return LSHIFT;}
">>"                    {   return RSHIFT;}
"<"                     {   return LT;}
">"                     {   return GT;}
"=="                    {   return EQ;}
"!="                    {   return NE;}
"<="                    {   return LE;}
">="                    {   return GE;}
"&&"                    {   return LOGAND;}
"||"                    {   return LOGOR;}
;                       {   return TERMINATOR; }
if                      {   return IF; }
elif                    {   return ELIF; }
else                    {   return ELSE; }
for                     {   return FOR; }
while                   {   return WHILE; }
return                  {   return RETURN; }
break                   {   return BREAK; }
[\(\)\{\}?:,\[\]]       {   return yytext[0]; }

[a-zA-Z]+[a-zA-Z0-9_]*  {   yylval.vval = strdup(yytext); return VARIABLE;}
\"([a-zA-Z0-9]|[ ])+\"        {
  // We have to strdup yytext because Flex will change it for the next token.
  // Note that this memory must be freed somewhere, so that's why we call
  // free() above in the Bison section.  (Aside: we use free() instead of
  // delete because strdup is a C function that uses malloc, not a C++
  // function that uses new.)
                        yylval.sval = strdup(yytext); return STRING;}
.                   {   yylval.inval = strdup(yytext); return INVALID; }
%%
