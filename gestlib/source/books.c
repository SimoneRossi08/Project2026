#include <stdio.h>
#include <stdlib.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

#define MAX_LIBRI 100
#define MAX_UTENTI 100
#define MAX_PRESTITI 100

void aggiungiLibro(Libro* libri, int *libri_size){
    if(*libri_size == MAX_LIBRI){
        printf("Errore: numero massimo di libri raggiunto.\n");
        return;
    }

    Libro new_libro;
    printf("Inserisci ID del libro: ");
    scanf("%d", &new_libro.id);
    printf("Inserisci titolo del libro: ");
    scanf("%s", new_libro.titolo);
    printf("Inserisci autore del libro: ");
    scanf("%s", new_libro.autore);
    printf("Inserisci genere del libro: ");
    scanf("%s", new_libro.genere);
    printf("Inserisci numero di copie: ");
    scanf("%d", &new_libro.copie);

    libri[*libri_size] = new_libro;
    (*libri_size)++;
}

void modificaLibro(Libro* libri, int *libri_size){
    if (*libri_size == 0) {
        printf("Errore: nessun libro da modificare.\n");
        return;
    }

    int id;
    printf("Inserisci ID del libro da modificare: ");
    scanf("%d", &id);

    Libro* trovato = cercaLibro(id, libri, *libri_size);
    if (trovato) {
        printf("Inserisci nuovo titolo: ");
        scanf("%s", trovato->titolo);
        printf("Inserisci nuovo autore: ");
        scanf("%s", trovato->autore);
        printf("Inserisci nuovo genere: ");
        scanf("%s", trovato->genere);
        printf("Inserisci nuove copie: ");
        scanf("%d", &trovato->copie);
    } else {
        printf("Libro non trovato.\n");
    }
}

void eliminaLibro(Libro* libri, int *libri_size){
    if (*libri_size == 0) {
        printf("Errore: nessun libro da eliminare.\n");
        return;
    }

    int id,i;
    printf("Inserisci ID del libro da eliminare: ");
    scanf("%d", &id);

    Libro* trovato = cercaLibro(id, libri, *libri_size);
    if (trovato) {
        // Elimina il libro dalla posizione corrispondente
        for (i = 0; i < (*libri_size) - 1 && libri[i].id != id; i++);
        if (i == (*libri_size) - 1) {
            printf("Libro non trovato.\n");
        } else {
            *libri_size = *libri_size - 1;
            for (int j = i; j < *libri_size; j++) {
                libri[j] = libri[j + 1];
            }
        }
    } else{
        printf("Libro non trovato.\n");
    }
}

Libro* cercaLibro(int id, Libro* libri, int libri_size){
    for (int i = 0; i < libri_size; i++){
        if (libri[i].id == id) {
            return &libri[i];
        }
    }
    return NULL;
}