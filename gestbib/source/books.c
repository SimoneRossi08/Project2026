#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"

#define INITIAL_CAPACITY 8

void inizializzaCatalogo(Catalogo* catalogo){
    catalogo->libri=(Libro**)malloc(sizeof(Libro*) * INITIAL_CAPACITY);
    catalogo->size=0;
    catalogo->capacity=INITIAL_CAPACITY;
    catalogo->next_id=1;
}

void liberaCatalogo(Catalogo* catalogo){
    for (int i=0; i<catalogo->size; i++){
        free(catalogo->libri[i]);
    }
    free(catalogo->libri);
}

void inserisciLibro(Catalogo* catalogo, Libro* libro){
    if (catalogo->size>=catalogo->capacity){
        catalogo->capacity=catalogo->capacity * 2;
        catalogo->libri=(Libro**)realloc(catalogo->libri, sizeof(Libro*) * catalogo->capacity);
    }
    catalogo->libri[catalogo->size]=libro;
    catalogo->size++;
    if (libro->id>=catalogo->next_id){
        catalogo->next_id=libro->id + 1;
    }
}

void aggiungiLibro(Catalogo* catalogo){
    Libro* libro=(Libro*)malloc(sizeof(Libro));
    libro->id=catalogo->next_id;
    catalogo->next_id++;
    libro->volte_prestato=0;

    printf("Titolo: ");
    scanf(" %[^\n]", libro->titolo);
    printf("Autore: ");
    scanf(" %[^\n]", libro->autore);
    printf("Genere: ");
    scanf(" %[^\n]", libro->genere);
    printf("Numero copie: ");
    scanf("%d", &libro->copie);

    inserisciLibro(catalogo, libro);
    salvaLibri(catalogo);
    printf("Libro aggiunto (ID: %d).\n", libro->id);
}

void modificaLibro(Catalogo* catalogo){
    if (catalogo->size==0){
        printf("Nessun libro presente.\n");
        return;
    }

    int id;
    printf("ID del libro da modificare: ");
    scanf("%d", &id);

    Libro* libro=trovaLibroPerId(catalogo, id);
    if (libro==NULL){
        printf("Libro non trovato.\n");
        return;
    }

    printf("Nuovo titolo: ");
    scanf(" %[^\n]", libro->titolo);
    printf("Nuovo autore: ");
    scanf(" %[^\n]", libro->autore);
    printf("Nuovo genere: ");
    scanf(" %[^\n]", libro->genere);
    printf("Nuove copie: ");
    scanf("%d", &libro->copie);

    salvaLibri(catalogo);
    printf("Libro modificato.\n");
}

void eliminaLibro(Catalogo* catalogo, struct Anagrafica* anag){
    if (catalogo->size==0){
        printf("Nessun libro presente.\n");
        return;
    }

    int id;
    printf("ID del libro da eliminare: ");
    scanf("%d", &id);

    int posizione=-1;
    for (int i=0; i<catalogo->size; i++){
        if (catalogo->libri[i]->id==id){
            posizione=i;
            break;
        }
    }
    if (posizione==-1){
        printf("Libro non trovato.\n");
        return;
    }

    Libro* libro=catalogo->libri[posizione];

    Anagrafica* anagrafica=(Anagrafica*)anag;
    int prestitoAttivo=0;
    for (int i=0; i<anagrafica->size; i++){
        NodoPrestito* current=anagrafica->utenti[i]->prestiti;
        while (current!=NULL){
            if (current->prestito->libro==libro){
                prestitoAttivo=1;
            }
            current=current->next;
        }
    }

    if (prestitoAttivo){
        printf("Errore: ci sono prestiti attivi per questo libro.\n");
        return;
    }

    free(libro);
    for (int j=posizione; j<catalogo->size - 1; j++){
        catalogo->libri[j]=catalogo->libri[j + 1];
    }
    catalogo->size--;

    salvaLibri(catalogo);
    printf("Libro eliminato.\n");
}

void cercaLibri(Catalogo* catalogo){
    if (catalogo->size==0){
        printf("Catalogo vuoto.\n");
        return;
    }

    char query[100];
    printf("Testo da cercare (titolo/autore/genere): ");
    scanf(" %[^\n]", query);

    int trovati=0;
    for (int i=0; i<catalogo->size; i++){
        Libro* libro=catalogo->libri[i];
        if (strstr(libro->titolo, query)!=NULL ||
            strstr(libro->autore, query)!=NULL ||
            strstr(libro->genere, query)!=NULL){
            printf("ID:%d | %s | %s | %s | copie:%d\n",
                   libro->id, libro->titolo, libro->autore,
                   libro->genere, libro->copie);
            trovati++;
        }
    }
    if (trovati==0){
        printf("Nessun libro corrisponde a \"%s\".\n", query);
    }
}

Libro* trovaLibroPerId(Catalogo* catalogo, int id){
    for (int i=0; i<catalogo->size; i++){
        if (catalogo->libri[i]->id==id){
            return catalogo->libri[i];
        }
    }
    return NULL;
}