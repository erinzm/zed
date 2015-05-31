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
  }
  s = sdscat(s, ")");
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
    default:
      return "";
  }
}
