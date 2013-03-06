#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "matrix.h"

struct matrix {
	int iRows;
	int iCols;
	int **iMatrix;
	MatrixState bState;
};

#define IF_ERROR(_pMatrix, _message) do {\
	if (! _pMatrix) {\
		fprintf (stderr, "ERROR: attempting to reach a non allocated matrix in function: %s\n", _message);\
		exit (EXIT_FAILURE); }} while (0)


matrix_t * matrix_alloc (int rows, int columns)
{
	// the structure
	matrix_t *pMatrix = (matrix_t*) malloc (sizeof (matrix_t));
	if (! pMatrix)
	{
		fprintf (stderr, "ERROR: when trying to allocate the memory for the matrix\n");
		return NULL;
	}

	// tabs of tabs
	pMatrix->iMatrix = (int **) malloc (rows * sizeof (int*));

	int i, j;
	for (i = 0 ; i < rows ; i++)
	{
		// tabs in tabs
		pMatrix->iMatrix[i] = (int *) malloc (columns * sizeof (int));
		if (! pMatrix->iMatrix[i]) // IF_ERROR
		{
			for (j = 0 ; j < i ; j++)
				free (pMatrix->iMatrix[j]);

			free (pMatrix->iMatrix);
			free (pMatrix);
			fprintf (stderr, "ERROR: when trying to allocate the memory for the row %d\n", i);
			return NULL;
		}
	}

	pMatrix->iRows = rows;
	pMatrix->iCols = columns;
	pMatrix->bState = MATRIX_BUSY; // busy par défaut

	return pMatrix;
}

void matrix_free (matrix_t *m)
{
	IF_ERROR (m, __func__);

	for (int i = 0 ; i < m->iRows ; i++)
		free (m->iMatrix[i]); // each columns

	free (m->iMatrix); // the row
	free (m); // the structure
}

void matrix_set (matrix_t *m, int i, int j, int val)
{
	IF_ERROR (m, __func__);
		// TODO: error? What do we do? + check limit matrix?*/
		// maybe useless to check each time... maybe safer but it's just used by us...

	m->iMatrix[i][j] = val;
}

int matrix_get (matrix_t *m, int i, int j)
{
	IF_ERROR (m, __func__);
		// maybe useless to check each time... maybe safer but it's just used by us...

	return m->iMatrix[i][j];
}

void matrix_set_state (matrix_t *m, MatrixState state)
{
	IF_ERROR (m, __func__);

	m->bState = state;
}

MatrixState matrix_get_state (matrix_t *m)
{
	IF_ERROR (m, __func__);

	return m->bState;
}

void matrix_print_to_stream (FILE *stream, matrix_t *m)
{
	IF_ERROR (m, __func__);

	int i, j;
	for (i = 0 ; i < m->iRows ; i++)
	{
		for (j = 0 ; j < m->iCols ; j++)
			fprintf (stream, "%d ", m->iMatrix[i][j]);
		fprintf (stream, "\n");
	}
	//printf ("\n");
}

matrix_t * matrix_product (matrix_t *m1, matrix_t *m2)
{
	IF_ERROR (m1, __func__);
	IF_ERROR (m2, __func__);

	// TODO test limits?

	int i, j, k;
	matrix_t *pMatrixResult = matrix_alloc (m1->iRows, m2->iCols);

	if (! pMatrixResult)
		return NULL; // TODO: error? What do we do?

	for (i = 0 ; i < pMatrixResult->iRows ; i++) 
	{
		for (j = 0 ; j < pMatrixResult->iCols ; j++) 
		{
			pMatrixResult->iMatrix[i][j] = 0;
			for (k = 0 ; k < m1->iCols ; k++)
				pMatrixResult->iMatrix[i][j] += m1->iMatrix[i][k] * m2->iMatrix[k][j];
		}
	}

	return pMatrixResult;
}
