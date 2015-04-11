%option noyywrap nodefault yylineno
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

#include "parser.tab.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

void yyerror(char *s, ...);
%}

%%

-?(0[xX][0-9A-Fa-f]+(\.[0-9A-Fa-f]+)?([pP]-?[0-9A-Fa-f]+)?|(([0-9]*(\.[0-9]+)?)|[0-9]+)([eE]-?[0-9]+)?)	{yylval.fval = atof(yytext); return FLOAT;}
\"(\\.|[^\\"])*\" {yylval.sval = strdup(yytext); return STRING;}
'(\\.|[^\'])*\'	  {yylval.sval = strdup(yytext); return STRING;}

%%
