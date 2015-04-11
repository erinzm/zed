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

   /* numbers and strings and such */
-?[0-9]+	        {yylval.fval = atoi(yytext); return INT;}
\"(\\.|[^\\"])*\" {yylval.sval = strdup(yytext); return STRING;}
'(\\.|[^\'])*\'	  {yylval.sval = strdup(yytext); return STRING;}

   /* everything else */
[ \t\n]           /* whitespace */
.                 {yyerror("mystery character '%c' on line '%d'", yytext, yylineno);}

%%
