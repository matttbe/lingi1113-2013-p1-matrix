#include <pthread.h>
// matrix
#include "matrix_reader.h"
// Double linked lists
#include "DLinkedList.h"
//semaphores
#include <semaphore.h>
// a few useful define for errors
#include "misc.h"

extern DLinkedList_t* g_pListHead;
extern sem_t g_aEmpty;
extern pthread_mutex_t g_aMutexProducer;
extern pthread_mutex_t g_aMutexProducerEnd;
extern pthread_mutex_t g_aMutexConsumer;
extern pthread_mutex_t g_aMutexConsumerCritical;
extern pthread_mutex_t g_aMutexConsumerEnd;
extern pthread_mutex_t g_aMutexFile;
extern int g_iProducerEnd;
extern int g_iConsumerEnd;

static int _producer_has_finished (void)
{
	int err, iTmp;
	err = pthread_mutex_lock (&g_aMutexProducerEnd);
	IF_ERROR ("pthread_mutex_lock Producer End")
	iTmp = g_iProducerEnd;
	err = pthread_mutex_unlock (&g_aMutexProducerEnd);
	IF_ERROR ("pthread_mutex_unlock Producer End")
	return iTmp;
}

static int _consumer_has_finished (void)
{
	int err, iTmp;
	err = pthread_mutex_lock (&g_aMutexConsumerEnd);
	IF_ERROR ("pthread_mutex_lock Consumer End")
	iTmp = g_iConsumerEnd;
	err = pthread_mutex_unlock (&g_aMutexConsumerEnd);
	IF_ERROR ("pthread_mutex_unlock Consumer End")
	return iTmp;
}

// for each consumer
void *consumer (void *arg)
{
	int err, iTmp = 0, iCheck = 0;
	while (1)
	{
		if (_consumer_has_finished () == 1)
			pthread_exit (NULL);

		//_________ Search matrix
		/////////////// LOCK T5
		err = pthread_mutex_lock (&g_aMutexConsumerCritical); // this mutex protects the first part of the job of consumer (read data)
		IF_ERROR ("pthread_mutex_lock consummer critical")

		/////////////// LOCK T3
		err = sem_wait (&g_aEmpty); // a nodes' pair is available 
		IF_ERROR ("sem_wait empty")
		
		err = pthread_mutex_lock (&g_aMutexConsumer); // this mutex protects consumers (we can modify and read the list at the same time)
		IF_ERROR ("pthread_mutex_lock consummer")

		if (_consumer_has_finished () == 1)
		{
			err = sem_post (&g_aEmpty); // it's the end, don't lock the producers, just leave.
			IF_ERROR ("sem_post Empty")
			err = pthread_mutex_unlock (&g_aMutexConsumer);
			IF_ERROR ("pthread_mutex_unlock consumer")
			err = pthread_mutex_unlock (&g_aMutexConsumerCritical);
			IF_ERROR ("pthread_mutex_unlock consumer Critical")
			pthread_exit (NULL);
		}

		err = pthread_mutex_lock (&g_aMutexProducer); // let's start reading the list if there is no producer
		IF_ERROR ("pthread_mutex_lock producer")

		DLinkedList_t* pList;
		DLinkedList_t* pListNext;
		matrix_t* pMatrixResult;
		matrix_t* pMatrix = NULL;
		matrix_t* pMatrixPrev;
		matrix_t* pMatrixNext = NULL;
		// it's our double linked list
		for (pList = g_pListHead ; pList != NULL ; pList = pList->next)
		{
			pListNext = pList->next;
			pMatrix = pList->data;
			pMatrixNext = pListNext->data;
			if (matrix_get_state (pMatrix) == MATRIX_FREE && matrix_get_state (pMatrixNext) == MATRIX_FREE)
			{
				// we found a pair
				matrix_set_state (pMatrix, MATRIX_BUSY);
				matrix_set_state (pMatrixNext, MATRIX_BUSY); // we reserve these matrix

				pMatrixPrev = (pList == g_pListHead) ? NULL : pList->prev->data; // on our list, the first node has a pointer (prev) to the last node

				if (pMatrixPrev && matrix_get_state (pMatrixPrev) == MATRIX_FREE)
				{
					err = sem_wait (&g_aEmpty); // should not wait... just to decrease the counter
					IF_ERROR ("sem_post empty")
				}

				// we have to continue if it's possible
				if (sem_trywait (&g_aEmpty) != 0) // && pListNext->next)
					iCheck = 1;

				break; // we have pList
			}
		}

		// product has finished and we have 2 elem => it's time to say it's the end!
		if (_producer_has_finished () == 1 && g_pListHead->next == g_pListHead->prev)
		{
			iTmp = 1;
			err = pthread_mutex_lock (&g_aMutexConsumerEnd);
			IF_ERROR ("pthread_mutex_lock Consumer End")
			g_iConsumerEnd = 1;
			err = pthread_mutex_unlock (&g_aMutexConsumerEnd);
			IF_ERROR ("pthread_mutex_unlock Consumer End")
			err = sem_post (&g_aEmpty);
			IF_ERROR ("sem_post Empty")
			err = sem_post (&g_aEmpty);
			IF_ERROR ("sem_post Empty") // two free port for the end
		}

		if (iCheck != 1)
		{
			err = pthread_mutex_unlock (&g_aMutexConsumerCritical);
			IF_ERROR ("pthread_mutex_unlock consumerCritical")
		}

		err = pthread_mutex_unlock (&g_aMutexProducer);
		IF_ERROR ("pthread_mutex_unlock producer")
		err = pthread_mutex_unlock (&g_aMutexConsumer);
		IF_ERROR ("pthread_mutex_unlock consumer")

		// ________ Matricial product
		pMatrixResult = matrix_product (pMatrix, pMatrixNext);

		// ________ Release
		err = pthread_mutex_lock (&g_aMutexConsumer);
		IF_ERROR ("pthread_mutex_lock consummer")
		err = pthread_mutex_lock (&g_aMutexProducer);
		IF_ERROR ("pthread_mutex_lock producer")

		matrix_set_state (pMatrixResult, MATRIX_FREE);
		matrix_free (pMatrix);
		matrix_free (pMatrixNext);
		dllist_replace_two_nodes_by_one_with_data (pList, g_pListHead, pMatrixResult);

		// we check the state of the next matrix
		if (pList->next && matrix_get_state (pList->next->data) == MATRIX_FREE)
		{
			err = sem_post (&g_aEmpty);
			IF_ERROR ("sem_post Empty")
		}

		// ... and the previous one (but not the first one-
		if (pList != g_pListHead && matrix_get_state (pList->prev->data) == MATRIX_FREE)
		{
			err = sem_post (&g_aEmpty);
			IF_ERROR ("sem_post Empty")
		}

		err = pthread_mutex_unlock (&g_aMutexProducer);
		IF_ERROR ("pthread_mutex_unlock producer")
		err = pthread_mutex_unlock (&g_aMutexConsumer);
		IF_ERROR ("pthread_mutex_unlock consumer")

		if (iCheck == 1)
		{	// if we are on a critital moment
			err = pthread_mutex_unlock (&g_aMutexConsumerCritical);
			IF_ERROR ("pthread_mutex_unlock consumerCritical")
			iCheck = 0;
		}

		if (iTmp == 1)
			pthread_exit (NULL);
	}
}
