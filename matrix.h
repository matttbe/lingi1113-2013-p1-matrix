/*
 * matrix.h
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



#ifndef __MATRIX_H__
#define __MATRIX_H__


typedef enum _MatrixState {
	MATRIX_BUSY=0,
	MATRIX_FREE
} MatrixState;

typedef struct matrix matrix_t;


/**
 * allocate the right memory
 */
matrix_t * matrix_alloc (int iNbRows, int iNbCols);

/**
 * free the allocated memory
 */
void matrix_free (matrix_t *m);

/**
 * Set the value val in m at [iRow][iCol], the first element is in 0x0.
 */
void matrix_set (matrix_t *m, int iRow, int iCol, int iData);

/**
 * Get the value in m at [iRow][iCol], the first element is in 0x0.
 */
int matrix_get (matrix_t *m, int iRow, int iCol);

/**
 * Change the state of the matrix
 */
void matrix_set_state (matrix_t *m, MatrixState state);

/**
 * Get the state of the matrix
 */
MatrixState matrix_get_state (matrix_t *m);

/**
 * Print to stdout the matrix
 */
void matrix_print (matrix_t *m);

/**
 * An easy way to do the matricial product between two matrix
 */
matrix_t * matrix_product (matrix_t *m1, matrix_t *m2);

#endif
