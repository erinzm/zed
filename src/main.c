#include <stdio.h>
#include <argp.h>
#include <stdbool.h>
#include "parser.h"

const char *argp_program_version = "compilerinaweek 0.1";
const char *argp_program_bug_address =
  "https://github.com/ArchimedesPi/compilerinaweek/issues";
const int argp_flags = 0;
static char doc[] =
  "Compiler In A Week - @archimedespi's personal project to learn about compilers";
static struct argp argp = {0, 0, 0, doc, 0, 0, 0};

int main(int argc, char** argv) {
  argp_parse(&argp, argc, argv, argp_flags, 0, 0);
  yyparse();
  return 0;
}
