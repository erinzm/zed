%option noyywrap yylineno nounput
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

#include "ast.h"
#include "parser.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

void yyerror(char *s, ...);
%}

%%

  /* keywords */
"fn"                      {return FN;}
"if"                      {return IF;}
"else"                    {return ELSE;}
"for"                     {return FOR;}
"return"                  {return RETURN;}
"const"                   {return CONST;}
"use"                     {return USE;}

  /* assignment, arithmetic, ordering, binops, and numtests */
    /*assignment*/
"="                     {return ASSIGN;}
    /*comparison*/
"=="                    {return EQ;}
"!="                    {return NEQ;}
"<"                     {return LT;}
">"                     {return GT;}
"<="                    {return LTE;}
"=>"                    {return GTE;}
  /*arithmetic*/
"+"                     {return ADDITION;}
"-"                     {return SUBTRACTION;}
"*"                     {return MULTIPLICATION;}
"/"                     {return DIVISION;}
"%"                     {return MODULO;}
  /*braces and parens*/
"("                     {return OPENPAREN;}
")"                     {return CLOSEPAREN;}
"{"                     {return OPENBRACE;}
"}"                     {return CLOSEBRACE;}
  /*misc*/
"."                     {return DOT;}
","                     {return COMMA;}
";"                     {return EOS;}
  /*binops*/
"!"                     {return BANG;}

  /* comments and whitespace and eol stuff */
\n                      {/* ignore eol */}
[ \t\n]                 {/* whitespace */}
#.*                     {/* comments */}

  /* numbers and strings and such */
[0-9]+	                {yylval.number = atof(yytext); return INT;}
[0-9]+\.[0-9]*          {yylval.number  = atof(yytext); return FLOAT;}
true                    {yylval.number  = 1; return BOOLEAN;}
false                   {yylval.number  = 0; return BOOLEAN;}
[a-z_][a-zA-Z0-9_]*       {yylval.string  = strdup(yytext); return IDENTIFIER;}
\"(\\.|[^\\"])*\"       {yylval.string = strdup(yytext); return STRING;}

   /* everything else */
.                       {yyerror("mystery character '%c' on line '%d'", yytext, yylineno);}

%%
