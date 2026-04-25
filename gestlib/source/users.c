#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

Utente **utenti     = NULL;
int      num_utenti = 0;
int      cap_utenti = 0;

#define CAP_INIZIALE 16


void utenti_init(void){
    cap_utenti = CAP_INIZIALE;
    num_utenti = 0;
    utenti     = (Utente **)malloc(cap_utenti * sizeof(Utente *));
    if (!utenti) { perror("malloc utenti"); exit(EXIT_FAILURE); }
}

void utenti_free(void){
    int i;
    for (i = 0; i < num_utenti; i++) {
        /* libera la lista dei prestiti attivi */
        NodoPrestito *n = utenti[i]->prestiti;
        while (n) {
            NodoPrestito *tmp = n->next;
            free(n);
            n = tmp;
        }
        free(utenti[i]);
    }
    free(utenti);
    utenti     = NULL;
    num_utenti = cap_utenti = 0;
}

static void utenti_espandi(void){
    cap_utenti *= 2;
    utenti      = (Utente **)realloc(utenti, cap_utenti * sizeof(Utente *));
    if (!utenti) { perror("realloc utenti"); exit(EXIT_FAILURE); }
}


Utente *utente_nuovo(const char *nome, const char *email, const char *tel){
    Utente *u = (Utente *)calloc(1, sizeof(Utente));
    if (!u) { perror("calloc Utente"); return NULL; }

    u->id              = genera_id();
    strncpy(u->nome,     nome,  MAX_NOME  - 1);
    strncpy(u->email,    email, MAX_EMAIL - 1);
    strncpy(u->telefono, tel,   MAX_TEL   - 1);
    u->prestiti_totali = 0;
    u->prestiti        = NULL;
    return u;
}

int utente_aggiungi(Utente *u){
    if (!u) return -1;
    if (num_utenti >= cap_utenti) utenti_espandi();
    utenti[num_utenti++] = u;
    return u->id;
}

int utente_elimina(int id){
    int i;
    for (i = 0; i < num_utenti; i++) {
        if (utenti[i]->id == id) {
            /* libera lista prestiti attivi */
            NodoPrestito *n = utenti[i]->prestiti;
            while (n) {
                NodoPrestito *tmp = n->next;
                free(n);
                n = tmp;
            }
            free(utenti[i]);
            utenti[i] = utenti[--num_utenti];
            return 1;
        }
    }
    printf("  [!] Utente con ID %d non trovato.\n", id);
    return 0;
}

Utente *utente_cerca_id(int id){
    int i;
    for (i = 0; i < num_utenti; i++)
        if (utenti[i]->id == id) return utenti[i];
    return NULL;
}

Utente *utente_cerca_nome(const char *nome)
{
    int i;
    for (i = 0; i < num_utenti; i++)
        if (strstr(utenti[i]->nome, nome)) return utenti[i];
    return NULL;
}

void utente_stampa_storico(int id){
    Utente *u = utente_cerca_id(id);
    if (!u) { printf("  [!] Utente ID %d non trovato.\n", id); return; }

    char buf_p[32], buf_s[32], buf_r[32];
    int  n_attivi = 0;

    printf("\n  Utente: %s (ID %d)\n", u->nome, u->id);
    printf("  Email: %s  Tel: %s\n", u->email, u->telefono);
    printf("  Prestiti totali (storico cumulativo): %d\n\n", u->prestiti_totali);

    /* scorre la lista collegata dei prestiti attivi */
    NodoPrestito *nodo = u->prestiti;
    if (!nodo) {
        printf("  Nessun prestito attivo.\n");
    } else {
        stampa_separatore();
        printf("  %-6s %-40s %-12s %-12s %s\n",
               "ID", "Titolo", "Prestito", "Scadenza", "Stato");
        stampa_separatore();
        while (nodo) {
            Prestito *p = nodo->prestito;
            data_to_str(p->data_prestito,    buf_p, sizeof(buf_p));
            data_to_str(p->data_scadenza,    buf_s, sizeof(buf_s));
            if (p->restituito) {
                data_to_str(p->data_restituzione, buf_r, sizeof(buf_r));
            } else {
                snprintf(buf_r, sizeof(buf_r), "attivo");
            }
            printf("  [%4d] %-40s %s  %s  %s\n",
                   p->id,
                   p->libro ? p->libro->titolo : "?",
                   buf_p, buf_s, buf_r);
            if (!p->restituito) n_attivi++;
            nodo = nodo->next;
        }
        stampa_separatore();
        printf("  Prestiti attivi: %d\n", n_attivi);
    }
}

void utenti_stampa_tutti(void){
    int i;
    if (num_utenti == 0) { printf("  Nessun utente registrato.\n"); return; }
    stampa_separatore();
    printf("  %-6s %-25s %-30s %-15s %s\n",
           "ID", "Nome", "Email", "Telefono", "Prestiti tot.");
    stampa_separatore();
    for (i = 0; i < num_utenti; i++) {
        Utente *u = utenti[i];
        printf("  [%4d] %-25s %-30s %-15s %d\n",
               u->id, u->nome, u->email, u->telefono, u->prestiti_totali);
    }
    stampa_separatore();
    printf("  Totale utenti registrati: %d\n", num_utenti);
}
