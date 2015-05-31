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
		}
		case AST_TYPE_FNCALL: {
			FREE_IF_EXISTS(node->fncall.name);
			for(unsigned int i=0; i<node->fncall.argc; i++) {
	      ast_node_free(node->fncall.args[i]); // recursively free those sub-asts
      }
      free(node->fncall.args);
		}
		default:
			break;
	}

	free(node); // free the rest of the struct
}
