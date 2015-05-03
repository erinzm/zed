/*
 * ast.h
 * an ast for COMPILER IN A WEEK
 * 
 * GPLv3 Liam Marshall 2015
 */

#ifndef _ast_h
#define _ast_h

typedef enum ast_node_type {
	AST_TYPE_NUMBER,
	AST_TYPE_VARIABLE,
	AST_TYPE_BINARY_OP,
	AST_TYPE_FNCALL
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

typedef struct ast_variable {
	char *name;
} ast_variable;

typedef struct ast_binary_op {
	ast_type_binop op;
	struct ast_node *lhs;
	struct ast_node *rhs;
} ast_binary_op;

typedef struct ast_fncall {
	char *name;
	struct ast_node **args;
	unsigned int argc;
} ast_fncall;

typedef struct ast_node {
	ast_node_type type;
	union {
		ast_number number;
		ast_variable variable;
		ast_binary_op binary_op;
		ast_fncall fncall;
	};
} ast_node;

// =================< AST functions >================= //

ast_node *ast_number_create(double value);

ast_node *ast_variable_create(char *name);

ast_node *ast_binary_op_create(ast_type_binop op,
    ast_node *lhs, ast_node *rhs);

ast_node *ast_fncall_create(char *name, ast_node **args,
    int argc);

void dump_ast_node(ast_node *node);

void ast_node_free(ast_node *node);

#endif
