#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pbm.h>
#include <string.h>

#include "aim.h"


#if CHAR_BIT != 8
#error char size larger than 8 is not supported
#endif


static struct image *
image_init(int cols, int rows)
{
	struct image *img = malloc(sizeof(*img));
	img->cols = cols, img->rows = rows;
	img->bitmap = malloc(img->rows * image_rowbytes(img));
	return img;
}

static struct image *
image_load(char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (!f) { perror("load"); exit(EXIT_FAILURE); }

	int cols, rows, fmt;
	pbm_readpbminit(f, &cols, &rows, &fmt);
	struct image *img = image_init(cols, rows);

	for (unsigned int i = 0; i < img->rows; i++)
		pbm_readpbmrow_packed(f, img->bitmap + i * image_rowbytes(img),
				img->cols, fmt);

	fclose(f);
	return img;
}

void
image_clear(struct image *image) {
	memset(image->bitmap, 0, image->rows * image_rowbytes(image));
}

struct image *
image_clone(struct image *image) {
	struct image *clone = image_init(image->cols, image->rows);
	memcpy(clone->bitmap, image->bitmap, image->rows * image_rowbytes(image));
	return clone;
}

static void
image_save(char *filename, struct image *img)
{
	FILE *f = fopen(filename, "wb");
	if (!f) { perror("save"); exit(EXIT_FAILURE); }

	pbm_writepbminit(f, img->cols, img->rows, 0);
	for (unsigned int i = 0; i < img->rows; i++)
		pbm_writepbmrow_packed(f, img->bitmap + i * image_rowbytes(img),
				img->cols, 0);

	fclose(f);
}

static void
image_free(struct image *img)
{
	free(img->bitmap);
	free(img);
}


typedef int_fast64_t timestamp_t;

static timestamp_t
get_timer(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return 1000000*t.tv_sec + t.tv_usec;
}


int
main(int argc, char *argv[])
{
	if (argc != 5) {
		fprintf(stderr, "%s ITERATIONS GENERATIONS SRCIMAGE.pbm DSTIMAGE.pbm\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct image *img_in = image_load(argv[3]);

	int iters = atoi(argv[1]);
	int gens = atoi(argv[2]);

	/* Copy the image for iterations */
	struct image **clones = malloc(sizeof(struct image*)*iters);
	for (int i = 0; i < iters; i++)
		clones[i] = image_clone(img_in);

	timestamp_t t0 = get_timer();

	/* Pre-warm the CPU! Neccessary for benchmarking w/ dynamic
	 * cpufreq policy. */
	while (get_timer() - t0 < 500000);


	t0 = get_timer();
	for (int j = 0; j < iters; j++) {
		exercise(&clones[j], gens);
	}

	t0 = get_timer() - t0;
	// time spent:
	printf("%.3f\n", (double) t0/1e6);
	image_save(argv[4], clones[iters-1]);
	image_free(img_in);
	return EXIT_SUCCESS;
}
