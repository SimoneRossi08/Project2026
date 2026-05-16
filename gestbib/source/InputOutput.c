#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/InputOutput.h"

// Estrae il prossimo campo separato da virgola dalla riga.
// Modifica la riga inserendo terminatori '\0' al posto delle virgole.
char* prossimoCampo(char** cursore){
    if(*cursore==NULL){
        return NULL;
    }
    char* inizio=*cursore;
    char* p=inizio;
    while(*p!='\0' && *p!=',' && *p!='\n' && *p!='\r'){
        p++;
    }
    if(*p!='\0'){
        *p='\0';
        *cursore=p + 1;
    }else{
        *cursore=NULL;
    }
    return inizio;
}

void caricaLibri(Catalogo* catalogo){
    FILE* f=fopen("data/books.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[512];
    fgets(riga, sizeof(riga), f);

    while(fgets(riga, sizeof(riga), f)!=NULL){
        char* cursore=riga;
        Libro* libro=(Libro*)malloc(sizeof(Libro));

        libro->id=atoi(prossimoCampo(&cursore));
        strcpy(libro->titolo, prossimoCampo(&cursore));
        strcpy(libro->autore, prossimoCampo(&cursore));
        strcpy(libro->genere, prossimoCampo(&cursore));
        libro->copie=atoi(prossimoCampo(&cursore));

        char* campoVP=prossimoCampo(&cursore);
        libro->volte_prestato=0;
        if(campoVP!=NULL){
            libro->volte_prestato=atoi(campoVP);
        }

        inserisciLibro(catalogo, libro);
    }

    fclose(f);
}

void salvaLibri(Catalogo* catalogo){
    FILE* f=fopen("data/books.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "id,titolo,autore,genere,copie,volte_prestato\n");
    for(int i=0; i<catalogo->size; i++){
        Libro* libro=catalogo->libri[i];
        fprintf(f, "%d,%s,%s,%s,%d,%d\n",
                libro->id, libro->titolo, libro->autore,
                libro->genere, libro->copie, libro->volte_prestato);
    }

    fclose(f);
}

void caricaUtenti(Anagrafica* anagrafica){
    FILE* f=fopen("data/users.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[256];
    fgets(riga, sizeof(riga), f);

    while(fgets(riga, sizeof(riga), f)!=NULL){
        char* cursore=riga;
        Utente* utente=(Utente*)malloc(sizeof(Utente));

        utente->id=atoi(prossimoCampo(&cursore));
        strcpy(utente->nome, prossimoCampo(&cursore));

        char* campoSC=prossimoCampo(&cursore);
        utente->storico_count=0;
        if(campoSC!=NULL){
            utente->storico_count=atoi(campoSC);
        }

        utente->prestiti=NULL;
        inserisciUtente(anagrafica, utente);
    }

    fclose(f);
}

void salvaUtenti(Anagrafica* anagrafica){
    FILE* f=fopen("data/users.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "id,nome,storico_count\n");
    for(int i=0; i<anagrafica->size; i++){
        Utente* utente=anagrafica->utenti[i];
        fprintf(f, "%d,%s,%d\n", utente->id, utente->nome, utente->storico_count);
    }

    fclose(f);
}

void caricaPrestiti(Catalogo* catalogo, Anagrafica* anagrafica, CodaNotifiche* coda){
    FILE* f=fopen("data/lending.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[256];
    fgets(riga, sizeof(riga), f);

    time_t adesso=time(NULL);

    while(fgets(riga, sizeof(riga), f)!=NULL){
        char* cursore=riga;

        int idLibro=atoi(prossimoCampo(&cursore));
        int idUtente=atoi(prossimoCampo(&cursore));
        long dataPrestito=atol(prossimoCampo(&cursore));
        long dataScadenza=atol(prossimoCampo(&cursore));

        Libro* libro=trovaLibroPerId(catalogo, idLibro);
        Utente* utente=trovaUtentePerId(anagrafica, idUtente);

        if(libro!=NULL && utente!=NULL){
            Prestito* prestito=(Prestito*)malloc(sizeof(Prestito));
            prestito->libro=libro;
            prestito->utente=utente;
            prestito->dataPrestito=(time_t)dataPrestito;
            prestito->dataScadenza=(time_t)dataScadenza;
            inserisciPrestito(utente, prestito);

            if(prestito->dataScadenza<adesso){
                char messaggio[256];
                char data[64];
                struct tm* tm_info=localtime(&prestito->dataScadenza);
                strftime(data, sizeof(data), "%Y-%m-%d", tm_info);
                printf("Prestito scaduto il %s: \"%s\" (utente: %s)", data, libro->titolo, utente->nome);
                inserisciNotifica(coda, messaggio);
            }
        }
    }

    fclose(f);
}

void salvaPrestiti(Anagrafica* anagrafica){
    FILE* f=fopen("data/lending.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "libro_id,utente_id,data_prestito,data_scadenza\n");
    for(int i=0; i<anagrafica->size; i++){
        NodoPrestito* current=anagrafica->utenti[i]->prestiti;
        while(current!=NULL){
            Prestito* prestito=current->prestito;
            fprintf(f, "%d,%d,%ld,%ld\n",
                    prestito->libro->id, prestito->utente->id,
                   (long)prestito->dataPrestito,(long)prestito->dataScadenza);
            current=current->next;
        }
    }

    fclose(f);
}

void aggiungiStorico(Prestito* prestito, time_t dataRestituzione){
    int vuoto=0;
    FILE* verifica=fopen("data/history.csv", "r");
    if(verifica==NULL){
        vuoto=1;
    }else{
        fseek(verifica, 0, SEEK_END);
        if(ftell(verifica)==0){
            vuoto=1;
        }
        fclose(verifica);
    }

    FILE* f=fopen("data/history.csv", "a");
    if(f==NULL){
        return;
    }

    if(vuoto){
        fprintf(f, "libro_id,utente_id,data_prestito,data_scadenza,data_restituzione\n");
    }
    fprintf(f, "%d,%d,%ld,%ld,%ld\n",
            prestito->libro->id, prestito->utente->id,
           (long)prestito->dataPrestito,(long)prestito->dataScadenza,
           (long)dataRestituzione);

    fclose(f);
}