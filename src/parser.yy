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

#include <iostream>
#include <cctype>
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <vector>
#include <stack>

extern "C" void yyerror(const char *s, ...);
extern "C" int yylex();

%}

%union {
  int integer;
  float floating;
  char* string;
}

/* keywords */
%token FN IF ELSE FOR RETURN CONST
/* operators */
%token ASSIGN ISEQ ISNEQ ISLT ISGT ISLTE ISGTE ADDITION SUBTRACTION MULTIPLICATION DIVISION DOT COMMA BANG
/* seperators */
%token OPENPAREN CLOSEPAREN OPENBRACE CLOSEBRACE

%token <integer> INT
%token <floating> FLOAT
%token <string> STRING
%token <string> IDENTIFIER
%token <integer> BOOLEAN

%right ASSIGN
%left NOT
%left ISEQ ISLT ISGT ISLTE ISGTE ISNEQ
%left ADDITION SUBTRACTION
%left MULTIPLICATION DIVISION MODULO

%start program

%%

program : statements {}
        ;

statements : statement
           | statements statement
           ;

statement : variable_declaration
          | function_declaration
          | expression {}
          ;

block : OPENBRACE statements CLOSEBRACE
      | OPENBRACE CLOSEBRACE
      ;

variable_declaration : identifier identifier {}
                     | identifier identifier ASSIGN expression {}
                     ;

function_declaration : identifier identifier OPENPAREN function_arguments CLOSEPAREN block {}
                     ;

function_arguments : /* no arguments */ {}
                   | variable_declaration {}
                   | function_arguments COMMA variable_declaration {}
                   ;

identifier : IDENTIFIER {}
           ;

number : INT {}
       | FLOAT {}
       ;

binop : ADDITION
      | SUBTRACTION
      | MULTIPLICATION
      | DIVISION
      | MODULO
      ;

expression : OPENPAREN expression CLOSEPAREN {}
           | number {}
           | expression binop expression {}
           ; 

%%

void yyerror(const char *s, ...) {
  extern int yylineno;

  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
