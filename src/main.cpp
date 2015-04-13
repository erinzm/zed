#include <stdio.h>
#include "cli.h"
#include "parser.h"

int main(int argc, char** argv) {
  argp_parse(&argp, argc, argv, argp_flags, 0, 0);
  yyparse();
  return 0;
}
