#include <stdio.h>
#include <argp.h>
#include <stdbool.h>
#include "parser.h"
#include "ast.h"
#include "codegen.h"

const char *argp_program_version = "Zed 0.1";
const char *argp_program_bug_address =
  "https://github.com/ArchimedesPi/zed/issues";
const int argp_flags = 0;
static char doc[] =
  "zed - a compiler";
static struct argp argp = {0, 0, 0, doc, 0, 0, 0};

extern int yyparse();
extern ast_node *parsetree;

int main(int argc, char** argv) {
  argp_parse(&argp, argc, argv, argp_flags, 0, 0);
  yyparse();
  printf("%s", codegen(parsetree));
  ast_node_free(parsetree);
  return 0;
}
