#include <stdlib.h>

#include <pthread.h>
// getopt
#include <getopt.h>
// matrix
#include "matrix_reader.h"
// Double linked lists
#include "DLinkedList.h"
//semaphores
#include <semaphore.h>
// a few useful define for errors
#include "misc.h"
#include "producer.h"
#include "consumer.h"

#include "compute_no_thread.h"

DLinkedList_t* g_pListHead;
sem_t g_aEmpty;
//sem_t g_aFull;
pthread_mutex_t g_aMutexConsumer;
pthread_mutex_t g_aMutexConsumerCritical;
pthread_mutex_t g_aMutexConsumerEnd;
pthread_mutex_t g_aMutexProducer;
pthread_mutex_t g_aMutexProducerEnd;
pthread_mutex_t g_aMutexFile;
matrix_reader_t* g_pReader;
int g_iProducerEnd;
int g_iConsumerEnd;

int main (int argc, char *argv[])
{	
	//____________ GetOpt
	int iThread = 1;
	int opt;
	int err = 0;
	const char* cFilename;
	const char* cOutputFileName = NULL;
	matrix_t *pMatrix;
	FILE *pOutputFile;


	int i = 0;
	while ((opt = getopt (argc, argv, "hn:o:")) != -1)
	{
		switch (opt)
		{
			case 'h':
				printf ("You can specify the number of threads (>1) with '-n' option\n");
				exit (EXIT_SUCCESS);
			case 'n':
				iThread = atoi (optarg);
				if (iThread < 1)
				{
					fprintf (stderr, "Error! you should have at least 1 thread -> Threads = 1\n");
					iThread = 1;
				}
				i++;
				break;
			case 'o':
				cOutputFileName = optarg;
				break;
			case '?':
				fprintf (stderr, "Error! No such option: '%c'\n", optopt);
				exit (EXIT_FAILURE);
		}
		i++;
	}

	// with file?
	if (argc-1-i == 0)
	{
		fprintf (stderr, "Error! No file specified\n");
		exit (EXIT_FAILURE);
	}
	cFilename = argv [argc-1];

	/*printf ("nom du fichier : [%s]\n", cFilename);
	printf ("number of threads : [%d]\n", iThread);*/

	g_pReader = matrix_reader_init (cFilename);
	if (! g_pReader) // wrong file
		exit (EXIT_FAILURE);

	// open the ouput file if needed
	if (cOutputFileName != NULL)
	{
		pOutputFile = fopen (cOutputFileName, "w");
		if (pOutputFile == NULL)
		{
			fprintf (stderr, "Not able to use this output file: %s\n", cOutputFileName);
			cOutputFileName = NULL; // to print to stdout
		}
	}

	if (iThread > 1)
	{
		g_pListHead = dllist_init ();
		if (! g_pListHead)
		{
			fprintf (stderr, "Error! Can't initialised the list\n");
			exit (EXIT_FAILURE);
		}

		err = pthread_mutex_init (&g_aMutexConsumer, NULL);
		IF_ERROR ("Mutex init Consumer")
		err = pthread_mutex_init (&g_aMutexConsumerEnd, NULL);
		IF_ERROR ("Mutex init Consumer End")
		g_iConsumerEnd = 0;
		err = pthread_mutex_init (&g_aMutexConsumerCritical, NULL);
		IF_ERROR ("Mutex init Consumer Critical")
		err = pthread_mutex_init (&g_aMutexProducer, NULL);
		IF_ERROR ("Mutex init Producer")
		err = pthread_mutex_init (&g_aMutexProducerEnd, NULL);
		IF_ERROR ("Mutex init Producer End")
		g_iProducerEnd = 0;
		err = pthread_mutex_init (&g_aMutexFile, NULL);
		IF_ERROR ("Mutex init File")
		err = sem_init (&g_aEmpty, 0, 0);
		IF_ERROR ("Semaphore Init Empty")
		//err = sem_init (&g_aFull, 0, 2*iThread);
		/*IF_ERROR ("Semaphore Init FULL")*/

		pthread_t aThreadsProducers [iThread];
		pthread_t aThreadsConsumers [iThread];

		for (int i = 0; i < iThread; i++)
		{	
			err = pthread_create (&aThreadsProducers [i], NULL, &producer, NULL);
			IF_ERROR ("pThread create")
				
			err = pthread_create (&aThreadsConsumers [i], NULL, &consumer, NULL);
			IF_ERROR ("pThread create")
		}

		for (int i = 0; i < iThread; i++)
		{
			err = pthread_join (aThreadsProducers [i], NULL);
			IF_ERROR ("pThread join")
			
			err = pthread_join (aThreadsConsumers [i], NULL);
			IF_ERROR ("pThread join")
		}
		pMatrix = g_pListHead->data;
	}
	else
	{
		pMatrix = compute_all_matrix_in_one_thread (g_pReader);
	}

	if (cOutputFileName == NULL)
		matrix_print (pMatrix); // to stdout if no valid output file set
	else
		matrix_print_to_stream (pOutputFile, pMatrix);

	// it's time to free
	matrix_free (pMatrix);
	matrix_reader_free (g_pReader);

	if (iThread > 1)
	{
		dllist_free (g_pListHead);

		err = sem_destroy (&g_aEmpty);
		IF_ERROR ("sem_destroy Empty")
		err = pthread_mutex_destroy (&g_aMutexConsumer);
		IF_ERROR ("mutex destroy Consumers")
		err = pthread_mutex_destroy (&g_aMutexConsumerEnd);
		IF_ERROR ("mutex destroy Consumers End")
		err = pthread_mutex_destroy (&g_aMutexConsumerCritical);
		IF_ERROR ("mutex destroy Consumers Critical")
		err = pthread_mutex_destroy (&g_aMutexProducer);
		IF_ERROR ("mutex destroy Producers")
		err = pthread_mutex_destroy (&g_aMutexProducerEnd);
		IF_ERROR ("mutex destroy Producers End")
		err = pthread_mutex_destroy (&g_aMutexFile);
		IF_ERROR ("mutex destroy File")
	}
}
