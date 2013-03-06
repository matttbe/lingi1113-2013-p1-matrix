/*
 * compute_no_thread.c
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
// matrix
#include "matrix_reader.h"
#include "matrix.h"
// Double linked lists
#include "DLinkedList.h"
#include "compute_no_thread.h"

/**
 * @pre: pReader should contain at least 2 matrix.
 */
matrix_t * compute_all_matrix_in_one_thread (matrix_reader_t* pReader)
{
	matrix_t *pNextMatrix, *pPrevMatrix, *pCurrentMatrix;

	pPrevMatrix = matrix_reader_next (pReader);
	if (! pPrevMatrix)
	{
		fprintf (stderr, "No matrix\n");
		exit (EXIT_FAILURE);
	}
	pNextMatrix = matrix_reader_next (pReader);
	if (! pNextMatrix)
	{
		free (pPrevMatrix);
		fprintf (stderr, "No enough matrix (only one)\n");
		exit (EXIT_FAILURE);
	}

	while (pNextMatrix != NULL)
	{
		pCurrentMatrix = matrix_product (pPrevMatrix, pNextMatrix);
		matrix_free (pPrevMatrix);
		matrix_free (pNextMatrix);
		pPrevMatrix = pCurrentMatrix;
		pNextMatrix = matrix_reader_next (pReader);
	}

	return pCurrentMatrix;
}

