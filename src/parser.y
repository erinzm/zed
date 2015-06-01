%{
/*************************************
 * Bison parser for Zed
 *
 * parser.y
 *
 * Licensed under the GPL
 * See LICENSE for more details
 *
 * Author: Liam Marshall (2015)
 *************************************/

#include "ast.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
%token FN IF ELSE FOR RETURN CONST USE
/* operators */
%token ASSIGN DOT COMMA BANG EOS
%token <token> EQ NEQ LT GT LTE GTE ADDITION SUBTRACTION MULTIPLICATION DIVISION
/* seperators */
%token OPENPAREN CLOSEPAREN OPENBRACE CLOSEBRACE

%token <number> INT
%token <number> FLOAT
%token <string> STRING
%token <string> IDENTIFIER
%token <integer> BOOLEAN

%right ASSIGN
%left NOT
%left EQ LT GT LTE GTE NEQ
%left ADDITION SUBTRACTION
%left MULTIPLICATION DIVISION MODULO

%type <node> expression number identifier variable_declaration variable_assignment function_declaration function_call statement string use
%type <fncall_args> function_call_arguments
%type <token> binop

%start program

%%

program : statements {}

statements : statement { printf("%s", codegen($1)); }
           | statements statement { printf("%s", codegen($2)); }
           ;

statement : variable_declaration EOS
          | function_declaration EOS
          | function_call        EOS
          | variable_assignment  EOS
          | expression           EOS
          | use                  EOS
          ;

block : OPENBRACE statements CLOSEBRACE
      | OPENBRACE CLOSEBRACE
      ;

variable_declaration : IDENTIFIER IDENTIFIER {}
                     | IDENTIFIER IDENTIFIER ASSIGN expression {}
                     ;

variable_assignment : identifier ASSIGN expression { $$ = ast_assignment_create($1, $3); }

function_declaration : FN IDENTIFIER IDENTIFIER OPENPAREN function_decl_arguments CLOSEPAREN block {}
                     ;

function_call : IDENTIFIER OPENPAREN function_call_arguments CLOSEPAREN { $$ = ast_fncall_create($1, $3.args, $3.count); free($1); free($3.args);}
              ;

function_decl_arguments : /* no arguments */ {}
                   | variable_declaration {}
                   | function_decl_arguments COMMA variable_declaration {}
                   ;

function_call_arguments : /* no arguments */ { $$.count = 0; $$.args = NULL; }
                        | expression { $$.count = 1;
                          $$.args = malloc(sizeof(ast_node*));
                          $$.args[0] = $1;}
                        | function_call_arguments COMMA expression {$1.count++;
                          $1.args = realloc($1.args, sizeof(ast_node*) * $1.count);
                          $1.args[$1.count-1] = $3; $$ = $1;}
                        ;

use : USE IDENTIFIER { $$ = ast_use_create($2, false); free($2); }
    | USE LT IDENTIFIER GT { $$ = ast_use_create($3, true); free($3); }
    ;

identifier : IDENTIFIER { $$ = ast_variable_create($1); free($1); }
           ;

number : INT { $$ = ast_number_create($1); }
       | FLOAT { $$ = ast_number_create($1); }
       ;

string : STRING { $$ = ast_string_create($1); free($1); }
       ;

binop : ADDITION { $$ = AST_BINOP_ADD; }
      | SUBTRACTION { $$ = AST_BINOP_SUB; }
      | MULTIPLICATION { $$ = AST_BINOP_MUL; }
      | DIVISION { $$ = AST_BINOP_DIV; }
      | MODULO { $$ = AST_BINOP_MOD; }
      ;

expression : OPENPAREN expression CLOSEPAREN { $$ = $2; }
           | number
           | string
           | expression binop expression { $$ = ast_binary_op_create($2, $1, $3);}
           | identifier
           ;

%%

void yyerror(const char *s, ...) {
  extern int yylineno;

  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wformat-nonliteral"
  vfprintf(stderr, s, ap);
  #pragma GCC diagnostic pop
  fprintf(stderr, "\n");
  exit(1);
}
