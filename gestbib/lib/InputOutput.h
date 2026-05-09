#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include "books.h"
#include "users.h"
#include "lending.h"

void caricaLibriCSV(Libro* libri, int *libri_size);
void caricaUtentiCSV(Utente* utenti, int *utenti_size);
void caricaPrestitiCSV(Prestito* prestiti, int *prestiti_size,
                       Libro* libri, int libri_size,
                       Utente* utenti, int utenti_size);

void salvaLibriCSV(Libro* libri, int libri_size);
void salvaUtentiCSV(Utente* utenti, int utenti_size);
void salvaPrestitiCSV(Prestito* prestiti, int prestiti_size);

#endif