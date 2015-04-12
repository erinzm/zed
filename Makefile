CC=gcc
CXX=g++
FLEX=flex
BISON=bison

FLEXFLAGS+=
BISONFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wpedantic -Wfloat-equal -Wformat=2 -g -std=c99
CXXFLAGS+=-O2 -Wall -Wextra -Werror -Wpedantic -Wfloat-equal -Wformat=2 -g -std=c++11
LDFLAGS+=-lm

%.o: %.c
	$(CC) -c -o $@ $<

%.o: %.cpp
	$(CXX) -c -o $@ $<

%.c %.h: %.y
	$(BISON) --defines=$*.h --output=$@ $<

%.c: %.l
	$(FLEX) -d -o $@ $<

ciaw: src/parser.o src/lexer.o src/main.o
	$(CC) -o $@ $^ $(CFLAGS)

lextest: src/parser.o src/lexer.o src/lextest.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f src/*.o src/parser.h src/{parser,lexer}.c ciaw lextest
