#include "util.h"

char * chopStringTips(char *input) {
  int len = strlen(input);
  if(len > 0)
    input++;//Go past the first char
  if(len > 1)
    input[len - 2] = '\0';//Replace the last char with a null termination
  return input;
}
