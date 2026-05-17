#ifndef LENDING_H
#define LENDING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "books.h"
#include "users.h"

typedef struct Prestito{
    Libro* libro;
    Utente* utente;
    time_t dataPrestito;
    time_t dataScadenza;
} Prestito;

void creaPrestito(Catalogo* catalogo, Anagrafica* anagrafica);
void restituisciPrestito(Catalogo* catalogo, Anagrafica* anagrafica);
void mostraScaduti(Anagrafica* anagrafica);

#endif