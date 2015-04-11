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

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "parser.tab.h"
%}

%%

-?(0[xX][0-9A-Fa-f]+(\.[0-9A-Fa-f]+)?([pP]-?[0-9A-Fa-f]+)?|(([0-9]*(\.[0-9]+)?)|[0-9]+)([eE]-?[0-9]+)?)	{yyval.fval = atof(yytext); return FLOAT;}
\"(\\.|[^\\"])*\" {yyval.sval = strdup(yytext); return STRING;}
'(\\.|[^\'])*\'	  {yyval.sval = strdup(yytext); return STRING;}

%%
