#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/lending.h"

#define MAX_PRESTITI 100
#define DURATA_PRESTITO (30 * 24 * 60 * 60)

void creaPrestito(Prestito* prestiti, int *prestiti_size, Libro* libro, Utente* utente){
    if (*prestiti_size == MAX_PRESTITI){
        printf("Errore: numero massimo di prestiti raggiunto.\n");
        return;
    }
    if (libro->copie <= 0){
        printf("Errore: nessuna copia disponibile.\n");
        return;
    }

    Prestito p;
    p.libro = libro;
    p.utente = utente;
    p.dataPrestito = time(NULL);
    p.dataScadenza = p.dataPrestito + DURATA_PRESTITO;
    strcpy(p.stato, "attivo");

    libro->copie--;
    prestiti[*prestiti_size] = p;
    (*prestiti_size)++;
    printf("Prestito creato.\n");
}

void restituisciPrestito(Prestito* prestiti, int *prestiti_size){
    if (*prestiti_size == 0){
        printf("Errore: nessun prestito da restituire.\n");
        return;
    }

    int idLibro, idUtente;
    printf("Inserisci ID del libro: ");
    scanf("%d", &idLibro);
    printf("Inserisci ID dell'utente: ");
    scanf("%d", &idUtente);

    int found = -1;
    for (int i = 0; i < *prestiti_size; i++){
        if (prestiti[i].libro->id == idLibro && prestiti[i].utente->id == idUtente){
            found = i;
            break;
        }
    }

    if (found == -1){
        printf("Prestito non trovato.\n");
        return;
    }

    prestiti[found].libro->copie++;
    for (int j = found; j < *prestiti_size - 1; j++){
        prestiti[j] = prestiti[j + 1];
    }
    (*prestiti_size)--;
    printf("Prestito restituito.\n");
}

void controllaScadenze(Prestito* prestiti, int prestiti_size){
    time_t adesso = time(NULL);
    int scaduti = 0;
    for (int i = 0; i < prestiti_size; i++){
        if (prestiti[i].dataScadenza < adesso){
            strcpy(prestiti[i].stato, "scaduto");
            printf("Prestito scaduto: libro \"%s\", utente \"%s\"\n",
                   prestiti[i].libro->titolo, prestiti[i].utente->nome);
            scaduti++;
        }
    }
    if (scaduti == 0){
        printf("Nessun prestito scaduto.\n");
    }
}