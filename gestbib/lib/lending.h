#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int id;
    char *titolo;
    char *autore;
    char *genere;
    int copie;
}Libro;

typedef struct{
    int id;
    char *nome;
    // Lista collegata prestiti
}Utente;

typedef struct{
    Libro* libro;
    Utente* utente;
    int dataPrestito;
    int dataScadenza;
    char stato[20];
}Prestito;

void creaPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro, Utente* utente);
void restituisciPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro);
void controllaScadenze(Prestito** prestiti, int *prestiti_size);

// Crea un nuovo prestito
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void creaPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro, Utente* utente);

// Restituisce un prestito
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void restituisciPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro);

// Controlla scadenze dei prestiti e imposta lo stato in caso necessario
// Prestiti è il puntatore al vettore di prestiti
// prestiti_size è il puntatore al numero di elementi attuali nel vettore

void controllaScadenze(Prestito** prestiti, int *prestiti_size);