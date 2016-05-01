/* Zed (originally Compiler In A Week)
   Copyright 2015 Liam Marshall
   Licensed under the GNU GPLv3. See LICENSE for details
   */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include <argp.h>

#include "sds/sds.h"

#include "parser.h"
#include "ast.h"
#include "codegen.h"

extern int yyparse();
extern ast_node *parsetree;
extern FILE *yyin;

// -- setup for argp
const char *argp_program_version = "zedc version beta0.1";
const char *argp_program_bug_address = "Liam Marshall <liam@cpan.org>";
static char doc[] = "zedc - the compiler for the Zed computer programming language";
static char args_doc[] = "file";
static struct argp_option options[] = {
  {"dump",    'd', 0,   0, "Dump AST to stdout"},
  {"stdout",  's', 0,   0, "Send .c.ir output to stdout instead of writing it to a file"},
  {0},
};

// -- struct to hold parsed arguments
struct arguments {
  char *args[1];
  enum {DUMP_MODE, C_MODE} output_mode;
  bool doStdout;
};

// -- function to parse out argp opts
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 'd':
      arguments->output_mode = DUMP_MODE;
      break;
    case 's':
      arguments->doStdout = true;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num > 1)
        argp_usage(state);
      arguments->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage(state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

// -- the actual argparse struct
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char** argv) {
  struct arguments arguments;

  arguments.output_mode = C_MODE;
  arguments.doStdout = false;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // open the source file
  FILE *source = fopen(arguments.args[0], "r");
  if (!source) {
    printf("Can't open %s!\n", argv[1]);
    return 1;
  }

  // set up flex and bison with the file
  yyin = source;
  // parse the code. now we have an ast.
  yyparse();
  sds emitted_code = NULL;

  // switch on the mode
  switch (arguments.output_mode) {
    case C_MODE: { // for c
      // emit c from the ast
      emitted_code = codegen(parsetree);
      if (arguments.doStdout) { // if we're outputting the emitted c to stdout
        printf("%s", emitted_code);
      } else {
        // open a new file, with the name of the original file + '.ir.c'
        FILE *output = fopen(strcat(basename(argv[1]), ".ir.c"), "w");
        fprintf(output, "%s", emitted_code);
        fclose(output);
      }
      break;
    }
    case DUMP_MODE: { // for an ast dump
      dump_ast_node(parsetree); // dump the ast to stdout. the ast dump is xml-like.
      break;
    }
  }
  
  // free the memory of the AST
  ast_node_free(parsetree);
  
  // free the memory of the emitted code
  if (emitted_code != NULL) sdsfree(emitted_code);

  return 0;
}
