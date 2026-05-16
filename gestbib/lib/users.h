#ifndef USERS_H
#define USERS_H

#include <stdio.h>
#include <stdlib.h>

struct Prestito;

typedef struct NodoPrestito{
    struct Prestito* prestito;
    struct NodoPrestito* next;
}NodoPrestito;

typedef struct Utente{
    int id;
    char nome[100];
    NodoPrestito* prestiti;
    int storico_count;
}Utente;

typedef struct Anagrafica{
    Utente** utenti;
    int size;
    int capacity;
    int next_id;
}Anagrafica;

void inizializzaAnagrafica(Anagrafica* anagrafica);
void liberaAnagrafica(Anagrafica* anagrafica);
void registraUtente(Anagrafica* anagrafica);
void eliminaUtente(Anagrafica* anagrafica);
void stampaUtenti(Anagrafica* anagrafica);
void mostraStorico(Anagrafica* anagrafica);
Utente* trovaUtentePerId(Anagrafica* anagrafica, int id);
void inserisciUtente(Anagrafica* anagrafica, Utente* utente);
void inserisciPrestito(Utente* utente, struct Prestito* prestito);
void rimuoviPrestito(Utente* utente, struct Prestito* prestito);

#endif