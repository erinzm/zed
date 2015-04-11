CC?=gcc
FLEX?=flex
BISON?=bison

FLEXFLAGS+=
BISONFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 -g -std=c99
LDFLAGS+=-lm

csrc = $(wildcard src/*.c)
flexsrc = $(wildcard src/*.l)
bisonsrc = $(wildcard src/*.y)
flexobj = $(flexsrc:.l=.yy.o)
bisonobj = $(bisonsrc:.y=.tab.o)
cobj = $(csrc:.c=.o)

%.tab.c: %.y
	$(BISON) --defines=$(@:.tab.c=.tab.h) -o $@ $< $(BISONFLAGS)

%.yy.c: %.l
	$(FLEX) -o $@ $< $(FLEXFLAGS)

%.yy.o: %.yy.c
	$(CC) -c -o $@ $<

%.tab.o: %.tab.c
	$(CC) -c -o $@ $<

%.obj: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

compilerinaweek: $(bisonobj) $(flexobj) $(cobj)
	$(CC) $^ -o $@ $(LDFLAGS)


.PHONY: clean
clean:
	rm -f src/*.o src/*.{yy,tab}.c src/*.tab.h compilerinaweek
