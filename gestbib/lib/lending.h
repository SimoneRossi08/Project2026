#ifndef LENDING_H
#define LENDING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "books.h"
#include "users.h"

typedef struct Prestito {
    Libro* libro;
    Utente* utente;
    time_t dataPrestito;
    time_t dataScadenza;
    char stato[20];
} Prestito;

// Crea un nuovo prestito
void creaPrestito(Prestito* prestiti, int *prestiti_size, Libro* libro, Utente* utente);

// Restituisce un prestito (chiede ID libro e utente)
void restituisciPrestito(Prestito* prestiti, int *prestiti_size);

// Controlla scadenze e marca come "scaduto" quelli oltre la data
void controllaScadenze(Prestito* prestiti, int prestiti_size);

#endif