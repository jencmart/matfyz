#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

#include "aim.h"


typedef int_fast64_t timestamp_t;

static timestamp_t
get_timer(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return 1000000*t.tv_sec + t.tv_usec;
}

int * seq_load(char *file, int * length) {
	FILE *f = fopen(file, "r");
	if (!f) {
		printf("Could not read input file.\n");
		exit(1);
	}
	char *line = NULL;
	size_t ll = 0, i = 0;
	int * seq = NULL;
	while (getline(&line, &ll, f) != -1) {
		if (seq == NULL) {
			int l = atoi(line);
			seq = malloc(sizeof(int)*l);
			(*length) = l;
		} else {
			seq[i++] = atoi(line);
		}
	}
	fclose(f);
	assert(seq);
	assert(i == (*length));
	return seq;
}

void seq_store(char *file, int *seq, int length) {
	FILE *f = fopen(file, "w");
	fprintf(f, "%i\n", length);
	assert(f);
	for (int i = 0; i < length; i++) {
		fprintf(f, "%i\n", seq[i]);
	}
	fclose(f);
	return;
}


int
main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "%s ITERATIONS SRCSEQ.txt OUTSEQ.txt\n", argv[0]);
		return EXIT_FAILURE;
	}

	int iters = atoi(argv[1]);

	int length;
	int * seq = seq_load(argv[2], &length);
	timestamp_t t0 = get_timer();
	int **seqs = malloc(sizeof(int*)*iters);
	seqs[0] = seq;

	/* Hmm, copy the data!*/
	for (int k = 1; k < iters; k++) {
		seqs[k] = malloc(sizeof(int)*length);
		memcpy(seqs[k], seq, sizeof(int)*length);
	}

	/* Pre-warm the CPU! Neccessary for benchmarking w/ dynamic
	 * cpufreq policy. */
	while (get_timer() - t0 < 500000);
	t0 = get_timer();

	for (int j = 0; j < iters; j++) {
		exercise(seqs[j], length);
	}

	t0 = get_timer() - t0;
	// time spent:
	printf("%.3f\n", (double) t0/1e6);
	seq_store(argv[3], seq, length);
	return EXIT_SUCCESS;
}
