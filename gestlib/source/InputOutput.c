#include <stdio.h>
#include <stdlib.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

void caricaLibriCSV(Libro* libri, int *libri_size) {
    FILE* fp = fopen("data/libri.csv", "r");
    if (fp == NULL) {
        printf("Errore: non riesco a aprire il file 'libri.csv'.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char titolo[100], autore[100], genere[100];
        int copie;

        sscanf(line, "%d,%[^,\n],%[^,\n],%[^,\n],%d", &id, titolo, autore, genere, &copie);

        Libro new_libro = {id, strdup(titolo), strdup(autore), strdup(genere), copie};
        libri[*libri_size] = new_libro;
        (*libri_size)++;
    }

    fclose(fp);
}

void caricaUtentiCSV(Utente* utenti, int *utenti_size) {
    FILE* fp = fopen("data/utenti.csv", "r");
    if (fp == NULL) {
        printf("Errore: non riesco a aprire il file 'utenti.csv'.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char nome[100];

        sscanf(line, "%d,%[^,\n]", &id, nome);

        Utente new_utente = {id, strdup(nome)};
        utenti[*utenti_size] = new_utente;
        (*utenti_size)++;
    }

    fclose(fp);
}

void caricaPrestitiCSV(Prestito** prestiti, int *prestiti_size) {
    FILE* fp = fopen("data/prestiti.csv", "r");
    if (fp == NULL) {
        printf("Errore: non riesco a aprire il file 'prestiti.csv'.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int idLibro, idUtente;
        char dataPrestito[10], dataScadenza[10], stato[20];

        sscanf(line, "%d,%d,%[^,\n],%[^,\n],%[^,\n],%s", &idLibro, &idUtente, dataPrestito, dataScadenza, stato);

        Libro* found_libro = cercaLibro(idLibro, *prestiti, *prestiti_size);
        Utente* found_utente = NULL;
        for (int i = 0; i < (*prestiti_size) && found_utente == NULL; i++) {
            if (prestiti[i].utente->id == idUtente) {
                found_utente = prestiti[i].utente;
            }
        }

        if (found_libro && found_utente) {
            Prestito new_prestito = {
                .libro = found_libro,
                .utente = found_utente,
                .dataPrestito = time(NULL),
                .dataScadenza = time(NULL) + (24 * 60 * 60), // Simula una scadenza di un giorno
                .stato[0] = '\0'
            };

            strcpy(new_prestito.stato, stato);

            prestiti[*prestiti_size] = new_prestito;
            (*prestiti_size)++;
        }
    }

    fclose(fp);
}