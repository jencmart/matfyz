# Compilation settings:
CFLAGS=-Wall -std=gnu99 -pthread $(MYCFLAGS)
LDFLAGS=-lm $(MYLDFLAGS)

OPTCFLAGS=$(CFLAGS) -ggdb3 -O3 -march=native -DNDEBUG
DBGCFLAGS=$(CFLAGS) -ggdb3

# Benchmarking settings:
RUNS=4
ITERS=20


######


.PHONY: all clean benchmark evaluate asm
all: aim-opt aim-dbg


seq-gen: seq-gen.c
	$(CC) $(OPTCFLAGS) -o $@ $^ $(LDFLAGS) 

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
	rm -f *.o *.s aim-opt aim-dbg output.pbm random

mkdata:
	random `echo '10^6' | bc ` > random.in


benchmark: aim-opt
	@{ echo; \
		top -b -n 5 | head -n 5; \
		echo; \
		echo '>>> Will compute time needed for $(ITERS) iterations averaged over $(RUNS) runs.'; \
		echo '>>> The measured mean time, its probable lower and upper bounds and S.D.'; \
		echo '>>> are printed out. See the header of statistics.awk for details.'; } >&2
	@(for i in `seq 1 $(RUNS)`; do \
		./aim-opt $(ITERS) random.in random.out | tee /dev/stderr; \
	done) | awk -f statistics.awk
