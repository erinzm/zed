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
    default:
      return "";
  }
}
