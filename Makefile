CC:=clang
FLEX:=flex
BISON:=bison

PREFIX:=/usr/local
DEPSPATH=deps

FLEXFLAGS+=
BISONFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wpedantic -Winit-self -Wfloat-equal -Wformat=2 -Wno-unused-function -Wno-format-nonliteral -Wno-unused-parameter -g -std=gnu11 -I$(DEPSPATH)
LDFLAGS+=-lm

DEPSSRC = $(wildcard deps/*/*.c)
DEPSOBJS = $(DEPSSRC:.c=.o)

TESTS=$(wildcard test/*.c)

all: clean zed


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.c %.h: %.y
	$(BISON) $(BISONFLAGS) --defines=$*.h --output=$@ $<

%.c: %.l
	$(FLEX) $(FLEXFLAGS) -o $@ $<

zed: $(DEPSOBJS) src/util.o src/ast.o src/codegen.o src/parser.o src/lexer.o src/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

install: zed
	install zed $(PREFIX)/bin/zed

.PHONY: test
test: $(TESTS)
	@sh tests/runtests.sh

watch:
	while true ; do \
		make all; \
		inotifywait -qre close_write .;\
	done

.PHONY: clean
clean:
	rm -f src/*.o src/lib/**/*.o src/parser.h src/{parser,lexer}.c zed
