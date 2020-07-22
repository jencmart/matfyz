all:
	gcc -o comp -g -std=c11 -Wall -Wextra comp.c

clean:
	rm -f comp

samples:
	./makegraph.pl 100 1 > small.g

samples-evil:
	./makegraph.pl 10000000 1 > big.g
	./makegraph.pl 10000000 2 > extra.g

.PHONY: all clean
