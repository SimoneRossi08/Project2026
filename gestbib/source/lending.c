#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/lending.h"
#include "../lib/InputOutput.h"

#define DURATA_PRESTITO_GIORNI 30

void creaPrestito(Catalogo* catalogo, Anagrafica* anagrafica) {
    int idLibro, idUtente;
    printf("ID libro: ");
    scanf("%d", &idLibro);
    printf("ID utente: ");
    scanf("%d", &idUtente);

    Libro* libro = trovaLibroPerId(catalogo, idLibro);
    Utente* utente = trovaUtentePerId(anagrafica, idUtente);

    if (libro == NULL || utente == NULL) {
        printf("Libro o utente non trovati.\n");
        return;
    }
    if (libro->copie <= 0) {
        printf("Errore: nessuna copia disponibile.\n");
        return;
    }

    Prestito* prestito = (Prestito*)malloc(sizeof(Prestito));
    prestito->libro = libro;
    prestito->utente = utente;
    prestito->dataPrestito = time(NULL);
    prestito->dataScadenza = prestito->dataPrestito + DURATA_PRESTITO_GIORNI * 24 * 60 * 60;

    libro->copie--;
    libro->volte_prestato++;
    utente->storico_count++;
    inserisciPrestito(utente, prestito);

    salvaLibri(catalogo);
    salvaUtenti(anagrafica);
    salvaPrestiti(anagrafica);

    char data[64];
    struct tm* tm_info = localtime(&prestito->dataScadenza);
    strftime(data, sizeof(data), "%Y-%m-%d", tm_info);
    printf("Prestito creato. Scadenza: %s\n", data);
}

void restituisciPrestito(Catalogo* catalogo, Anagrafica* anagrafica) {
    int idLibro, idUtente;
    printf("ID libro: ");
    scanf("%d", &idLibro);
    printf("ID utente: ");
    scanf("%d", &idUtente);

    Utente* utente = trovaUtentePerId(anagrafica, idUtente);
    if (utente == NULL) {
        printf("Utente non trovato.\n");
        return;
    }

    Prestito* prestito = NULL;
    NodoPrestito* current = utente->prestiti;
    while (current != NULL) {
        if (current->prestito->libro->id == idLibro) {
            prestito = current->prestito;
            break;
        }
        current = current->next;
    }
    if (prestito == NULL) {
        printf("Prestito non trovato per quell'utente.\n");
        return;
    }

    time_t adesso = time(NULL);
    aggiungiStorico(prestito, adesso);

    prestito->libro->copie++;
    rimuoviPrestito(utente, prestito);
    free(prestito);

    salvaLibri(catalogo);
    salvaPrestiti(anagrafica);
    printf("Prestito restituito.\n");
}

void mostraScaduti(Anagrafica* anagrafica) {
    time_t adesso = time(NULL);
    int trovati = 0;

    for (int i = 0; i < anagrafica->size; i++) {
        Utente* utente = anagrafica->utenti[i];
        NodoPrestito* current = utente->prestiti;
        while (current != NULL) {
            if (current->prestito->dataScadenza < adesso) {
                char data[64];
                struct tm* tm_info = localtime(&current->prestito->dataScadenza);
                strftime(data, sizeof(data), "%Y-%m-%d", tm_info);
                printf("SCADUTO: utente %s - libro \"%s\" (scadenza: %s)\n",
                       utente->nome, current->prestito->libro->titolo, data);
                trovati++;
            }
            current = current->next;
        }
    }

    if (trovati == 0) {
        printf("Nessun prestito scaduto.\n");
    }
}