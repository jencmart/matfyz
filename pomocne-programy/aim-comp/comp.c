/******************************************************************************
 * Filename: comp.c
 *
 * Created: 2015/04/26 20:56
 * Author: Ladislav Láska
 * e-mail: laska@kam.mff.cuni.cz
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct vertex {
	struct vertex **neighbours;
	int comp;
};

struct graph {
	int n;
	struct vertex* vertices;
};


struct graph * graph_load( char *file ) {
	FILE *f = fopen(file, "r");
	if (!f) {
		printf("Could not read input file.\n");
		goto fail;
	}
	struct graph *g = calloc(1, sizeof(struct graph));
	
	/* First line contains number of vertices. */
	if (1 != fscanf(f, "%i\n", &g->n)) {
		fprintf(stderr, "Format error: Invalid header.\n");
		goto fail;
	}
	g->vertices = calloc(g->n, sizeof(struct vertex));
	/* Other lines contain it's number, neighbour count and their numbers. */
	for (int i = 0; i < g->n; i++) {
		int id, neighbours;
		if (2 != fscanf(f, "%i %i", &id, &neighbours)) {
			fprintf(stderr, "Format error: Invalid line %i.\n", i);
			goto fail;
		}
		if (id == g->n) {
			fprintf(stderr, "Found vertex #%i, but only %i vertices expected!\n", id, g->n);
			goto fail;
		}
		g->vertices[id].neighbours = calloc(neighbours+1, sizeof(struct vertex*));
		for (int j = 0; j < neighbours; j++) {
			int vref;
			if (1 != fscanf(f, "%i", &vref)) {
				fprintf(stderr, "Format error: Invalid edge %i at vertex %i.\n", j, i);
				goto fail;
			}
			if (vref >= g->n) {
				fprintf(stderr, "Format error: Invalid reference to nonexistent vertex %i.\n", vref);
				goto fail;
			}
			g->vertices[id].neighbours[j] = &g->vertices[vref];
			assert(g->vertices[id].neighbours[j+1] == NULL);
		}
	}
	fclose(f);
	return g;
	fail:
	if (g) {
		if (g->vertices) {
			for (int i = 0; i < g->n; i++) {
				if (g->vertices[i].neighbours)
					free(g->vertices[i].neighbours);
			}
			free(g->vertices);
		}
		free(g);
	}
	return NULL;
}

void mark_component(struct vertex *v, int comp) {
	assert(v);
	v->comp = comp;
	for (int i = 0; v->neighbours[i] != NULL; i++) {
		if (v->neighbours[i]->comp) continue;
		mark_component(v->neighbours[i], comp);
	}
}

int find_components(struct graph *g) {
	int comp = 0;
	for (int i = 0; i < g->n; i++) {
		if (g->vertices[i].comp) continue;
		comp++;
		mark_component(&g->vertices[i], comp);
	}
	return comp;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./comp graph_file.g");
		exit(1);
	}
	struct graph *g = graph_load(argv[1]);
	if (g) {
		fprintf(stderr, "Processing graph '%s' on %i vertices.\n", argv[1], g->n);
		printf("Found %i components.\n", find_components(g));
	}
	return 0;
}
