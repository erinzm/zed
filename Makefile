CC?=gcc
FLEX?=flex
BISON?=bison

CFLAGS+=-O2 -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 -g -std=c99
LDFLAGS+=-lm

csrc = $(wildcard src/*.c)
flexsrc = $(wildcard src/*.l)
cobj = $(csrc:.c=.o)
cdep = $(cobj:.o=.d)

compilerinaweek: $(cobj)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


# generate cdepfiles
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: cleanall
cleanall: clean cleancdep

.PHONY: clean
clean:
	rm -f $(cobj) compilerinaweek

.PHONY: cleancdep
cleancdep:
	rm -f $(cdep)
