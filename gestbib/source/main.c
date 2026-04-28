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

int main() {
    Libro libri[MAX_LIBRI];
    int libr_size = 0;

    Utente utenti[MAX_UTENTI];
    int utenti_size = 0;

    Prestito prestiti[MAX_PRESTITI];
    int prestiti_size = 0;

    // Carica i dati dal CSV
    caricaLibriCSV(libri, &libr_size);
    caricaUtentiCSV(utenti, &utenti_size);
    caricaPrestitiCSV(&prestiti, &prestiti_size);

    int opt;
    do {
        printf("Menu:\n");
        printf("(1) Aggiungi Libro\n");
        printf("(2) Modifica Libro\n");
        printf("(3) Elimina Libro\n");
        printf("(4) Cerca Libro\n");
        printf("(5) Registra Utente\n");
        printf("(6) Elimina Utente\n");
        printf("(7) Stampa Utenti\n");
        printf("(8) Crea Prestito\n");
        printf("(9) Restituisci Prestito\n");
        printf("(10) Controlla Scadenze\n");
        printf("(11) Statistiche Libri\n");
        printf("(12) Esci\n");
        scanf("%d", &opt);

        switch (opt) {
            case 1:
                aggiungiLibro(libri, &libr_size);
                break;
            case 2:
                modificaLibro(libri, &libr_size);
                break;
            case 3:
                eliminaLibro(libri, &libr_size);
                break;
            case 4:
                int id;
                printf("Inserisci ID del libro: ");
                scanf("%d", &id);
                Libro* trovatoLibro = cercaLibro(id, libri, libr_size);
                if (trovatoLibro){
                    printf("Trovato il libro:\n");
                    // Stampa i dettagli del libro
                } else{
                    printf("Libro non trovato.\n");
                }
                break;
            case 5:
                registraUtente(utenti, &utenti_size);
                break;
            case 6:
                eliminaUtente(utenti, &utenti_size);
                break;
            case 7:
                stampaUtenti(utenti, utenti_size);
                break;
            case 8:
                {
                    int idLibro, idUtente;
                    printf("Inserisci ID del libro: ");
                    scanf("%d", &idLibro);
                    printf("Inserisci ID dell'utente: ");
                    scanf("%d", &idUtente);

                    Libro* trovatoLibro = cercaLibro(idLibro, libri, libr_size);
                    Utente* trovatoUtente = NULL;
                    for (int i = 0; i < utenti_size && trovatoUtente == NULL; i++) {
                        if (utenti[i].id == idUtente) {
                            trovatoUtente = &utenti[i];
                        }
                    }

                    if (trovatoLibro && trovatoUtente) {
                        creaPrestito(&prestiti, &prestiti_size, trovatoLibro, trovatoUtente);
                        printf("Prestito creato.\n");
                    } else {
                        printf("Libro o utente non trovati.\n");
                    }
                }
                break;
            case 9:
                restituisciPrestito(&prestiti, &prestiti_size);
                break;
            case 10:
                controllaScadenze(&prestiti, &prestiti_size);
                break;
            case 11:
                statistiche(libri, &libr_size);
                break;
            case 12:
                printf("Esci.\n");
                break;
            default:
                printf("Scelta non valida. Riprova.\n");
        }
    }while (opt != 12);

    // Salva i dati nel CSV
    caricaLibriCSV(libri, &libr_size);
    caricaUtentiCSV(utenti, &utenti_size);
    caricaPrestitiCSV(&prestiti, &prestiti_size);

    return 0;
}