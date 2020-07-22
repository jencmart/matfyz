CC=gcc
LD=gcc
CFLAGS=-O2 -Wall -W -Wno-parentheses -Wstrict-prototypes -Wmissing-prototypes -Wundef -Wredundant-decls -std=gnu99
# CFLAGS+=-funroll-loops

all: access

access: access.c
access: CFLAGS+=$(F)
F=-DSIZE=16 -DMS=1000

%.s: %.c
	$(CC) $(CFLAGS) $(F) -S -o $@ $^

clean:
	rm -f `find . -name "*~" -or -name "*.[oa]" -or -name "\#*\#" -or -name TAGS -or -name core -or -name .depend -or -name .#*`
	rm -f access a-* plot.gp
