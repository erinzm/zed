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
  s = sdscat(s, ")");
  return s;
}

char *codegen(ast_node *root_node);
