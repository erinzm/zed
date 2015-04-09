CC=gcc
FLEX=flex
BISON=bison

CFLAGS=-O2 -Wall -std=c99
#LDFLAGS=

src = $(wildcard src/*.c)
obj = $(src:.c=.o)

all: compilerinaweek
compilerinaweek: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) compilerinaweek
