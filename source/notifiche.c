#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/notifiche.h"

void inizializzaCoda(CodaNotifiche* coda){
    coda->testa=NULL;
    coda->coda=NULL;
}

void inserisciNotifica(CodaNotifiche* coda, const char* messaggio){
    NodoNotifica* nuovoNodo=(NodoNotifica*)malloc(sizeof(NodoNotifica));
    strcpy(nuovoNodo->messaggio, messaggio);
    nuovoNodo->next=NULL;

    if(coda->testa==NULL){
        coda->testa=nuovoNodo;
        coda->coda=nuovoNodo;
    } else{
        coda->coda->next=nuovoNodo;
        coda->coda=nuovoNodo;
    }
}

void mostraNotifiche(CodaNotifiche* coda){
    if(coda->testa==NULL){
        printf("Nessuna notifica.\n");
        return;
    }

    printf("\n=== Notifiche ===\n");
    NodoNotifica* current=coda->testa;
    while(current!=NULL){
        printf("- %s\n", current->messaggio);
        NodoNotifica* temp=current;
        current=current->next;
        free(temp);
    }
    coda->testa=NULL;
    coda->coda=NULL;
    printf("=================\n");
}

void liberaCoda(CodaNotifiche* coda){
    NodoNotifica* current=coda->testa;
    while(current!=NULL){
        NodoNotifica* temp=current;
        current=current->next;
        free(temp);
    }
    coda->testa=NULL;
    coda->coda=NULL;
}