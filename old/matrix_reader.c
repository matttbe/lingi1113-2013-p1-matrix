#include <stdio.h>
#include <stdlib.h>

#include "matrix_reader.h"

struct matrix_reader {
	FILE *f;
};

matrix_reader_t *
matrix_reader_init(const char *filename)
{
	matrix_reader_t * r;
	
	r = malloc(sizeof(*r));
	if (!r)	
		return NULL;

	r->f = fopen(filename, "r");
	if (!r->f) {
		perror("Unable to open file:");
		free(r);
		return NULL;
	}
		
	return r;
}

matrix_t *
matrix_reader_next(matrix_reader_t *r)
{
	int n_lines, n_columns, val;
	int i, j;
	matrix_t *m;
	
	if (fscanf(r->f,"%dx%d\n", &n_lines, &n_columns) == EOF)
		return NULL;
	
	m = matrix_alloc(n_lines, n_columns);
	if (!m)
		return NULL;
	
	for (i = 0; i < n_lines; ++i) {
		for (j = 0; j < n_columns; ++j) {
			if (fscanf(r->f, "%d", &val) == EOF) {
				matrix_free(m);
				return NULL;
			}
			matrix_set(m, i, j, val);
		}
	}
	return m;
}

void
matrix_reader_free(matrix_reader_t *r)
{
	fclose(r->f);
	free(r);
}
