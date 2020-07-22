#include <stdio.h>
#include "aim.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

static int qcomp(int *a, int *b) {
	return (*a) - (*b);
}

void exercise(int * seq, size_t length) {
	qsort(seq, length, sizeof(int), (__compar_fn_t) &qcomp);
}
