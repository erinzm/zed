#include "codegen.h"

char *codegen_number(ast_node *node) {
  sds s;
  s = sdsnew("");
  s = sdscatprintf(s, "%f", node->number.value);
  return s;
}

char *codegen_string(ast_node *node) {
  sds s;
  s = sdsnew("\"");
  s = sdscat(s, node->string.value);
  s = sdscat(s, "\"");
  return s;
}

char *codegen_fncall(ast_node *node) {
  sds s;
  s = sdsnew(node->fncall.name);
  s = sdscat(s, "(");
  for (unsigned int i = 0; i < node->fncall.argc; i++) {
    s = sdscat(s, codegen(node->fncall.args[i])); // yay recusive descent!
    if ((i + 1) < node->fncall.argc) s = sdscat(s, ",");
  }
  s = sdscat(s, ")");
  return s;
}

char *codegen_use(ast_node *node) {
  if (node->use.isC) {
    sds header = sdsnew(node->use.value);
    header = sdscat(header, ".h");
    sds s = sdsnew("#include <");
    s = sdscat(s, header);
    s = sdscat(s, ">");
    return s;
  } else {
    return ""; // not implemented yet
  }
}

char *codegen_binary_op(ast_node *node) {
  sds s = sdsnew(codegen(node->binary_op.lhs));
  s = sdscat(s, codegen_getBinOp(node->binary_op.op));
  s = sdscat(s, codegen(node->binary_op.rhs));
  return s;
}

char *codegen_variable(ast_node *node) {
  sds s = sdsnew("");
  if (node->variable.type != NULL) {
    s = sdscat(s, node->variable.type);
    s = sdscat(s, " ");
  }
  s = sdscat(s, node->variable.name);
  return s; // handling code may go before here, so declaring a new sds isn't as worthless as it seems.
}

char *codegen_assignment(ast_node *node) {
  sds assignment = sdsnew("");
  sds variable = sdsnew(codegen(node->assignment.lhs));
  sds value = sdsnew(codegen(node->assignment.rhs));

  assignment = sdscat(assignment, variable);
  assignment = sdscat(assignment, "=");
  assignment = sdscat(assignment, value);

  return assignment;
}

char *codegen_statements(ast_node *node) {
  sds s = sdsnew("");
  if (node->type == AST_TYPE_BLOCK) s = sdscat(s, "{\n");
  for (unsigned int i = 0; i < node->statements.count; i++) {
    s = sdscat(s, codegen(node->statements.nodes[i]));
    switch((node->statements.nodes[i])->type) {
      case AST_TYPE_USE:
        break;
      case AST_TYPE_BLOCK:
        break;
      case AST_TYPE_FUNCTION:
        break;
      default:
        s = sdscat(s, ";");
        break;
    }
    s = sdscat(s, "\n");
  }
  if (node->type == AST_TYPE_BLOCK) s = sdscat(s, "}");
  return s;
}

char *codegen_function(ast_node *node) {
  sds s = sdsnew("");
  s = sdscat(s, node->function.type);
  s = sdscat(s, " ");
  s = sdscat(s, node->function.name);
  s = sdscat(s, "(");
  for (unsigned int i = 0; i < node->function.argc; i++) {
    s = sdscat(s, codegen(node->function.arguments[i]));
    if ((i + 1) < node->function.argc) s = sdscat(s, ",");
  }
  s = sdscat(s, ")");
  s = sdscat(s, codegen(node->function.innerBlock));
  return s;
}

char *codegen_getBinOp(ast_type_binop op) {
  switch(op) {
    case AST_BINOP_ADD:
      return "+";
    case AST_BINOP_SUB:
      return "-";
    case AST_BINOP_MUL:
      return "*";
    case AST_BINOP_DIV:
      return "/";
    case AST_BINOP_MOD:
      return "%";
    default:
      return "";
  }
}

char *codegen(ast_node *node) {
  switch (node->type) {
    case AST_TYPE_NUMBER:
      return codegen_number(node);
    case AST_TYPE_STRING:
      return codegen_string(node);
    case AST_TYPE_FNCALL:
      return codegen_fncall(node);
    case AST_TYPE_USE:
      return codegen_use(node);
    case AST_TYPE_BINARY_OP:
      return codegen_binary_op(node);
    case AST_TYPE_VARIABLE:
      return codegen_variable(node);
    case AST_TYPE_ASSIGNMENT:
      return codegen_assignment(node);
    case AST_TYPE_BLOCK:
    case AST_TYPE_STATEMENTS:
      return codegen_statements(node);
    case AST_TYPE_FUNCTION:
      return codegen_function(node);
    default:
      return "";
  }
}
