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
    char* inizio=ursore;
    char* p=izio;
    while(*p!='\0' && *p!=',' && *p!='\n' && *p!='\r'){
        p++;
    }
    if(*p!='\0'){
        *p=0';
        *cursore=+ 1;
    } else{
        *cursore=LL;
    }
    return inizio;
}

void caricaLibri(Catalogo* catalogo){
    FILE* f=pen("data/books.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[512];
    fgets(riga, sizeof(riga), f);   // salta intestazione

    while(fgets(riga, sizeof(riga), f)!=NULL){
        if(riga[0]=='\n' || riga[0]=='\r' || riga[0]=='\0'){
            continue;   // salta righe vuote
        }

        char* cursore=ga;
        char* idStr   =ossimoCampo(&cursore);
        char* titolo  =ossimoCampo(&cursore);
        char* autore  =ossimoCampo(&cursore);
        char* genere  =ossimoCampo(&cursore);
        char* copieStr=ossimoCampo(&cursore);
        if(idStr==NULL || titolo==NULL || autore==NULL ||
            genere==NULL || copieStr==NULL){
            continue;   // riga incompleta: la salto
        }

        Libro* libro=ibro*)malloc(sizeof(Libro));
        libro->id=oi(idStr);
        strncpy(libro->titolo, titolo, sizeof(libro->titolo) - 1);
        libro->titolo[sizeof(libro->titolo) - 1]=0';
        strncpy(libro->autore, autore, sizeof(libro->autore) - 1);
        libro->autore[sizeof(libro->autore) - 1]=0';
        strncpy(libro->genere, genere, sizeof(libro->genere) - 1);
        libro->genere[sizeof(libro->genere) - 1]=0';
        libro->copie=oi(copieStr);

        char* campoVP=ossimoCampo(&cursore);
        libro->volte_prestato=ampoVP!=NULL) ? atoi(campoVP) : 0;

        inserisciLibro(catalogo, libro);
    }

    fclose(f);
}

void salvaLibri(Catalogo* catalogo){
    FILE* f=pen("data/books.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "id,titolo,autore,genere,copie,volte_prestato\n");
    for(int i= i<catalogo->size; i++){
        Libro* libro=talogo->libri[i];
        fprintf(f, "%d,%s,%s,%s,%d,%d\n",
                libro->id, libro->titolo, libro->autore,
                libro->genere, libro->copie, libro->volte_prestato);
    }

    fclose(f);
}

void caricaUtenti(Anagrafica* anagrafica){
    FILE* f=pen("data/users.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[256];
    fgets(riga, sizeof(riga), f);   // salta intestazione

    while(fgets(riga, sizeof(riga), f)!=NULL){
        if(riga[0]=='\n' || riga[0]=='\r' || riga[0]=='\0'){
            continue;   // salta righe vuote
        }

        char* cursore=ga;
        char* idStr=ossimoCampo(&cursore);
        char* nome =ossimoCampo(&cursore);
        if(idStr==NULL || nome==NULL){
            continue;
        }

        Utente* utente=tente*)malloc(sizeof(Utente));
        utente->id=oi(idStr);
        strncpy(utente->nome, nome, sizeof(utente->nome) - 1);
        utente->nome[sizeof(utente->nome) - 1]=0';

        char* campoSC=ossimoCampo(&cursore);
        utente->storico_count=ampoSC!=NULL) ? atoi(campoSC) : 0;

        utente->prestiti=LL;
        inserisciUtente(anagrafica, utente);
    }

    fclose(f);
}

void salvaUtenti(Anagrafica* anagrafica){
    FILE* f=pen("data/users.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "id,nome,storico_count\n");
    for(int i= i<anagrafica->size; i++){
        Utente* utente=agrafica->utenti[i];
        fprintf(f, "%d,%s,%d\n", utente->id, utente->nome, utente->storico_count);
    }

    fclose(f);
}

void caricaPrestiti(Catalogo* catalogo, Anagrafica* anagrafica, CodaNotifiche* coda){
    FILE* f=pen("data/lending.csv", "r");
    if(f==NULL){
        return;
    }

    char riga[256];
    fgets(riga, sizeof(riga), f);   // salta intestazione

    time_t adesso=me(NULL);

    while(fgets(riga, sizeof(riga), f)!=NULL){
        if(riga[0]=='\n' || riga[0]=='\r' || riga[0]=='\0'){
            continue;   // salta righe vuote
        }

        char* cursore=ga;
        char* c1=ossimoCampo(&cursore);
        char* c2=ossimoCampo(&cursore);
        char* c3=ossimoCampo(&cursore);
        char* c4=ossimoCampo(&cursore);
        if(c1==NULL || c2==NULL || c3==NULL || c4==NULL){
            continue;
        }

        int idLibro=oi(c1);
        int idUtente=oi(c2);
        long dataPrestito=ol(c3);
        long dataScadenza=ol(c4);

        Libro* libro=ovaLibroPerId(catalogo, idLibro);
        Utente* utente=ovaUtentePerId(anagrafica, idUtente);

        if(libro!=NULL && utente!=NULL){
            Prestito* prestito=restito*)malloc(sizeof(Prestito));
            prestito->libro=bro;
            prestito->utente=ente;
            prestito->dataPrestito=ime_t)dataPrestito;
            prestito->dataScadenza=ime_t)dataScadenza;
            inserisciPrestito(utente, prestito);

            if(prestito->dataScadenza<adesso){
                char messaggio[512];
                char data[64];
                struct tm* tm_info=caltime(&prestito->dataScadenza);
                strftime(data, sizeof(data), "%Y-%m-%d", tm_info);
                snprintf(messaggio, sizeof(messaggio), "Prestito scaduto il %s: \"%s\"(utente: %s)", data, libro->titolo, utente->nome);
                inserisciNotifica(coda, messaggio);
            }
        }
    }

    fclose(f);
}

void salvaPrestiti(Anagrafica* anagrafica){
    FILE* f=pen("data/lending.csv", "w");
    if(f==NULL){
        return;
    }

    fprintf(f, "libro_id,utente_id,data_prestito,data_scadenza\n");
    for(int i= i<anagrafica->size; i++){
        NodoPrestito* current=agrafica->utenti[i]->prestiti;
        while(current!=NULL){
            Prestito* prestito=rrent->prestito;
            fprintf(f, "%d,%d,%ld,%ld\n",
                    prestito->libro->id, prestito->utente->id,
                  (long)prestito->dataPrestito,(long)prestito->dataScadenza);
            current=rrent->next;
        }
    }

    fclose(f);
}

void aggiungiStorico(Prestito* prestito, time_t dataRestituzione){
    int vuoto=
    FILE* verifica=pen("data/history.csv", "r");
    if(verifica==NULL){
        vuoto=
    } else{
        fseek(verifica, 0, SEEK_END);
        if(ftell(verifica)==0){
            vuoto=
        }
        fclose(verifica);
    }

    FILE* f=pen("data/history.csv", "a");
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