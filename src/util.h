#ifndef _util_h
#define _util_h
#include <stdio.h>

// free a pointer if it exists
#define FREE_IF_EXISTS(pointer) if (pointer) free(pointer)
#define STRUCT_INSTANCE(type) malloc(sizeof(type))
#define STRUCT_WIDE_INSTANCE(type, width) malloc(sizeof(type) * width)
#define INSPECT(thing, fmt) printf(#thing " == " fmt "\n", thing)

#endif
