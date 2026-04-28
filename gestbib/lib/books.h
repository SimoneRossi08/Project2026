#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char *titolo;
    char *autore;
    char *genere;
    int copie;
}Libro;

void aggiungiLibro(Libro* libri, int *libri_size);
void modificaLibro(Libro* libri, int *libri_size);
void eliminaLibro(Libro* libri, int *libri_size);
Libro* cercaLibro(int id, Libro* libri, int libri_size);

// Aggiunge un libro alla lista
// Libri è il puntatore al vettore di libri
// libr_size è il puntatore al numero di elementi attuali nel vettore

void aggiungiLibro(Libro* libri, int *libri_size);

// Modifica un libro esistente
// Libri è il puntatore al vettore di libri
// libr_size è il puntatore al numero di elementi attuali nel vettore

void modificaLibro(Libro* libri, int *libri_size);

// Elimina un libro dalla lista
// Libri è il puntatore al vettore di libri
// libr_size è il puntatore al numero di elementi attuali nel vettore

void eliminaLibro(Libro* libri, int *libri_size);

// Cerca un libro nella lista per ID
// Libri è il puntatore al vettore di libri
// libr_size è il numero di elementi attuali nel vettore
// Ritorna NULL se non trovato, altrimenti il puntatore al libro

Libro* cercaLibro(int id, Libro* libri, int libri_size);