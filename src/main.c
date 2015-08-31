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
  mode = C; // by default, the mode should be outputting C

  if (!source) {
    printf("Can't open %s!\n", cmd.argv[0]);
    return 1;
  }

  // set up flex and bison with the file
  yyin = source;
  // parse the code. now we have an ast.
  yyparse();

  // switch on the mode
  switch (mode) {
        printf("%s", codegen(parsetree));
    case C: { // for c
      if (doStdout) { // if we're outputting the emitted c to stdout
      } else {
        FILE *output = fopen(strcat(basename(cmd.argv[0]), ".ir.c"), "w");
        fprintf(output, "%s", codegen(parsetree));
        fclose(output);
      }
      break;
    }
    case DUMP: { // for an ast dump
      dump_ast_node(parsetree); // dump the ast to stdout. the ast dump is xml-like.
      break;
    }
  }

  return 0;
}
