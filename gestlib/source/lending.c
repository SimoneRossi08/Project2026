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

void creaPrestito(Prestito** prestiti, int *prestiti_size, Libro* libro, Utente* utente) {
    if (*prestiti_size == MAX_PRESTITI) {
        printf("Errore: numero massimo di prestiti raggiunto.\n");
        return;
    }

    Prestito new_prestito;
    new_prestito.libro = libro;
    new_prestito.utente = utente;
    new_prestito.dataPrestito = time(NULL);
    new_prestito.dataScadenza = time(NULL) + (24 * 60 * 60); // Simula una scadenza di un giorno
    new_prestito.stato[0] = '\0';

    strcpy(new_prestito.stato, "attivo");

    prestiti[*prestiti_size] = new_prestito;
    (*prestiti_size)++;
}

void restituisciPrestito(Prestito** prestiti, int *prestiti_size) {
    if (*prestiti_size == 0) {
        printf("Errore: nessun prestito da restituire.\n");
        return;
    }

    int idLibro;
    printf("Inserisci ID del libro da restituire: ");
    scanf("%d", &idLibro);

    Prestito* trovato = NULL;
    for (int i = 0; i < *prestiti_size && trovato == NULL; i++) {
        if(prestiti[i].libro->id == idLibro) {
            trovato = &prestiti[i];
        }
    }

    if (trovato) {
        printf("Libro restituito: %s\n", trovato->libro->titolo);
        strcpy(trovato->stato, "concluso");
    } else {
        printf("Prestito non trovato o libro non disponibile.\n");
    }
}

void controllaScadenze(Prestito** prestiti, int *prestiti_size){
    for(int i = 0; i < *prestiti_size; i++){
        if (time(NULL)>prestiti[i].dataScadenza && strcmp(prestiti[i].stato, "concluso") == 0){
            printf("Prestito scaduto: %s\n", prestiti[i].libro->titolo);
            strcpy(prestiti[i].stato, "scaduto");
        }
    }
}