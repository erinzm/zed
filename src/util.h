#ifndef _util_h
#define _util_h
#include <stdio.h>
#include <string.h>

// free a pointer if it exists.
#define FREE_IF_EXISTS(pointer) if (pointer) free(pointer)
// nice instantiator shorthand for structs.
#define STRUCT_INSTANCE(type) malloc(sizeof(type))
// same as previous but for "wide" structs - arrays of structs.
#define STRUCT_WIDE_INSTANCE(type, width) malloc(sizeof(type) * width)
// inspect a thing - just pass a thing and a printf format string for that thing. great for debug.
#define INSPECT(thing, fmt) printf(#thing " == " fmt "\n", thing)

// chop the lexed "s off of a string
char *chopStringTips(char *input);

#endif
