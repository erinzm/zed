/*
 * ast.h
 * an ast for Zed
 *
 * GPLv3 Liam Marshall 2015
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "util.h"


#ifndef _ast_h
#define _ast_h

typedef enum ast_node_type {
	AST_TYPE_NUMBER,
	AST_TYPE_STRING,
	AST_TYPE_VARIABLE,
	AST_TYPE_BINARY_OP,
	AST_TYPE_USE,
	AST_TYPE_FNCALL,
	AST_TYPE_ASSIGNMENT,
	AST_TYPE_STATEMENTS,
	AST_TYPE_BLOCK
} ast_node_type;

typedef enum ast_type_binop {
	AST_BINOP_ADD,
	AST_BINOP_SUB,
	AST_BINOP_MUL,
	AST_BINOP_DIV,
	AST_BINOP_MOD
} ast_type_binop;

typedef struct ast_number {
	double value;
} ast_number;

typedef struct ast_string {
	char *value;
} ast_string;

typedef struct ast_variable {
	char *name;
} ast_variable;

typedef struct ast_binary_op {
	ast_type_binop op;
	struct ast_node *lhs;
	struct ast_node *rhs;
} ast_binary_op;

typedef struct ast_use {
	char *value;
	bool isC;
} ast_use;

typedef struct ast_assignment {
	struct ast_node *lhs;
	struct ast_node *rhs;
} ast_assignment;

typedef struct ast_fncall {
	char *name;
	struct ast_node **args;
	unsigned int argc;
} ast_fncall;

typedef struct ast_statements {
	struct ast_node **nodes;
	unsigned int count;
} ast_statements;

typedef struct ast_node {
	ast_node_type type;
	union {
		ast_number number;
		ast_string string;
		ast_variable variable;
		ast_binary_op binary_op;
		ast_use use;
		ast_assignment assignment;
		ast_fncall fncall;
		ast_statements statements;
	};
} ast_node;

// =================< AST functions >================= //

ast_node *ast_number_create(double value);

ast_node *ast_string_create(char *value);

ast_node *ast_variable_create(char *name);

ast_node *ast_binary_op_create(ast_type_binop op,
    ast_node *lhs, ast_node *rhs);

ast_node *ast_fncall_create(char *name, ast_node **args,
    int argc);

ast_node *ast_statements_create(ast_node **nodes, int count, bool isBlock);

ast_node *ast_use_create(char *value, bool isC);

ast_node *ast_assignment_create(ast_node *lhs, ast_node *rhs);

void dump_ast_node(ast_node *node);

void ast_node_free(ast_node *node);

#endif
