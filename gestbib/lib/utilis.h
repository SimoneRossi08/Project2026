#ifndef UTILIS_H
#define UTILIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "books.h"

// Ordina i libri per titolo con bubble sort
void bubbleSort(Libro* libri, int size);

// Stampa statistiche (totale copie disponibili)
void statistiche(Libro* libri, int *libri_size);

#endif