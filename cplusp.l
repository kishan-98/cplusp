%{
  #include <cstdio>
  #include "cplusp.tab.h"  // to get the token types from Bison
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
\'[a-zA-Z0-9]\'         {   return CHAR;}
true                    {   yylval.bval = true; return BOOL;}
True                    {   yylval.bval = true; return BOOL;}
false                   {   yylval.bval = false; return BOOL;}
False                   {   yylval.bval = false; return BOOL;}
int                     {   yylval.dtype = strdup(yytext); return TYPE_INT;}
float                   {   yylval.dtype = strdup(yytext); return TYPE_FLOAT;}
char                    {   yylval.dtype = strdup(yytext); return TYPE_CHAR;}
bool                    {   yylval.dtype = strdup(yytext); return TYPE_BOOL;}
File                    {   yylval.dtype = strdup(yytext); return TYPE_FILE;}
void                    {   return TYPE_VOID;}
=                       {   return ASSIGNMENT; }
[!~]                    {   return UNARY;}
[+\-*/%]                {   return ARITHMATIC;}
[&|^<>]                 {   return BINARY;}
"<<"                    {   return BINARY;}
">>"                    {   return BINARY;}
"=="                    {   return BINARY;}
"!="                    {   return BINARY;}
"<="                    {   return BINARY;}
">="                    {   return BINARY;}
"&&"                    {   return LOGICAL;}
"||"                    {   return LOGICAL;}
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
