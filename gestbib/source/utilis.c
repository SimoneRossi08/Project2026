#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/utilis.h"
#include "../lib/lending.h"

#define BAR_MAX 30

void stampaBarra(int valore, int massimo){
    int n=0;
    if (massimo>0){
        n=valore * BAR_MAX / massimo;
    }
    if (n==0 && valore>0){
        n=1;
    }
    for (int i=0; i<n; i++){
        putchar('|');
    }
}

void mostraStatistiche(Catalogo* catalogo, Anagrafica* anagrafica){
    if (catalogo->size==0 && anagrafica->size==0){
        printf("Nessun dato per le statistiche.\n");
        return;
    }

    Libro* libroTop=NULL;
    int maxPrestiti=0;
    for (int i=0; i<catalogo->size; i++){
        if (catalogo->libri[i]->volte_prestato>maxPrestiti){
            maxPrestiti=catalogo->libri[i]->volte_prestato;
            libroTop=catalogo->libri[i];
        }
    }
    printf("\n--- Libro piu' prestato ---\n");
    if (libroTop!=NULL && maxPrestiti>0){
        printf("\"%s\" - %d prestiti\n", libroTop->titolo, maxPrestiti);
    }else{
        printf("Nessun prestito ancora effettuato.\n");
    }

    Utente* utenteTop=NULL;
    int maxStorico=0;
    for (int i=0; i<anagrafica->size; i++){
        if (anagrafica->utenti[i]->storico_count>maxStorico){
            maxStorico=anagrafica->utenti[i]->storico_count;
            utenteTop=anagrafica->utenti[i];
        }
    }
    printf("\n--- Utente piu' attivo ---\n");
    if (utenteTop!=NULL && maxStorico>0){
        printf("%s - %d prestiti totali\n", utenteTop->nome, maxStorico);
    }else{
        printf("Nessun utente ha effettuato prestiti.\n");
    }

    int totale=0;
    int attivi=0;
    for (int i=0; i<anagrafica->size; i++){
        totale+=anagrafica->utenti[i]->storico_count;
        NodoPrestito* current=anagrafica->utenti[i]->prestiti;
        while (current!=NULL){
            attivi++;
            current=current->next;
        }
    }
    int restituiti=totale - attivi;
    printf("\n--- Tasso di restituzione ---\n");
    if (totale>0){
        int percentuale=restituiti * 100 / totale;
        printf("Restituiti %d/%d (%d%%)\n", restituiti, totale, percentuale);
    }else{
        printf("Nessun prestito ancora effettuato.\n");
    }

    printf("\n--- Generi piu' richiesti ---\n");
    if (catalogo->size==0){
        printf("Catalogo vuoto.\n");
        return;
    }

    char generi[256][100];
    int conteggi[256];
    int numGeneri=0;

    for (int i=0; i<catalogo->size; i++){
        Libro* libro=catalogo->libri[i];
        if (libro->volte_prestato>0){
            int trovato=-1;
            for (int j=0; j<numGeneri; j++){
                if (strcmp(generi[j], libro->genere)==0){
                    trovato=j;
                    break;
                }
            }
            if (trovato>=0){
                conteggi[trovato]+=libro->volte_prestato;
            }else{
                strcpy(generi[numGeneri], libro->genere);
                conteggi[numGeneri]=libro->volte_prestato;
                numGeneri++;
            }
        }
    }

    if (numGeneri==0){
        printf("Nessun prestito ancora effettuato.\n");
        return;
    }

    int massimo=0;
    for (int i=0; i<numGeneri; i++){
        if (conteggi[i]>massimo){
            massimo=conteggi[i];
        }
    }

    for (int i=0; i<numGeneri; i++){
        printf("%-20s ", generi[i]);
        stampaBarra(conteggi[i], massimo);
        printf(" %d\n", conteggi[i]);
    }
}