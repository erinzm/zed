#include <stdio.h>
#include <stdbool.h>
#include "commander/commander.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

extern int yyparse();
extern ast_node *parsetree;

int main(int argc, char** argv) {
  command_t cmd;
  command_init(&cmd, argv[0], "0.1");
  cmd.usage = "[options] <inputfile>";
  command_parse(&cmd, argc, argv);

  yyparse();
  // dump_ast_node(parsetree);
  printf("%s", codegen(parsetree));
  ast_node_free(parsetree);
  return 0;
}
