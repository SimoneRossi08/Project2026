#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char *nome;
    // Lista collegata prestiti da completare

} Utente;

void registraUtente(Utente* utenti, int *utenti_size);
void eliminaUtente(Utente* utenti, int *utenti_size);
void stampaUtenti(Utente* utenti, int utenti_size);

// Registra un nuovo utente e aggiunge alla lista
// Utenti è il puntatore al vettore di utenti
// utenti_size è il puntatore al numero di elementi attuali nel vettore

void registraUtente(Utente* utenti, int *utenti_size) {
    // Implementazione da completare
}

// Elimina un utente dalla lista e rimuove prestiti associati
// Utenti è il puntatore al vettore di utenti
// utenti_size è il puntatore al numero di elementi attuali nel vettore

void eliminaUtente(Utente* utenti, int *utenti_size) {
    // Implementazione da completare
}

// Stampa tutti gli utenti
// Utenti è il puntatore al vettore di utenti
// utenti_size è il numero di elementi attuali nel vettore

void stampaUtenti(Utente* utenti, int utenti_size) {
    // Implementazione da completare
}