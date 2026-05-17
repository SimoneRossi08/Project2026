#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "books.h"
#include "users.h"
#include "lending.h"
#include "notifiche.h"

void caricaLibri(Catalogo* catalogo);
void salvaLibri(Catalogo* catalogo);

void caricaUtenti(Anagrafica* anagrafica);
void salvaUtenti(Anagrafica* anagrafica);

void caricaPrestiti(Catalogo* catalogo, Anagrafica* anagrafica, CodaNotifiche* coda);
void salvaPrestiti(Anagrafica* anagrafica);

void aggiungiStorico(Prestito* prestito, time_t dataRestituzione);

#endif