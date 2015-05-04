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

#include "ast.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

ast_node *root;

void yyerror(const char *s, ...);
int yylex();

%}

%code requires {
  #include "ast.h"
}


%union {
  double number;
  char* string;
  ast_node *node;
  struct {
    ast_node **args;
    int count;
  } fncall_args;
  int token;
}

/* keywords */
%token FN IF ELSE FOR RETURN CONST
/* operators */
%token ASSIGN DOT COMMA BANG
%token <token> ISEQ ISNEQ ISLT ISGT ISLTE ISGTE ADDITION SUBTRACTION MULTIPLICATION DIVISION
/* seperators */
%token OPENPAREN CLOSEPAREN OPENBRACE CLOSEBRACE

%token <number> INT
%token <number> FLOAT
%token <string> STRING
%token <string> IDENTIFIER
%token <integer> BOOLEAN

%right ASSIGN
%left NOT
%left ISEQ ISLT ISGT ISLTE ISGTE ISNEQ
%left ADDITION SUBTRACTION
%left MULTIPLICATION DIVISION MODULO

%type <node> expression identifier number variable_declaration function_declaration statement
%type <token> binop

%start program

%%

program : statements {}
        ;

statements : statement
           | statements statement
           ;

statement : variable_declaration
          | function_declaration
          | expression
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

identifier : IDENTIFIER { $$ = ast_variable_create($1); free($1); dump_ast_node($$); }
           ;

number : INT { $$ = ast_number_create($1); }
       | FLOAT { $$ = ast_number_create($1); }
       ;

binop : ADDITION { $$ = AST_BINOP_ADD; }
      | SUBTRACTION { $$ = AST_BINOP_SUB; }
      | MULTIPLICATION { $$ = AST_BINOP_MUL; }
      | DIVISION { $$ = AST_BINOP_DIV; }
      | MODULO { $$ = AST_BINOP_MOD; }
      ;

expression : OPENPAREN expression CLOSEPAREN { $$ = $2; dump_ast_node($$); }
           | number
           | expression binop expression { $$ = ast_binary_op_create($2, $1, $3);}
           | identifier
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
