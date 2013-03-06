#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
* Usage ./exec nbreMatrices lignesMax colonnesMax Fichier
*
*Fichier ne doit pas encore exister
*/

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int nbreMat = atoi(argv[1]);
	int lignesMax = atoi(argv[2]);
	int colonnesMax = atoi(argv[3]);
	char *filename = argv[4];
	FILE *f = fopen(filename, "w");
	int lignes = rand()%lignesMax + 1, colonnes;
	int i;
	for(i=0;i<nbreMat;i++)
	{
		colonnes = rand()%colonnesMax+1;
		fprintf(f,"%dx%d\n",lignes,colonnes);
		int j;
		for(j=0;j<lignes;j++)
		{
		  int k;
			for(k=0;k<colonnes;k++)
			{
				if(k >= 1)
					fprintf(f," ");
				int val = (rand()%4 == 0) ? 1 : 0;
				fprintf(f,"%d",val);
			}
			fprintf(f,"\n");
		}
		lignes = colonnes;
	}

	fclose(f);
	return EXIT_SUCCESS;
}
