#include <pthread.h>
// matrix
#include "matrix_reader.h"
#include "matrix.h"
// Double linked lists
#include "DLinkedList.h"
//semaphores
#include <semaphore.h>
//errors
#include "misc.h"

extern DLinkedList_t* g_pListHead;
extern sem_t g_aEmpty;
extern pthread_mutex_t g_aMutexProducer;
extern pthread_mutex_t g_aMutexProducerEnd;
extern pthread_mutex_t g_aMutexFile;
extern matrix_reader_t* g_pReader;
extern int g_iProducerEnd;

// to check the state of the previous mode and do a sem_post if it's possible
static void _check_prev_state (DLinkedList_t* pList)
{
	// not with the first node
	if (pList != g_pListHead && matrix_get_state (pList->prev->data) == MATRIX_FREE)
	{
		int err = sem_post (&g_aEmpty);
		IF_ERROR ("sem_post Empty")
	}
}

// read matrix and change the state
static matrix_t* _read_and_create_matrix (void)
{
	matrix_t* m = matrix_reader_next (g_pReader);

	if (m == NULL)
		return NULL;

	matrix_set_state (m, MATRIX_FREE);

	return m;
} 

// it just
static void _add_first_matrix_in_list (matrix_t* pMatrix)
{
	g_pListHead->data = pMatrix;
}


// at the end and check the state of the previous node
static void _add_matrix_in_list (matrix_t* pMatrix)
{
	// matrix_print (pMatrix);
	int err = pthread_mutex_lock (&g_aMutexProducer);
	IF_ERROR ("pthread_mutex_lock Producer")
	dllist_append (g_pListHead, pMatrix);
	_check_prev_state (g_pListHead->prev);
	err = pthread_mutex_unlock (&g_aMutexProducer);
	IF_ERROR ("pthread_mutex_unlock Producer")
}

// modify g_iProducerEnd to 1
static void _end_of_file_actions (void)
{
	int err;
	err = pthread_mutex_lock (&g_aMutexProducerEnd);
	IF_ERROR ("pthread_mutex_lock Producer End")
	g_iProducerEnd = 1;
	err = pthread_mutex_unlock (&g_aMutexProducerEnd);
	IF_ERROR ("pthread_mutex_unlock Producer End")
}

// read and create matrix + check if it's the last one.
static matrix_t * _read_create_and_check_matrix (void)
{
	matrix_t* pMatrix = _read_and_create_matrix ();
	if (! pMatrix)
	{	// the last one
		_end_of_file_actions ();
		int err;
		err = pthread_mutex_unlock (&g_aMutexFile);
		IF_ERROR ("pthread_mutex_unlock File")
		err = pthread_mutex_lock (&g_aMutexProducer);
		IF_ERROR ("pthread_mutex_lock Producer")
		if (g_pListHead == g_pListHead->prev)
		{
			err = sem_post (&g_aEmpty);
			IF_ERROR ("sem_post Empty")
		}
		err = pthread_mutex_unlock (&g_aMutexProducer);
		IF_ERROR ("pthread_mutex_unlock Producer")
	}
	return pMatrix;
}

void* producer (void* arg)
{
	int err;
	matrix_t* pMatrix;

	// special case for the first initialisation (g_pListHead->data is NULL)
	err = pthread_mutex_lock (&g_aMutexProducer);
	IF_ERROR ("pthread_mutex_lock Producer")
	if (g_pListHead->data == NULL) // (g_pListHead->prev == g_pListHead)
	{
		err = pthread_mutex_lock (&g_aMutexFile);
		IF_ERROR ("pthread_mutex_lock File")

		pMatrix = _read_create_and_check_matrix ();
		if (! pMatrix)
		{
			err = pthread_mutex_unlock (&g_aMutexProducer);
			IF_ERROR ("pthread_mutex_unlock Producer")
			pthread_exit (NULL);
		}
		
		// matrix_print (pMatrix);
		
		_add_first_matrix_in_list (pMatrix);
		err = pthread_mutex_unlock (&g_aMutexFile);
		IF_ERROR ("pthread_mutex_unlock File")
	}
	err = pthread_mutex_unlock (&g_aMutexProducer);
	IF_ERROR ("pthread_mutex_unlock Producer")

	while (1)
	{
		err = pthread_mutex_lock (&g_aMutexFile);
		IF_ERROR ("pthread_mutex_lock File")

		pMatrix = _read_create_and_check_matrix ();
		if (! pMatrix) // g_aMutexFile has been unlocked
			pthread_exit (NULL);

		_add_matrix_in_list (pMatrix);
		err = pthread_mutex_unlock (&g_aMutexFile);
		IF_ERROR ("pthread_mutex_unlock File")
	}
	pthread_exit (NULL);
}
