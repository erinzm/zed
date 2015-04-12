#include <stdio.h>

extern void yylex();

int main(void) {
    yylex();
    return 0;
}
