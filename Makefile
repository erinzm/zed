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
flexc = $(flexsrc:.l=.yy.c)
bisonc = $(bisonsrc:.y=.tab.c)
cobj = $(csrc:.c=.o)
cdep = $(cobj:.o=.d)

%.tab.c: %.y
	$(BISON) --defines=$(@:.tab.c=.tab.h) -o $@ $< $(BISONFLAGS)

%.yy.c: %.l
	$(FLEX) -o $@ $< $(FLEXFLAGS)

%.obj: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

compilerinaweek: $(bisonc) $(flexc) $(cobj)
	$(CC) $^ -o $@ $(LDFLAGS)


-include $(cdep) # include all C depfiles

# generate cdepfiles
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(cobj) $(cdep) $(flexc) $(bisonc) src/*.tab.h compilerinaweek
