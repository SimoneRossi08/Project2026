#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

void csv_escape(const char *src, char *dst, int dst_len){
    int i = 0, j = 0;
    while (src[i] && j < dst_len - 1) {
        dst[j++] = (src[i] == ',') ? ';' : src[i];
        i++;
    }
    dst[j] = 0;
}

int salva_libri(void){
    FILE *f = fopen(FILE_LIBRI, "w");
    if(!f){ perror("fopen libri.csv"); return 0; }

    fprintf(f, "id,titolo,autore,genere,isbn,anno,copie_totali,copie_disponibili,volte_prestato\n");
    int i;
    char t[MAX_TITOLO], a[MAX_AUTORE], g[MAX_GENERE];
    for(i = 0; i < num_libri; i++){
        Libro *l = catalogo[i];
        csv_escape(l->titolo, t, MAX_TITOLO);
        csv_escape(l->autore, a, MAX_AUTORE);
        csv_escape(l->genere, g, MAX_GENERE);
        fprintf(f, "%d,%s,%s,%s,%s,%d,%d,%d,%d\n",
                l->id, t, a, g, l->isbn,
                l->anno, l->copie_totali, l->copie_disponibili, l->volte_prestato);
    }
    fclose(f);
    return 1;
}

int carica_libri(void){
    FILE *f = fopen(FILE_LIBRI, "r");
    if (!f) return 0;   /* primo avvio — file non esiste ancora */

    char riga[1024];
    fgets(riga, sizeof(riga), f);   /* intestazione */

    while (fgets(riga, sizeof(riga), f)) {
        riga[strcspn(riga, "\n")] = 0;
        if (!riga[0]) continue;

        Libro *l = (Libro *)calloc(1, sizeof(Libro));
        if (!l) continue;

        char *tok;
        tok = strtok(riga, ","); if (!tok) { free(l); continue; }
        l->id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        strncpy(l->titolo, tok, MAX_TITOLO - 1);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        strncpy(l->autore, tok, MAX_AUTORE - 1);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        strncpy(l->genere, tok, MAX_GENERE - 1);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        strncpy(l->isbn, tok, MAX_ISBN - 1);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        l->anno = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        l->copie_totali = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        l->copie_disponibili = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(l); continue; }
        l->volte_prestato = atoi(tok);

        libro_aggiungi(l);
    }
    fclose(f);
    return 1;
}

int salva_utenti(void){
    FILE *f = fopen(FILE_UTENTI, "w");
    if (!f) { perror("fopen utenti.csv"); return 0; }

    fprintf(f, "id,nome,email,telefono,prestiti_totali\n");
    int i;
    char n[MAX_NOME], e[MAX_EMAIL];
    for (i = 0; i < num_utenti; i++) {
        Utente *u = utenti[i];
        csv_escape(u->nome,  n, MAX_NOME);
        csv_escape(u->email, e, MAX_EMAIL);
        fprintf(f, "%d,%s,%s,%s,%d\n",
                u->id, n, e, u->telefono, u->prestiti_totali);
    }
    fclose(f);
    return 1;
}

int carica_utenti(void){
    FILE *f = fopen(FILE_UTENTI, "r");
    if (!f) return 0;

    char riga[512];
    fgets(riga, sizeof(riga), f);

    while (fgets(riga, sizeof(riga), f)) {
        riga[strcspn(riga, "\n")] = 0;
        if (!riga[0]) continue;

        Utente *u = (Utente *)calloc(1, sizeof(Utente));
        if (!u) continue;

        char *tok;
        tok = strtok(riga, ","); if (!tok) { free(u); continue; }
        u->id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(u); continue; }
        strncpy(u->nome, tok, MAX_NOME - 1);

        tok = strtok(NULL, ","); if (!tok) { free(u); continue; }
        strncpy(u->email, tok, MAX_EMAIL - 1);

        tok = strtok(NULL, ","); if (!tok) { free(u); continue; }
        strncpy(u->telefono, tok, MAX_TEL - 1);

        tok = strtok(NULL, ","); if (!tok) { free(u); continue; }
        u->prestiti_totali = atoi(tok);

        u->prestiti = NULL;
        utente_aggiungi(u);
    }
    fclose(f);
    return 1;
}


int salva_prestiti(void){
    FILE *f = fopen(FILE_PRESTITI, "w");
    if (!f) { perror("fopen prestiti.csv"); return 0; }

    fprintf(f, "id,libro_id,utente_id,data_prestito,data_scadenza,restituito,data_restituzione\n");
    int i;
    for(i = 0; i < num_prestiti; i++){
        Prestito *p = tutti_prestiti[i];
        if (p->restituito) continue;   /* i restituiti vanno in storico */
        fprintf(f, "%d,%d,%d,%ld,%ld,%d,%ld\n",
                p->id,
                p->libro  ? p->libro->id  : -1,
                p->utente ? p->utente->id : -1,
                (long)p->data_prestito,
                (long)p->data_scadenza,
                p->restituito,
                (long)p->data_restituzione);
    }
    fclose(f);
    return 1;
}

int carica_prestiti(void){
    FILE *f = fopen(FILE_PRESTITI, "r");
    if (!f) return 0;

    char riga[256];
    fgets(riga, sizeof(riga), f);

    while(fgets(riga, sizeof(riga), f)){
        riga[strcspn(riga, "\n")] = 0;
        if (!riga[0]) continue;

        Prestito *p = (Prestito *)calloc(1, sizeof(Prestito));
        if (!p) continue;

        char *tok;
        tok = strtok(riga, ","); if (!tok) { free(p); continue; }
        p->id = atoi(tok);

        int libro_id, utente_id;
        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        libro_id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        utente_id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_prestito = (time_t)atol(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_scadenza = (time_t)atol(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->restituito = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_restituzione = (time_t)atol(tok);

        p->libro  = libro_cerca_id(libro_id);
        p->utente = utente_cerca_id(utente_id);

        /* ricostruisce la lista collegata nell'utente */
        if(p->utente && !p->restituito){
            NodoPrestito *nodo = (NodoPrestito *)malloc(sizeof(NodoPrestito));
            if (nodo) {
                nodo->prestito    = p;
                nodo->next        = p->utente->prestiti;
                p->utente->prestiti = nodo;
            }
        }

        if(num_prestiti >= cap_prestiti){
            cap_prestiti *= 2;
            tutti_prestiti = (Prestito **)realloc(tutti_prestiti,
                                                   cap_prestiti * sizeof(Prestito *));
            if(!tutti_prestiti) { perror("realloc"); exit(EXIT_FAILURE); }
        }
        tutti_prestiti[num_prestiti++] = p;
    }
    fclose(f);
    return 1;
}


int salva_storico(void){
    FILE *f = fopen(FILE_STORICO, "w");
    if (!f) { perror("fopen storico.csv"); return 0; }

    fprintf(f, "id,libro_id,utente_id,data_prestito,data_scadenza,restituito,data_restituzione\n");
    int i;
    for(i = 0; i < num_prestiti; i++){
        Prestito *p = tutti_prestiti[i];
        if (!p->restituito) continue;
        fprintf(f, "%d,%d,%d,%ld,%ld,%d,%ld\n",
                p->id,
                p->libro  ? p->libro->id  : -1,
                p->utente ? p->utente->id : -1,
                (long)p->data_prestito,
                (long)p->data_scadenza,
                p->restituito,
                (long)p->data_restituzione);
    }
    fclose(f);
    return 1;
}

int carica_storico(void){
    FILE *f = fopen(FILE_STORICO, "r");
    if (!f) return 0;

    char riga[256];
    fgets(riga, sizeof(riga), f);

    while(fgets(riga, sizeof(riga), f)) {
        riga[strcspn(riga, "\n")] = 0;
        if (!riga[0]) continue;

        Prestito *p = (Prestito *)calloc(1, sizeof(Prestito));
        if (!p) continue;

        char *tok;
        tok = strtok(riga, ","); if (!tok) { free(p); continue; }
        p->id = atoi(tok);

        int libro_id, utente_id;
        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        libro_id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        utente_id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_prestito = (time_t)atol(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_scadenza = (time_t)atol(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->restituito = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) { free(p); continue; }
        p->data_restituzione = (time_t)atol(tok);

        p->libro  = libro_cerca_id(libro_id);
        p->utente = utente_cerca_id(utente_id);

        if(num_prestiti >= cap_prestiti){
            cap_prestiti *= 2;
            tutti_prestiti = (Prestito **)realloc(tutti_prestiti,
                                                   cap_prestiti * sizeof(Prestito *));
            if (!tutti_prestiti) { perror("realloc storico"); exit(EXIT_FAILURE); }
        }
        tutti_prestiti[num_prestiti++] = p;
    }
    fclose(f);
    return 1;
}

void carica_tutto(void){
    carica_libri();
    carica_utenti();
    carica_prestiti();
    carica_storico();
    printf("  [OK] Dati caricati dai file CSV.\n");
}

void salva_tutto(void){
    salva_libri();
    salva_utenti();
    salva_prestiti();
    salva_storico();
    printf("  [OK] Dati salvati sui file CSV.\n");
}