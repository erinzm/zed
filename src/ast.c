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

ast_node *ast_variable_create(char *name, char *type) {
	ast_node *node = STRUCT_INSTANCE(ast_node);
	node->type = AST_TYPE_VARIABLE;
	node->variable.name = strdup(name);
	if (type != NULL) {
		node->variable.type = strdup(type);
	} else {
		node->variable.type = NULL;
	}
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
	switch(node->type) {
		case AST_TYPE_NUMBER:
			printf("<number value=%f />\n", node->number.value);
			break;
		case AST_TYPE_STRING:
			printf("<string value=\"%s\" />\n", node->string.value);
			break;
		case AST_TYPE_VARIABLE:
			printf("<variable name=\"%s\" type=\"%s\" />", node->variable.name, node->variable.type);
			break;
		case AST_TYPE_USE:
			printf("<use thing=\"%s\" isC=\"%s\" />\n", node->use.value, node->use.isC ? "true" : "false");
			break;
		case AST_TYPE_BINARY_OP:
			printf("<binop type=%i>\n", node->binary_op.op);
			dump_ast_node(node->binary_op.lhs);
			dump_ast_node(node->binary_op.rhs);
			printf("</binop>");
			break;
		case AST_TYPE_FNCALL:
			printf("<fncall name=\"%s\">\n", node->fncall.name);
			for (unsigned int i = 0; i < node->fncall.argc; i++) {
				dump_ast_node(node->fncall.args[i]);
			}
			printf("</fncall>\n");
			break;
		case AST_TYPE_STATEMENTS:
		case AST_TYPE_BLOCK:
			if (node->type == AST_TYPE_STATEMENTS) {
				printf("<statements>\n");
			} else if (node->type == AST_TYPE_BLOCK) {
				printf("<block>\n");
			}
			for (unsigned int i = 0; i < node->statements.count; i++) {
				dump_ast_node(node->statements.nodes[i]);
			}
			if (node->type == AST_TYPE_STATEMENTS) {
				printf("</statements>\n");
			} else if (node->type == AST_TYPE_BLOCK) {
				printf("</block>\n");
			}
			break;
		case AST_TYPE_FUNCTION:
			printf("<function name=\"%s\" type=\"%s\">\n", node->function.name, node->function.type);
			printf("<arguments>\n");
			for (unsigned int i = 0; i < node->function.argc; i++) {
				dump_ast_node(node->function.arguments[i]);
			}
			printf("</arguments>\n");
			dump_ast_node(node->function.innerBlock);
			printf("</function>\n");
			break;
		default:
			break;
	}
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
