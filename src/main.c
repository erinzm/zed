#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "commander/commander.h"
#include "sds/sds.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

extern int yyparse();
extern ast_node *parsetree;
extern FILE *yyin;

enum {
  DUMP,
  C
} mode;

bool doStdout = false;

int main(int argc, char** argv) {
  mode = C;
  if (!source) {
    printf("Can't open %s!\n", cmd.argv[0]);
    return 1;
  }
  yyin = source;
  yyparse();

  switch (mode) {
    case C: {
      if (doStdout) {
        printf("%s", codegen(parsetree));
      } else {
        FILE *output = fopen(strcat(basename(cmd.argv[0]), ".ir.c"), "w");
        fprintf(output, "%s", codegen(parsetree));
        fclose(output);
      }
      break;
    }
    case DUMP: {
      dump_ast_node(parsetree);
      break;
    }
  }

  return 0;
}
