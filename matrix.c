#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "matrix.h"

// Info about the row
typedef struct {
    int iRowNo;        // Row number
    Node *pFirstNode;  // First Node of the row
    RowInfo *pNextRow; // Next Row
} RowInfo;

// Info about the column
typedef struct {
    int iColNo;        // Col number
    Node *pFirstNode;  // First node of the column
    ColInfo *pNextCol; // Next column
} ColInfo;

// A node should contain info about its position (row X col), the value (here: int), and the next elements (right/down)
typedef struct {
    int iRow;
    int iCol;
    int iData; // or we can use generic data...
    Node *pNextRight;
    Node *pNextDown;
} Node;

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
	
  // set the numbers of rows and columns
  pmatrix->iNbRows = rows;
  pmatrix->inbCols = columns;
	
  // set the columns and rows to null, new matrix consdered as empty
  pmatrix->pFirstRow = NULL;
  pmatrix->pFirstCol = NULL;
	
  // set de state of the matrix
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
