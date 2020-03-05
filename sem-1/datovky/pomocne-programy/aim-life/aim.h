#ifndef AIM__AIM_H
#define AIM__AIM_H

#include <stdbool.h>

struct image {
	unsigned int cols, rows;
	/* Black-white image, one _BIT_ per pixel. Rows-oriented,
	 * rows are rounded to whole bytes. */
	unsigned char *bitmap;
};

static unsigned int image_rowbytes(struct image *img);
static bool image_getpixel(struct image *img, unsigned int x, unsigned int y);
static void image_putpixel(struct image *img, unsigned int x, unsigned int y, bool pixel);

void exercise(struct image ** life, unsigned int gens);


/** Implementation: */

static inline unsigned int
image_rowbytes(struct image *img)
{
	return (img->cols + 7) / 8;
}

static inline bool
image_getpixel(struct image *img, unsigned int x, unsigned int y)
{
	if ((x >= img->cols) || y >= img->rows) return 0;
	return (img->bitmap[image_rowbytes(img) * y + x / 8] >> (7 - x % 8)) & 1;
}

static inline void
image_putpixel(struct image *img, unsigned int x, unsigned int y, bool pixel)
{
	(img->bitmap[image_rowbytes(img) * y + x / 8] &= ~(1 << (7 - x % 8)));
	(img->bitmap[image_rowbytes(img) * y + x / 8] |= pixel << (7 - x % 8));
}

struct image *
image_clone(struct image *image);

void
image_clear(struct image *image);

#endif
