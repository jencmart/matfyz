#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aim.h"

struct coord {
	int x;
	int y;
};

static struct coord around[] = {
	{ -1, -1 },
	{  0, -1 },
	{  1, -1 },
	{  1,  0 },
	{  1,  1 },
	{  0,  1 },
	{ -1,  1 },
	{ -1,  0 }
};

void
display(struct image * life) {
	for (size_t j = 0; j < life->rows; j++) {
		for (size_t i = 0; i < life->cols; i++) {
			putchar(".x"[image_getpixel(life, i, j)]);
		}
		putchar('\n');
	}
	putchar('\n');
}

void
exercise(struct image ** life_p, unsigned int gens) {
	struct image * life = *life_p;
	struct image * tmp = image_clone(life);
	//display(life);
	for (unsigned int gen = 0; gen < gens; gen++) {
		for (size_t i = 0; i < life->cols; i++) {
			for (size_t j = 0; j < life->rows; j++) {
				size_t sum = 0;
				for (size_t k = 0; k < sizeof around / sizeof *around; k ++)
					if (image_getpixel(life, i + around[k].x, j + around[k].y))
						sum ++;
				if (sum == 3)
					image_putpixel(tmp, i, j, 1);
				else if (sum == 2 && image_getpixel(life, i, j))
					image_putpixel(tmp, i, j, 1);
				else
					image_putpixel(tmp, i, j, 0);
			}
		}
		struct image * _t = tmp; tmp = life; life = _t;
		//display(life);
	}
	(*life_p) = life;
}

