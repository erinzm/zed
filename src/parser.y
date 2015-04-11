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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void yyerror(const char *s, ...);
%}

%union {
  int integer;
  float floating;
  char* string;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%%
prgm:
    INT {printf("found an int\n");}
    | FLOAT {printf("found an float\n");}
    | STRING {printf("found an string\n");}
    ;
%%

void yyerror(const char *s, ...) {
  extern yylineno;

  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
