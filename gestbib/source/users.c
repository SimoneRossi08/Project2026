#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"

#define INITIAL_CAPACITY 8

void inizializzaAnagrafica(Anagrafica* anagrafica){
    anagrafica->utenti=(Utente**)malloc(sizeof(Utente*) * INITIAL_CAPACITY);
    anagrafica->size=0;
    anagrafica->capacity=INITIAL_CAPACITY;
    anagrafica->next_id=1;
}

void liberaAnagrafica(Anagrafica* anagrafica){
    for(int i=0; i<anagrafica->size; i++){
        Utente* utente=anagrafica->utenti[i];
        NodoPrestito* current=utente->prestiti;
        while(current!=NULL){
            NodoPrestito* temp=current;
            current=current->next;
            free(temp->prestito);
            free(temp);
        }
        free(utente);
    }
    free(anagrafica->utenti);
}

void inserisciUtente(Anagrafica* anagrafica, Utente* utente){
    if(anagrafica->size>=anagrafica->capacity){
        anagrafica->capacity=anagrafica->capacity * 2;
        anagrafica->utenti=(Utente**)realloc(anagrafica->utenti,
                                                sizeof(Utente*) * anagrafica->capacity);
    }
    anagrafica->utenti[anagrafica->size]=utente;
    anagrafica->size++;
    if(utente->id>=anagrafica->next_id){
        anagrafica->next_id=utente->id + 1;
    }
}

void registraUtente(Anagrafica* anagrafica){
    Utente* utente=(Utente*)malloc(sizeof(Utente));
    utente->id=anagrafica->next_id;
    anagrafica->next_id++;
    utente->prestiti=NULL;
    utente->storico_count=0;

    printf("Nome utente: ");
    scanf(" %[^\n]", utente->nome);

    inserisciUtente(anagrafica, utente);
    salvaUtenti(anagrafica);
    printf("Utente registrato(ID: %d).\n", utente->id);
}

void eliminaUtente(Anagrafica* anagrafica){
    if(anagrafica->size==0){
        printf("Nessun utente presente.\n");
        return;
    }

    int id;
    printf("ID dell'utente da eliminare: ");
    scanf("%d", &id);

    int posizione=-1;
    for(int i=0; i<anagrafica->size; i++){
        if(anagrafica->utenti[i]->id==id){
            posizione=i;
            break;
        }
    }
    if(posizione==-1){
        printf("Utente non trovato.\n");
        return;
    }

    Utente* utente=anagrafica->utenti[posizione];
    if(utente->prestiti!=NULL){
        printf("Errore: l'utente ha prestiti attivi. Restituiscili prima.\n");
        return;
    }

    free(utente);
    for(int j=posizione; j<anagrafica->size - 1; j++){
        anagrafica->utenti[j]=anagrafica->utenti[j + 1];
    }
    anagrafica->size--;

    salvaUtenti(anagrafica);
    printf("Utente eliminato.\n");
}

void stampaUtenti(Anagrafica* anagrafica){
    if(anagrafica->size==0){
        printf("Nessun utente registrato.\n");
        return;
    }

    for(int i=0; i<anagrafica->size; i++){
        Utente* utente=anagrafica->utenti[i];
        int attivi=0;
        NodoPrestito* current=utente->prestiti;
        while(current!=NULL){
            attivi++;
            current=current->next;
        }
        printf("ID:%d | %s | attivi:%d | totale prestiti:%d\n",
               utente->id, utente->nome, attivi, utente->storico_count);
    }
}

void mostraStorico(Anagrafica* anagrafica){
    int id;
    printf("ID utente: ");
    scanf("%d", &id);

    Utente* utente=trovaUtentePerId(anagrafica, id);
    if(utente==NULL){
        printf("Utente non trovato.\n");
        return;
    }

    printf("\n--- Prestiti attivi di %s ---\n", utente->nome);
    NodoPrestito* current=utente->prestiti;
    int n=0;
    while(current!=NULL){
        Prestito* prestito=current->prestito;
        char data[64];
        struct tm* tm_info=localtime(&prestito->dataScadenza);
        strftime(data, sizeof(data), "%Y-%m-%d", tm_info);
        printf("  - \"%s\"(scadenza: %s)\n", prestito->libro->titolo, data);
        n++;
        current=current->next;
    }
    if(n==0){
        printf(" (nessun prestito attivo)\n");
    }
    printf("Totale prestiti effettuati nel tempo: %d\n", utente->storico_count);
}

Utente* trovaUtentePerId(Anagrafica* anagrafica, int id){
    for(int i=0; i<anagrafica->size; i++){
        if(anagrafica->utenti[i]->id==id){
            return anagrafica->utenti[i];
        }
    }
    return NULL;
}

void inserisciPrestito(Utente* utente, Prestito* prestito){
    NodoPrestito* nuovoNodo=(NodoPrestito*)malloc(sizeof(NodoPrestito));
    nuovoNodo->prestito=prestito;
    nuovoNodo->next=utente->prestiti;
    utente->prestiti=nuovoNodo;
}

void rimuoviPrestito(Utente* utente, Prestito* prestito){
    if(utente->prestiti==NULL){
        return;
    }

    if(utente->prestiti->prestito==prestito){
        NodoPrestito* temp=utente->prestiti;
        utente->prestiti=utente->prestiti->next;
        free(temp);
        return;
    }

    NodoPrestito* current=utente->prestiti;
    while(current->next!=NULL){
        if(current->next->prestito==prestito){
            NodoPrestito* temp=current->next;
            current->next=current->next->next;
            free(temp);
            return;
        }
        current=current->next;
    }
}