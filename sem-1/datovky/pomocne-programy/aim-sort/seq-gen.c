#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define swap(a,b) do { __typeof(a) t = a; a = b; b = t; } while (0);
void qs(int *seq, int l, int r){
	if(l < r){
		int p = l;
		for(int i = l + 1; i < r; i++){
			if(seq[i] < seq[l]){
				p++;
				swap(seq[i], seq[p]);
			}
		}
		swap(seq[l], seq[p]);
		qs(seq, l, p);
		qs(seq, p + 1, r);
	}
}

int main(int argc, char **argv) {
	if (argc < 3 || argc > 4) { fprintf(stderr, "Usage: [random|sorted|reverse] %s count repeat\n", argv[0]); exit(1); }
	int count = atoi(argv[2]);
	int repeat = 1;
	char *type = argv[1];
	if (argc == 4) repeat = atoi(argv[3]);

	if (count <= 0) {
		fprintf(stderr, "Invalid count given: %i\n", count);
		exit(2);
	}

	if (repeat < 1) {
		fprintf(stderr, "Invalid repeat given: %i\n", repeat);
		exit(2);
	}

	int *data = malloc(sizeof(int)*count);

	fprintf(stderr, "Generating %i %s sequences, each of size %i\n", repeat, type, count);

	for (int i = 0; i < count; i++) {
		data[i] = rand();
	}

	if ((!strcmp(type,"sorted"))||(!strcmp(type,"reverse"))) {
		qs(data, 0, count);
		if (!strcmp(type, "reverse")) {
			for (int i = 0; i < count/2; i++) {
				swap(data[i],data[count-i-1]);
			}
		}
	}

	/* First, generate a random sequence */
	printf("%i\n", count*repeat);
	for (int r = 0; r < repeat; r++) {
		for (int i = 0; i < count; i++) {
			printf("%i\n", data[i]);
		}
	}
}
