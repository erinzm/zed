/* Zed (originally Compiler In A Week)
   Copyright 2015 Liam Marshall
   Licensed under the GNU GPLv3. See LICENSE for details
   */
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

  // open the source file
  FILE *source = fopen(argv[1], "r");
  if (!source) {
    printf("Can't open %s!\n", argv[1]);
    return 1;
  }

  // set up flex and bison with the file
  yyin = source;
  // parse the code. now we have an ast.
  yyparse();

  // emit c from the ast
  sds emitted_code = codegen(parsetree);

  // switch on the mode
  switch (mode) {
    case C: { // for c
      if (doStdout) { // if we're outputting the emitted c to stdout
        printf("%s", emitted_code);
      } else {
        // open a new file, with the name of the original file + '.ir.c'
        FILE *output = fopen(strcat(basename(argv[1]), ".ir.c"), "w");
        fprintf(output, "%s", emitted_code);
        fclose(output);
      }
      break;
    }
    case DUMP: { // for an ast dump
      dump_ast_node(parsetree); // dump the ast to stdout. the ast dump is xml-like.
      break;
    }
  }

  // free the memory of the emitted code
  sdsfree(emitted_code);

  return 0;
}
