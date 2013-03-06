#ifndef __MATRIX_READER_H__
#define __MATRIX_READER_H__

/* 
 * Ce fichier défini un lecteur de matrices stockées sur le disque.
 * Les fichiers de matrice en entrée ont le format suivant: 
 * Une ligne spécifiant la taille de la matrice, par exemple 100x3, 
 * suivi de la matrice représentée sous forme de suite de nombre 
 * séparés par des espaces et des sauts de ligne pour délimiter les 
 * lignes de matrice. Plusieurs matrices peuvent bien entendu être 
 * définie en chainant les différentes définition de matrices.
 * Un example d'un tel fichier:
 *
 *   5x5
 *   1 1 1 1 0
 *   0 0 1 1 1
 *   0 0 0 1 1
 *   1 1 0 0 0
 *   1 1 1 0 0
 *   5x2
 *   1 1
 *   1 0
 *   0 0
 *   1 1
 *   1 0
 * 
 * 
 * matrix_reader_* vous permet de facilement itérer sur les matrices
 * stockées dans un fichier. Un exemple d'utilisation serait:
 *
 *   matrix_reader_t *reader = matrix_reader_init("/chemin/vers/matrices.mat");
 *   /!\ reader peut-être NULL, si c'est le cas une erreur s'est produite
 *
 *   matrix_t *m;
 *   while ( (m = matrix_reader_next(reader)) != NULL) {
 *      // m contient la dernière matrice lue dans le fichier.
 *   }
 *   matrix_reader_free(reader);
 */

#include "matrix.h"

struct matrix_reader;
typedef struct matrix_reader matrix_reader_t;

matrix_reader_t *
matrix_reader_init(const char *filename);

matrix_t *
matrix_reader_next(matrix_reader_t *r);

void
matrix_reader_free(matrix_reader_t *r);

#endif
