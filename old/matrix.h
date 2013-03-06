#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>

typedef enum _MatrixState {
	MATRIX_BUSY=0,
	MATRIX_FREE
} MatrixState; 

typedef struct {
	int iRows;
	int iCols;
	int **iMatrix;
	MatrixState bState;
} matrix_t;


/**
 * allocate the right memory
 */
matrix_t *
matrix_alloc (int rows, int columns);

/**
 * free the allocated memory
 */
void
matrix_free (matrix_t *m);

/**
 * Set the value val in m at [i][j]
 */
void
matrix_set (matrix_t *m, int i, int j, int val);

/**
 * Get the value in m at [i][j]
 */
int
matrix_get (matrix_t *m, int i, int j);

/**
 * Change the state of the matrix
 */
void
matrix_set_state (matrix_t *m, MatrixState state);

/**
 * Get the state of the matrix
 */
MatrixState
matrix_get_state (matrix_t *m);

/**
 * Print to stream the matrix
 */
void matrix_print_to_stream (FILE *stream, matrix_t *m);

/**
 * Print to stdout the matrix
 */
#define matrix_print(m) matrix_print_to_stream (stdout, m)

/**
 * An easy way to do the matricial product between two matrix
 */
matrix_t *
matrix_product (matrix_t *m1, matrix_t *m2);

#endif
