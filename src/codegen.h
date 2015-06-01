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
char *codegen_use(ast_node *node);
char *codegen_binary_op(ast_node *node);
char *codegen_variable(ast_node *node);
char *codegen_assignment(ast_node *node);
char *codegen_statements(ast_node *node);

char *codegen_getBinOp(ast_type_binop op);

char *codegen(ast_node *node);

#endif
