# Compilation settings:
CFLAGS=-Wall -Wextra -std=gnu99 -I/usr/include/netpbm $(MYCFLAGS) -pthread
LDFLAGS=-lm -lnetpbm $(MYLDFLAGS)

OPTCFLAGS=$(CFLAGS) -O3 -march=native -DNDEBUG
DBGCFLAGS=$(CFLAGS) -ggdb3

######


.PHONY: all clean asm
all: aim-opt aim-dbg


aim-opt: aim-run.c exercise.c
	$(CC) $(OPTCFLAGS) -o $@ $^ $(LDFLAGS) 

aim-dbg: aim-run.c exercise.c
	$(CC) $(DBGCFLAGS) -o $@ $^ $(LDFLAGS)


exercise-opt.s: exercise.c
	$(CC) $(OPTCFLAGS) -fverbose-asm -S $< -o $@

exercise-dbg.s: exercise.c
	$(CC) $(DBGCFLAGS) -fverbose-asm -S $< -o $@

asm: exercise-dbg.s exercise-opt.s


clean:
	rm -f *.o *.s aim-opt aim-dbg output.pbm
