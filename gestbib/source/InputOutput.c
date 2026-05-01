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

void caricaLibriCSV(Libro* libri, int *libri_size){
    FILE* fp = fopen("data/books.csv", "r");
    if(fp == NULL){
        printf("Errore: non riesco a aprire il file 'books.csv'.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)){
        int id;
        char titolo[100], autore[100], genere[100];
        int copie;

    while (scanf("%d,%[^,\n],%[^,\n],%[^,\n],%d", &id, titolo, autore, genere, &copie) == 5) {
        Libro new_libro;
        new_libro.id = id;
        new_libro.titolo = strdup(titolo);
        new_libro.autore = strdup(autore);
        new_libro.genere = strdup(genere);
        new_libro.copie = copie;
        
        libri[*libri_size] = new_libro;
        (*libri_size)++;
    }

    fclose(fp);
}

void caricaUtentiCSV(Utente* utenti, int *utenti_size){
    FILE* fp = fopen("data/users.csv", "r");
    if(fp == NULL){
        printf("Errore: non riesco a aprire il file 'users.csv'.\n");
        return;
    }

    char line[256];
    while(fgets(line, sizeof(line), fp)){
        int id;
        char nome[100];
    }
    while(scanf("%d,%[^,\n]", &id, nome) == 2){
        Utente new_utente;
        new_utente.id = id;
        new_utente.nome = strdup(nome);
        utenti[*utenti_size] = new_utente;
        (*utenti_size)++;
    }

    fclose(fp);
}

void caricaPrestitiCSV(Prestito** prestiti, int *prestiti_size){
    FILE* fp = fopen("data/lending.csv", "r");
    if (fp == NULL) {
        printf("Errore: non riesco a aprire il file 'lending.csv'.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)){
        int idLibro, idUtente;
        char dataPrestito[10], dataScadenza[10], stato[20];
    }
    while (scanf("%d,%d,%[^,\n],%[^,\n],%[^,\n]", &idLibro, &idUtente, dataPrestito, dataScadenza, stato) == 5){
        Libro* trovatoLib = cercaLibro(idLibro, *prestiti, *prestiti_size);
        Utente* trovatoUser = NULL;
        for(int i = 0; i < (*prestiti_size) && trovatoUser == NULL; i++){
            if(prestiti[i].utente->id == idUtente){
                trovatoUser = prestiti[i].utente;
            }
        }

        if (trovatoLib && trovatoUser){
            Prestito new_prestito;
            new_prestito.libro = trovatoLib;
            new_prestito.utente = trovatoUser;
            new_prestito.stato[0] = '\0';

            strcpy(new_prestito.stato, stato);

            prestiti[*prestiti_size] = new_prestito;
            (*prestiti_size)++;
        }
    }
    fclose(fp);
}