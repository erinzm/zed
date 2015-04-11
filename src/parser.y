%{
/*************************************
 * Bison parser for Compiler In a Week
 *
 * parser.y
 *
 * Licensed under the GPL
 * See LICENSE for more details
 *
 * Author: Liam Marshall (2015)
 *************************************/

#include <stdio.h>
extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);
%}

%token INT
%token FLOAT
%token STRING

%%

%%

void yyerror(const char *s) {
  printf("Parse error: %s\n", s);
  exit(-1);
}
