/*
 * matrix.c
 * 
 * Copyright 2013 UCLouvain / pole INGI
 *
 * Authors:
 *   Matthieu Baerts <matthieu.baerts@student.uclouvain.be>
 *   Hélène Verhaeghe <helene.verhaeghe@student.uclouvain.be>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "matrix.h"

typedef struct _Node Node;
typedef struct _RowInfo RowInfo;
typedef struct _ColInfo ColInfo;

// A node should contain info about its position (row X col), the value (here: int), and the next elements (right/down)
struct _Node {
    int iRow;
    int iCol;
    int iData; // or we can use generic data...
    Node *pNextRight;
    Node *pNextDown;
};

// Info about the row
struct _RowInfo {
    int iRowNo;        // Row number
    Node *pFirstNode;  // First Node of the row
    RowInfo *pNextRow; // Next Row
};

// Info about the column
struct _ColInfo {
    int iColNo;        // Col number
    Node *pFirstNode;  // First node of the column
    ColInfo *pNextCol; // Next column
};

// info about the matrix: the first row, the first col and their numbers
struct matrix {
    int iNbRows;
    int iNbCols;
    RowInfo *pFirstRow;
    ColInfo *pFirstCol;
    MatrixState bState;
} ;

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

void matrix_print (matrix_t *m)
{
	IF_ERROR (m, __func__);

	int i, j;
	for (i = 0 ; i < m->iRows ; i++)
	{
		for (j = 0 ; j < m->iCols ; j++)
			printf ("%d ", m->iMatrix[i][j]);
		printf ("\n");
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
