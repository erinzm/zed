%option noyywrap
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
%}

%%

-?(0[xX][0-9A-Fa-f]+(\.[0-9A-Fa-f]+)?([pP]-?[0-9A-Fa-f]+)?|(([0-9]*(\.[0-9]+)?)|[0-9]+)([eE]-?[0-9]+)?)	{printf("A number %f\n", atof(yytext));}
\"(\\.|[^\\"])*\" {printf("A string %s\n", yytext);}
'(\\.|[^\'])*\'	  {printf("A string %s\n", yytext);}

%%

int main(void) {
  yylex();
  return 0;
}
