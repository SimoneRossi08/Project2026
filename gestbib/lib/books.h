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
    int volte_prestato;
} Libro;

typedef struct Catalogo {
    Libro** libri;
    int size;
    int capacity;
    int next_id;
} Catalogo;

struct Anagrafica;

void inizializzaCatalogo(Catalogo* catalogo);
void liberaCatalogo(Catalogo* catalogo);
void aggiungiLibro(Catalogo* catalogo);
void modificaLibro(Catalogo* catalogo);
void eliminaLibro(Catalogo* catalogo, struct Anagrafica* anagrafica);
void cercaLibri(Catalogo* catalogo);
Libro* trovaLibroPerId(Catalogo* catalogo, int id);
void inserisciLibro(Catalogo* catalogo, Libro* libro);

#endif