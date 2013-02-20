#!/bin/bash

# Générateur de matrices
# USAGE: gen.sh <NOMBRE_MATRICES> <MAX_LINE_DIM> <MAX_COL_DIM> 
# EXEMPLE: génère 10 matrices de taille maximale 5x5 et 
#          les place dans le fichier local test_matrix.mat:
#   $ ./gen.sh 10 5 5 > test_matrix.mat

N_MATRICES=${1:-10}
MAX_LINE=${2:-100}
MAX_COLS=${3:-100}
MAX_VAL=2 #${4:-2}

NLINES=-1
NCOLS=0

for ((n=0; n < ${N_MATRICES}; n++)); do
    if [ ${NLINES} = -1 ]; then 
        NLINES=`expr ${RANDOM} % ${MAX_LINE}`
        NLINES=`expr ${NLINES} + 1`
    else
        NLINES=${NCOLS}
    fi
    NCOLS=`expr ${RANDOM} % ${MAX_COLS}`
    NCOLS=`expr ${NCOLS} + 1`
    
    echo "${NLINES}x${NCOLS}"
    
    for ((i=0; i < ${NLINES}; i++)); do
        for ((j=0; j < ${NCOLS}; j++)); do
            if [ $j -ge 1 ]; then
                echo -n " "
            fi
            VALUE=`expr ${RANDOM} % ${MAX_VAL}`
            echo -n "${VALUE}"
        done
        echo
    done
done
