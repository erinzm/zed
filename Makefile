CC=gcc
FLEX=flex
BISON=bison

CFLAGS=-O2 -Wall -std=c99
#LDFLAGS=

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
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
