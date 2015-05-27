CC=gcc
FLEX=flex
BISON=bison

FLEXFLAGS+=
BISONFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wpedantic -Winit-self -Wfloat-equal -Wformat=2 -Wno-unused-function -g -std=gnu11
LDFLAGS+=-lm

TESTS=$(wildcard test/*.c)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.c %.h: %.y
	$(BISON) $(BISONFLAGS) --defines=$*.h --output=$@ $<

%.c: %.l
	$(FLEX) $(FLEXFLAGS) -o $@ $<

ciaw: src/ast.o src/parser.o src/lexer.o src/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

lextest: src/parser.o src/lexer.o src/lextest.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: test
test: $(TESTS)
	@sh tests/runtests.sh

.PHONY: clean
clean:
	rm -f src/*.o src/parser.h src/{parser,lexer}.c ciaw lextest
