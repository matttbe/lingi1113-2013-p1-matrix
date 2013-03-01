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

#define IF_ERROR_WITH_MSG(_cond, _sentence, _message) do {\
	if (_cond) {\
		fprintf (stderr, _sentence, _message);\
		exit (EXIT_FAILURE); }} while (0)

#define IF_ERROR_MATRIX(_pMatrix) IF_ERROR_WITH_MSG (! _pMatrix, "ERROR: attempting to reach a non allocated matrix in function: %s\n", __func__)
#define IF_ERROR_ALLOC(_pMatrix) IF_ERROR_WITH_MSG (! _pMatrix, "ERROR: when trying to allocate the memory for the matrix (%s)\n", __func__)


matrix_t * matrix_alloc (int iNbRows, int iNbCols)
{
	// the structure
	matrix_t *pMatrix = (matrix_t *) malloc (sizeof (matrix_t));

	IF_ERROR_ALLOC (pMatrix);

	// set the numbers of rows and columns
	pMatrix->iNbRows = iNbRows;
	pMatrix->iNbCols = iNbCols;

	// set the columns and rows to null, new matrix consdered as empty
	pMatrix->pFirstRow = NULL;
	pMatrix->pFirstCol = NULL;

	// set de state of the matrix
	pMatrix->bState = MATRIX_BUSY;	// busy by default

	return pMatrix;
}

// we need to free each ColInfo, RowInfo, (data), Node, matrix
void matrix_free (matrix_t *m)
{
	IF_ERROR_MATRIX (m);

	if (m->pFirstCol)
	{
		ColInfo *pCol = m->pFirstCol, *pCurrCol;
		while (pCol != NULL) // each columns
		{
			pCurrCol = pCol;
			pCol = pCurrCol->pNextCol;
			free (pCurrCol);
		}
	}
	if (m->pFirstRow)
	{
		RowInfo *pRow = m->pFirstRow, *pCurrRow;
		Node *pNode, *pCurrNode;
		while (pRow != NULL) // each rows
		{
			pCurrRow = pRow;
			pRow = pCurrRow->pNextRow;
			pNode = pCurrRow->pFirstNode;
			while (pNode != NULL) // each node
			{
				pCurrNode = pNode;
				pNode = pCurrNode->pNextRight;
				free (pCurrNode);
				// no need to free data here
			}
			free (pCurrRow);
		}
	}

	free (m); // the structure
}

///TODO: if val = 0, return; --> if it's only used to add result (and not to modify some of them)
void matrix_set (matrix_t *m, int iRow, int iCol, int iData)
{
	IF_ERROR_MATRIX (m);

	IF_ERROR_WITH_MSG (iRow > m->iNbRows || iCol > m->iNbCols, "ERROR: attempting to set a value outsite of the matrix (%s)\n", __func__);

  if (i > m->iNbRows || j > m->iNbCols){
    //erreur, on sort des bornes
  }
	
  // cas ou pas encore de ligne implique pas encore de colone non plus
  if (m->pFirstRow == NULL) {
		
    RowInfo *pNewRow = (RowInfo*) malloc (sizeof (RowInfo));
    ColInfo *pNewCol = (ColInfo*) malloc (sizeof (ColInfo));
    Node *pNewNode = (Node*) malloc (sizeof (Node));
		
    // test si les mallocs se sont bien passes
    if (!pNewRow || !pNewCol || !pNewNode){
      fprintf (stderr, "ERROR: when trying to allocate the memory for the new node\n");
      if (pNewRow){free(pNewRow);}
      if (pNewCol){free(pNewCol);}
      if (pNewNode){free(pNewNode);}

    }else{

      m->pFirstRow = pNewRow;
      m->pFirstCol = pNewCol;
	  
      pNewRow->iRowNo = i;
      pNewRow->pFirstNode = pNewNode;
      pNewRow->pNewRow = NULL;

      pNewCol->iColNo = j;
      pNewCol->pFirstNode = pNewNode;
      pNewCol->pNewCol = NULL;
			
      pNewNode->iRow = i;
      pNewNode->iCol = j;
      pNewNode->iData = val;
      pNewNode->pNextRight = NULL;
      pNewNode->pNextDown = NULL;		

    }

  }else{
    
    RowInfo *tempRow;
    ColInfo *tempCol;

    if (m->pFirstRow->iRowNo > i){
      
    
    
  }
		
	
	
	
    RowInfo rowpres = NULL;
    RowInfo row = m->pFirstRow;
    ColInfo colpres = NULL;
    ColInfo col = m->pFirstCol;
	
    int iterpres = 0;
    int iternew = row->iRowNo;
    bool end = false;
    while (iternew < i && iternew <= m->iNbRows && !end){
      iterpres = iternew;
      rowpres = row;
      if (rowpres->pNextRow == NULL
	  iternew = rowpres->iRowNo;
	  row = rowpres->pNextRow;
	  }
      if (! iternew == i) { // row doesn't exist	
		
		 
	
	
	IF_ERROR_MATRIX (m);
	// TODO: error? What do we do? + check limit matrix?
	// maybe useless to check each time... maybe safer but it's just used by us...

	m->iMatrix[i][j] = val;
      }
}

/**
 * Return the right node or NULL if "error"
 */
static Node * _get_right_node_from_row (RowInfo *pRow, int iCol)
{
	Node *pNode = pRow->pFirstNode;
	while (pNode != NULL)
	{
		if (pNode->iCol < iCol)
			pNode = pNode->pNextRight; // next
		else if (pNode->iCol > iCol)
			return NULL; // "error"
		else /*if (pNode->iCol == iCol)*/
			return pNode; // right node
	}
	return NULL; // "error"
}

int matrix_get (matrix_t *m, int iRow, int iCol)
{
	IF_ERROR_MATRIX (m);

	RowInfo *pRow = m->pFirstRow;
	while (pRow != NULL)
	{
		if (pRow->iRowNo < iRow)
			pRow = pRow->pNextRow;
		else if (pRow->iRowNo > iRow) // no node on this row: "error"
			break;
		else /*if (pRow->iRowNo == iRow)*/ // the right row
		{
			Node *pNode = _get_right_node_from_row (pRow, iCol);
			if (pNode)
				return pNode->iData;
			break; // "error", no right node
		}
	}
	// the element doen't exist, it's 0...
	return 0;
}

void matrix_set_state (matrix_t *m, MatrixState state)
{
	IF_ERROR_MATRIX (m);

	m->bState = state;
}

MatrixState matrix_get_state (matrix_t *m)
{
	IF_ERROR_MATRIX (m);

	return m->bState;
}

void matrix_print (matrix_t *m)
{
	IF_ERROR_MATRIX (m);

	int iRow, iCol;
	RowInfo *pRow = m->pFirstRow;
	Node *pNode;
	for (iRow = 1 ; iRow <= m->iNbRows ; iRow++) // each rows, we start from 1 -> NbRows
	{
		if (pRow && pRow->iRowNo == iRow) // this row exists
		{
			pNode = pRow->pFirstNode;
			pRow = pRow->pNextRow;
		}
		else
			pNode = NULL;

		for (iCol = 1 ; iCol <= m->iNbCols ; iCol++) // each col
		{
			if (pNode && pNode->iCol == iCol) // this elem exists
			{
				printf ("%d ", pNode->iData);
				pNode = pNode->pNextRight;
			}
			else
				printf ("0 ");
		}
		printf ("\n");
	}
}

static Node * _get_right_node_in_col_from_node (Node *pNode, int iRow)
{
	while (pNode != NULL)
	{
		if (pNode->iRow < iRow)
			pNode = pNode->pNextDown; // next
		else if (pNode->iRow > iRow)
			return NULL; // "error"
		else /*if (pNode->iRow == iRow)*/
			return pNode; // right node
	}
	return NULL; // "error"
}

matrix_t * matrix_product (matrix_t *m1, matrix_t *m2)
{
	IF_ERROR_MATRIX (m1);
	IF_ERROR_MATRIX (m2);

	///TODO test if we can do the product?

	matrix_t *pMatrixResult = matrix_alloc (m1->iNbRows, m2->iNbCols);

	if (! pMatrixResult)
		return NULL; // TODO: error? What do we do?

	RowInfo *pRowM1 = m1->pFirstRow;
	ColInfo *pColM2;
	Node *pNodeM1, *pNodeM2, *pNode;
	int iResult;
	while (pRowM1 != NULL) // each row
	{
		pColM2 = m2->pFirstCol;
		while (pColM2 != NULL) // each col
		{
			iResult = 0;
			pNodeM2 = pColM2->pFirstNode;
			pNodeM1 = pRowM1->pFirstNode;
			while (pNodeM1 != NULL) // each node of the row
			{
				pNode = _get_right_node_in_col_from_node (pNodeM2, pRowM1->iRowNo);
				if (pNode) // can compute
				{
					pNodeM2 = pNode;
					iResult += pNodeM1->iData * pNodeM2->iData;
				}

				pNodeM1 = pNodeM1->pNextRight;
			}
			matrix_set (pMatrixResult, pRowM1->iRowNo, pColM2->iColNo, iResult);
		}

		pRowM1 = pRowM1->pNextRow;
	}

	return pMatrixResult;
}
