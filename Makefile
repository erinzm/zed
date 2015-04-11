CC=gcc
FLEX=flex
BISON=bison

CFLAGS=-O2 -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 -g -std=c99
LDFLAGS=-lm

csrc = $(wildcard src/*.c)
flexsrc = $(wildcard src/*.l)
obj = $(csrc:.c=.o)
dep = $(obj:.o=.d)

compilerinaweek: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

-include $(dep) # include all depfiles

# generate depfiles
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@


.PHONY: clean
clean:
	rm -f $(obj) compilerinaweek

.PHONY: cleandep
cleandep:
	rm -f $(dep)
