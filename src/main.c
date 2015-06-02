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
sds outName;


void cmdIsDump(command_t *self) {
  mode = DUMP;
}

void cmdIsC(command_t *self) {
  mode = C;
}

void cmdStdOut(command_t *self) {
  doStdout = true;
}

int main(int argc, char** argv) {
  outName = sdsnew("a.out");
  mode = C;
  command_t cmd;
  command_init(&cmd, argv[0], "0.1");
  cmd.usage = "[options] <inputfile>";
  command_option(&cmd, "-d", "--dump", "dump AST", cmdIsDump);
  command_option(&cmd, "-c", "--c", "output C", cmdIsC);
  command_option(&cmd, "-s", "--stdout", "output to stdout instead of a file", cmdStdOut);
  command_parse(&cmd, argc, argv);
  FILE *source = fopen(cmd.argv[0], "r");
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
        FILE *output = fopen(strcat(cmd.argv[0], ".ir.c"), "w");
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
