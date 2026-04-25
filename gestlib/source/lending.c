#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

/* ---- array globale dei prestiti ---- */
Prestito **tutti_prestiti = NULL;
int        num_prestiti   = 0;
int        cap_prestiti   = 0;

/* ---- coda notifiche ---- */
CodaNotifiche notifiche = {NULL, NULL, 0};

#define CAP_INIZIALE 16

void prestiti_init(void){
    cap_prestiti   = CAP_INIZIALE;
    num_prestiti   = 0;
    tutti_prestiti = (Prestito **)malloc(cap_prestiti * sizeof(Prestito *));
    if (!tutti_prestiti) { perror("malloc prestiti"); exit(EXIT_FAILURE); }
}

void prestiti_free(void)
{
    int i;
    for (i = 0; i < num_prestiti; i++) free(tutti_prestiti[i]);
    free(tutti_prestiti);
    tutti_prestiti = NULL;
    num_prestiti   = cap_prestiti = 0;

    /* svuota la coda notifiche */
    NodoNotifica *n = notifiche.testa;
    while (n) {
        NodoNotifica *tmp = n->next;
        free(n);
        n = tmp;
    }
    notifiche.testa = notifiche.coda = NULL;
    notifiche.dimensione = 0;
}

static void prestiti_espandi(void){
    cap_prestiti *= 2;
    tutti_prestiti = (Prestito **)realloc(tutti_prestiti,
                                          cap_prestiti * sizeof(Prestito *));
    if (!tutti_prestiti) { perror("realloc prestiti"); exit(EXIT_FAILURE); }
}

void coda_notifiche_enqueue(const char *msg){
    NodoNotifica *n = (NodoNotifica *)malloc(sizeof(NodoNotifica));
    if (!n) return;
    strncpy(n->messaggio, msg, sizeof(n->messaggio) - 1);
    n->messaggio[sizeof(n->messaggio) - 1] = 0;
    n->next = NULL;

    if (!notifiche.coda) {
        notifiche.testa = notifiche.coda = n;
    } else {
        notifiche.coda->next = n;
        notifiche.coda       = n;
    }
    notifiche.dimensione++;
}

char *coda_notifiche_dequeue(void){
    if (!notifiche.testa) return NULL;

    NodoNotifica *n   = notifiche.testa;
    char         *msg = (char *)malloc(sizeof(n->messaggio));
    if (!msg) return NULL;
    strcpy(msg, n->messaggio);

    notifiche.testa = n->next;
    if (!notifiche.testa) notifiche.coda = NULL;
    free(n);
    notifiche.dimensione--;
    return msg;
}

int coda_notifiche_vuota(void){
    return notifiche.dimensione == 0;
}

Prestito *prestito_nuovo(int libro_id, int utente_id){
    Libro  *l = libro_cerca_id(libro_id);
    Utente *u = utente_cerca_id(utente_id);

    if (!l) { printf("  [!] Libro ID %d non trovato.\n",  libro_id);  return NULL; }
    if (!u) { printf("  [!] Utente ID %d non trovato.\n", utente_id); return NULL; }
    if (l->copie_disponibili <= 0) {
        printf("  [!] Nessuna copia disponibile di \"%s\".\n", l->titolo);
        return NULL;
    }

    Prestito *p = (Prestito *)calloc(1, sizeof(Prestito));
    if (!p) { perror("calloc Prestito"); return NULL; }

    p->id               = genera_id();
    p->libro            = l;
    p->utente           = u;
    p->data_prestito    = oggi();
    p->data_scadenza    = oggi_plus_giorni(DURATA_PRESTITO_GIORNI);
    p->restituito       = 0;
    p->data_restituzione = 0;

    /* aggiorna contatori libro */
    l->copie_disponibili--;
    l->volte_prestato++;

    /* aggiunge alla lista collegata dell'utente (in testa) */
    NodoPrestito *nodo = (NodoPrestito *)malloc(sizeof(NodoPrestito));
    if (!nodo) { free(p); perror("malloc NodoPrestito"); return NULL; }
    nodo->prestito = p;
    nodo->next     = u->prestiti;
    u->prestiti    = nodo;

    /* aggiorna contatore storico utente */
    u->prestiti_totali++;

    /* aggiunge all'array globale */
    if (num_prestiti >= cap_prestiti) prestiti_espandi();
    tutti_prestiti[num_prestiti++] = p;

    char buf[32];
    data_to_str(p->data_scadenza, buf, sizeof(buf));
    printf("  [OK] Prestito #%d creato. Scadenza: %s\n", p->id, buf);
    return p;
}


int prestito_restituisci(int prestito_id){
    Prestito *p = prestito_cerca_id(prestito_id);
    if (!p) { printf("  [!] Prestito ID %d non trovato.\n", prestito_id); return 0; }
    if (p->restituito) { printf("  [!] Prestito già restituito.\n"); return 0; }

    p->restituito        = 1;
    p->data_restituzione = oggi();

    if (p->libro) p->libro->copie_disponibili++;

    printf("  [OK] Restituzione registrata per \"%s\" (prestito #%d).\n",
           p->libro ? p->libro->titolo : "?", p->id);
    return 1;
}

Prestito *prestito_cerca_id(int id){
    int i;
    for (i = 0; i < num_prestiti; i++)
        if (tutti_prestiti[i]->id == id) return tutti_prestiti[i];
    return NULL;
}

void prestiti_verifica_scadenze(void){
    int i;
    time_t adesso = oggi();
    char buf[512], buf_data[32];

    for(i = 0; i < num_prestiti; i++){
        Prestito *p = tutti_prestiti[i];
        if (!p->restituito && difftime(adesso, p->data_scadenza) > 0) {
            int giorni = (int)(difftime(adesso, p->data_scadenza) / 86400);
            data_to_str(p->data_scadenza, buf_data, sizeof(buf_data));
            snprintf(buf, sizeof(buf),
                     "SCADUTO da %d giorno/i — Prestito #%d | \"%s\" | Utente: %s (ID %d) | Scadenza: %s",
                     giorni,
                     p->id,
                     p->libro  ? p->libro->titolo  : "?",
                     p->utente ? p->utente->nome    : "?",
                     p->utente ? p->utente->id      : -1,
                     buf_data);
            coda_notifiche_enqueue(buf);
        }
    }
}

void prestiti_stampa_scaduti(void){
    int    i, trovati = 0;
    time_t adesso = oggi();
    char   buf_p[32], buf_s[32];

    printf("\n  Prestiti scaduti non restituiti:\n");
    stampa_separatore();
    printf("  %-6s %-35s %-20s %-12s %-12s %s\n",
           "ID", "Titolo", "Utente", "Prestito", "Scadenza", "Giorni scad.");
    stampa_separatore();
    for (i = 0; i < num_prestiti; i++) {
        Prestito *p = tutti_prestiti[i];
        if (!p->restituito && difftime(adesso, p->data_scadenza) > 0) {
            int giorni = (int)(difftime(adesso, p->data_scadenza) / 86400);
            data_to_str(p->data_prestito, buf_p, sizeof(buf_p));
            data_to_str(p->data_scadenza, buf_s, sizeof(buf_s));
            printf("  [%4d] %-35s %-20s %s  %s  +%d gg\n",
                   p->id,
                   p->libro  ? p->libro->titolo  : "?",
                   p->utente ? p->utente->nome    : "?",
                   buf_p, buf_s, giorni);
            trovati++;
        }
    }
    if (!trovati) printf("  Nessun prestito scaduto.\n");
    stampa_separatore();
    printf("  Totale scaduti: %d\n", trovati);
}