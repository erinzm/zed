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
    sds fncall_arg = codegen(node->fncall.args[i]); // yay recusive descent!
    s = sdscat(s, fncall_arg);
    sdsfree(fncall_arg);
    if ((i + 1) < node->fncall.argc) s = sdscat(s, ",");
  }
  s = sdscat(s, ")");
  return s;
}

char *codegen_use(ast_node *node) {
  if (node->use.isC) {
    sds use = sdsnew("#include <");
    use = sdscat(use, node->use.value);
    use = sdscat(use, ".h");
    use = sdscat(use, ">");

    return use;
  } else {
    return sdsnew(""); // not implemented yet
  }
}

char *codegen_binary_op(ast_node *node) {
  sds binop = sdsnew("");

  sds lhs = codegen(node->binary_op.lhs);
  sds op = sdsnew(codegen_getBinOp(node->binary_op.op));
  sds rhs = codegen(node->binary_op.rhs);

  binop = sdscat(binop, lhs);
  binop = sdscat(binop, op);
  binop = sdscat(binop, rhs);

  sdsfree(op);
  sdsfree(lhs);
  sdsfree(rhs);

  return binop;
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
  sds statements = sdsnew("");
  if (node->type == AST_TYPE_BLOCK) statements = sdscat(statements, "{\n");
  for (unsigned int i = 0; i < node->statements.count; i++) {
    sds statement = codegen(node->statements.nodes[i]);
    statements = sdscat(statements, statement);
    sdsfree(statement);
    switch((node->statements.nodes[i])->type) {
      case AST_TYPE_USE:
        break;
      case AST_TYPE_BLOCK:
        break;
      case AST_TYPE_FUNCTION:
        break;
      default:
        statements = sdscat(statements, ";");
        break;
    }
    statements = sdscat(statements, "\n");
  }
  if (node->type == AST_TYPE_BLOCK) statements = sdscat(statements, "}\n");
  return statements;
}

char *codegen_function(ast_node *node) {
  sds function = sdsnew("");
  function = sdscat(function, node->function.type);
  function = sdscat(function, " ");
  function = sdscat(function, node->function.name);
  function = sdscat(function, "(");
  for (unsigned int i = 0; i < node->function.argc; i++) {
    sds argument = codegen(node->function.arguments[i]);
    function = sdscat(function, argument);
    sdsfree(argument);
    if ((i + 1) < node->function.argc) function = sdscat(function, ",");
  }
  function = sdscat(function, ")");
  sds innerBlock = codegen(node->function.innerBlock);
  function = sdscat(function, innerBlock);
  sdsfree(innerBlock);
  return function;
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
      return sdsnew("");
  }
}
