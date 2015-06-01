#include "ast.h"


ast_node *ast_number_create(double value) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_NUMBER;
	node->number.value = value;
	return node;
}

ast_node *ast_string_create(char *value) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_STRING;
	node->string.value = strdup(value);
	return node;
}

ast_node *ast_variable_create(char *name) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_VARIABLE;
	node->variable.name = strdup(name);
	return node;
}

ast_node *ast_fncall_create(char *name, ast_node **args, int argc) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_FNCALL;
	node->fncall.name = strdup(name);
	// Shallow copy arguments
	node->fncall.args = malloc(sizeof(ast_node*) * argc);
	memcpy(node->fncall.args, args, sizeof(ast_node*) * argc);
	node->fncall.argc = argc;
	return node;
}

ast_node *ast_binary_op_create(ast_type_binop op, ast_node *lhs, ast_node *rhs) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_BINARY_OP;
	node->binary_op.op = op;
	node->binary_op.lhs = lhs;
	node->binary_op.rhs = rhs;
	return node;
}

ast_node *ast_use_create(char *value, bool isC) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_USE;
	node->use.value = strdup(value);
	node->use.isC = isC;
	return node;
}

ast_node *ast_assignment_create(ast_node *lhs, ast_node *rhs) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_ASSIGNMENT;
	node->assignment.lhs = lhs;
	node->assignment.rhs = rhs;
	return node;
}

ast_node *ast_statements_create(ast_node **nodes, int count, bool isBlock) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	if (isBlock) {
		node->type = AST_TYPE_BLOCK;
	} else {
		node->type = AST_TYPE_STATEMENTS;
	}
	node->statements.nodes = nodes;
	node->statements.count = count;
	return node;
}

ast_node *ast_function_create(char *name, char *type, ast_node *innerBlock, ast_node **arguments, unsigned int argc) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_FUNCTION;
	node->function.name = strdup(name);
	node->function.type = strdup(type);
	node->function.innerBlock = innerBlock;
	node->function.arguments = arguments;
	node->function.argc = argc;
	return node;
}

void dump_ast_node(ast_node *node) {
	printf("=== node dump ===\n");
	switch(node->type) {
		case AST_TYPE_NUMBER:
			printf("node->type == AST_TYPE_NUMBER\n");
			INSPECT(node->number.value, "%f");
			break;
		case AST_TYPE_STRING:
			printf("node->type == AST_TYPE_STRING\n");
			INSPECT(node->string.value, "%s");
			break;
		case AST_TYPE_VARIABLE:
			printf("node->type == AST_TYPE_VARIABLE\n");
			INSPECT(node->variable.name, "%s");
			break;
		case AST_TYPE_USE:
			printf("node->type == AST_TYPE_USE\n");
			INSPECT(node->use.value, "%s");
			INSPECT(node->use.isC, "%i");
			break;
		case AST_TYPE_BINARY_OP:
			printf("node->type == AST_TYPE_BINARY_OP\n");
			INSPECT(node->binary_op.op, "%i");
			dump_ast_node(node->binary_op.lhs);
			dump_ast_node(node->binary_op.rhs);
			break;
		case AST_TYPE_FNCALL:
			printf("node->type == AST_TYPE_FNCALL\n");
			INSPECT(node->fncall.name, "%s");
			for (unsigned int i = 0; i < node->fncall.argc; i++) {
				dump_ast_node(node->fncall.args[i]);
			}
			break;
		case AST_TYPE_STATEMENTS:
		case AST_TYPE_BLOCK:
			if (node->type == AST_TYPE_STATEMENTS) {
				printf("node->type == AST_TYPE_STATEMENTS\n");
			} else if (node->type == AST_TYPE_BLOCK) {
				printf("node->type == AST_TYPE_BLOCK\n");
			}
			for (unsigned int i = 0; i < node->statements.count; i++) {
				dump_ast_node(node->statements.nodes[i]);
			}
			break;
		case AST_TYPE_FUNCTION:
			printf("node->type == AST_TYPE_FUNCTION\n");
			INSPECT(node->function.name, "%s");
			INSPECT(node->function.type, "%s");
			dump_ast_node(node->function.innerBlock);
			for (unsigned int i = 0; i < node->function.argc; i++) {
				dump_ast_node(node->function.arguments[i]);
			}
			break;
		default:
			break;
	}
	printf("=== end node dump ===\n");
}

void ast_node_free(ast_node *node) {
	if (!node) return; // don't try to free nodes that don't exist

	// Free pointers inside the struct
	switch (node->type) {
		case AST_TYPE_NUMBER: break; // Numbers are a *non-pointer* double,
		// they don't need to be *explicitly* freed if the struct is being freed after this
		case AST_TYPE_STRING: {
			FREE_IF_EXISTS(node->string.value);
			break;
		}
		case AST_TYPE_VARIABLE: {
			FREE_IF_EXISTS(node->variable.name);
			break;
		}
		case AST_TYPE_BINARY_OP: {
			FREE_IF_EXISTS(node->binary_op.lhs);
			FREE_IF_EXISTS(node->binary_op.rhs);
			break;
		}
		case AST_TYPE_USE: {
			FREE_IF_EXISTS(node->use.value);
			break;
		}
		case AST_TYPE_FNCALL: {
			FREE_IF_EXISTS(node->fncall.name);
			for(unsigned int i=0; i<node->fncall.argc; i++) {
	      ast_node_free(node->fncall.args[i]); // recursively free those sub-asts
      }
      FREE_IF_EXISTS(node->fncall.args);
			break;
		}
		case AST_TYPE_ASSIGNMENT: {
			FREE_IF_EXISTS(node->assignment.lhs);
			FREE_IF_EXISTS(node->assignment.rhs);
			break;
		}
		case AST_TYPE_BLOCK:
		case AST_TYPE_STATEMENTS: {
			for (unsigned int i = 0; i < node->statements.count; i++) {
				ast_node_free(node->statements.nodes[i]);
			}
			break;
		}
		case AST_TYPE_FUNCTION: {
			FREE_IF_EXISTS(node->function.name);
			FREE_IF_EXISTS(node->function.type);
			ast_node_free(node->function.innerBlock);
			break;
		}
	}

	free(node); // free the rest of the struct
}
