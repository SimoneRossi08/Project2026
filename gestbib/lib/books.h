#ifndef BOOKS_H
#define BOOKS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Libro {
    int id;
    char titolo[100];
    char autore[100];
    char genere[100];
    int copie;
} Libro;

// Aggiunge un libro alla lista
void aggiungiLibro(Libro* libri, int *libri_size);

// Modifica un libro esistente
void modificaLibro(Libro* libri, int *libri_size);

// Elimina un libro dalla lista
void eliminaLibro(Libro* libri, int *libri_size);

// Cerca un libro per ID, ritorna NULL se non trovato
Libro* cercaLibro(int id, Libro* libri, int libri_size);

#endif