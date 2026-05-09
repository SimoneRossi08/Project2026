#ifndef USERS_H
#define USERS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Utente {
    int id;
    char nome[100];
} Utente;

// Registra un nuovo utente
void registraUtente(Utente* utenti, int *utenti_size);

// Elimina un utente dalla lista
void eliminaUtente(Utente* utenti, int *utenti_size);

// Stampa tutti gli utenti
void stampaUtenti(Utente* utenti, int utenti_size);

// Cerca un utente per ID, ritorna NULL se non trovato
Utente* cercaUtente(int id, Utente* utenti, int utenti_size);

#endif