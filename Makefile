CC?=gcc
FLEX?=flex
BISON?=bison

FLEXFLAGS+=
CFLAGS+=-O2 -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 -g -std=c99
LDFLAGS+=-lm

csrc = $(wildcard src/*.c)
flexsrc = $(wildcard src/*.l)
flexc = $(flexsrc:.l=.yy.c)
cobj = $(csrc:.c=.o)
cdep = $(cobj:.o=.d)

%.yy.c: %.l
	$(FLEX) -o $@ $< $(FLEXFLAGS)

%.obj: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

compilerinaweek: $(cobj) $(flexc)
	$(CC) $^ -o $@ $(LDFLAGS)


-include $(cdep) # include all C depfiles

# generate cdepfiles
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: cleanall
cleanall: clean cleancdep

.PHONY: clean
clean:
	rm -f $(cobj) $(flexc) compilerinaweek

.PHONY: cleancdep
cleancdep:
	rm -f $(cdep)
