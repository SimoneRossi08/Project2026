#include <stdio.h>
#include <stdlib.h>

typedef struct Libro {
    int id;
    char *titolo;
    char *autore;
    char *genere;
    int copie;
} Libro;

typedef struct Utente {
    int id;
    char *nome;
    // Lista collegata prestiti
} Utente;

typedef struct Prestito {
    Libro* libro;
    Utente* utente;
    time_t dataPrestito;
    time_t dataScadenza;
    char stato[20];
} Prestito;

void creaPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro, Utente* utente);
void restituisciPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro);
void controllaScadenze(Prestito** prestiti, int *prestiti_size);

// Crea un nuovo prestito
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void creaPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro, Utente* utente) {
    // Implementazione da completare
}

// Restituisce un prestito
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void restituisciPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro) {
    // Implementazione da completare
}

// Controlla scadenze dei prestiti e imposta lo stato in caso necessario
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void controllaScadenze(Prestito** prestiti, int *prestiti_size) {
    // Implementazione da completare
}