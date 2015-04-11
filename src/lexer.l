%option noyywrap yylineno
%{
/*************************************
 * Flex lexer for Compiler In a Week
 *
 * lexer.l
 *
 * Licensed under the GPL
 * See LICENSE for more details
 *
 * Author: Liam Marshall (2015)
 *************************************/

#include "parser.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

void yyerror(char *s, ...);
%}

%%

  /* keywords */
fn                      {return FN;}
if                      {return IF;}
for                     {return FOR;}

  /* comments and whitespace and eol stuff */
\n                      {/* ignore eol */}
[ \t\n]                 {/* whitespace */}
\/\/.*                  {/* comments */}

  /* numbers and strings and such */
[0-9]+	                {yylval.integer = atoi(yytext); return INT;}
[0-9]+\.[0-9]*          {yyval.floating  = atof(yytext); return FLOAT;}
[a-zA-Z_][a-z-A-Z0-9_]* {yyval.string  = strdup(yytext); return IDENTIFIER;}
true                    {yyval.integer  = 1; return BOOLEAN;}
false                   {yyval.integer  = 0; return BOOLEAN;}
\"(\\.|[^\\"])*\"       {yyval.string = strdup(yytext); return STRING;}

   /* everything else */
.                       {yyerror("mystery character '%c' on line '%d'", yytext, yylineno);}

%%
