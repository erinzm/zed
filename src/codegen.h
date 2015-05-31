/*
* ast.h
* a code generator for COMPILER IN A WEEK
*
* GPLv3 Liam Marshall 2015
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "ast.h"
#include "util.h"
#include "sds/sds.h"

#ifndef _codegen_h
#define _codegen_h

char *codegen_number(ast_node *node);
char *codegen_string(ast_node *node);
char *codegen_fncall(ast_node *node);

char *codegen(ast_node *node);

#endif
