CC=gcc
FLEX=flex
BISON=bison

FLEXFLAGS+=
BISONFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 -g -std=c99
LDFLAGS+=-lm

%.o: %.c
	$(CC) -c -o $@ $<

ciaw: src/main.o
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f src/*.o src/*.{yy,tab}.c src/*.tab.h compilerinaweek
