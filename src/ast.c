#include "ast.h"
#include "util.h"

void ast_node_free(ast_node *node) {
	if (!node) return; // don't try to free nodes that don't exist

	// Free pointers inside the struct
	switch (node->type) {
		case AST_TYPE_NUMBER: break; // Numbers are a *non-pointer* double,
		// they don't need to be *explicitly* freed if the struct is being freed after this
		case AST_TYPE_VARIABLE: {
			FREE_IF_EXISTS(node->variable.name);
			break;
		}
		case AST_TYPE_BINARY_OP: {
			FREE_IF_EXISTS(node->binary_op.lhs);
			FREE_IF_EXISTS(node->binary_op.rhs);
			break;
		}
		case AST_TYPE_FNCALL: {
			FREE_IF_EXISTS(node->fncall.name);
			for(unsigned int i=0; i<node->call.arg_count; i++) {
                ast_node_free(node->call.args[i]); // recursively free those sub-asts
            }
            free(node->fncall.args);
		}
	}

	free(node); // free the rest of the struct
}