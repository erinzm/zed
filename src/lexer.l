%option noyywrap yylineno
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

  /* assignment, arithmetic, ordering, binops, and numtests */
    /*assignment*/
"="                     {return ASSIGN;}
    /*comparison*/
"=="                    {return ISEQ;}
"!="                    {return ISNEQ;}
"<"                     {return ISLT;}
">"                     {return ISGT;}
"<="                    {return ISLTE;}
"=>"                    {return ISGTE;}
  /*arithmetic*/
"+"                     {return ADDITION;}
"-"                     {return SUBTRACTION;}
"*"                     {return MULTIPLICATION;}
"/"                     {return DIVISION;}
  /*braces and parens*/
"("                     {return OPENPAREN;}
")"                     {return CLOSEPAREN;}
"{"                     {return OPENBRACE;}
"}"                     {return CLOSEBRACE;}
  /*misc*/
"."                     {return DOT;}
","                     {return COMMA;}
  /*binops*/
"!"                     {return BANG;}

  /* comments and whitespace and eol stuff */
\n                      {/* ignore eol */}
[ \t\n]                 {/* whitespace */}
\/\/.*                  {/* comments */}

  /* numbers and strings and such */
[0-9]+	                {yylval.integer = atoi(yytext); return INT;}
[0-9]+\.[0-9]*          {yylval.floating  = atof(yytext); return FLOAT;}
true                    {yylval.integer  = 1; return BOOLEAN;}
false                   {yylval.integer  = 0; return BOOLEAN;}
[a-z][a-zA-Z0-9]*       {yylval.string  = strdup(yytext); return IDENTIFIER;}
\"(\\.|[^\\"])*\"       {yylval.string = strdup(yytext); return STRING;}

   /* everything else */
.                       {yyerror("mystery character '%c' on line '%d'", yytext, yylineno);}

%%
