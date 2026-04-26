#ifndef PRESTITI_H
#define PRESTITI_H

#include <time.h>

/* Forward declarations */
struct Libro;
struct Utente;

#define DURATA_PRESTITO_GIORNI 30

typedef struct Prestito {
    int    id;
    struct Libro  *libro;
    struct Utente *utente;
    time_t data_prestito;
    time_t data_scadenza;
    int    restituito;      /* 0 = attivo, 1 = restituito */
    time_t data_restituzione;
} Prestito;

/* nodo della lista collegata per i prestiti di un utente */
typedef struct NodoPrestito {
    Prestito           *prestito;
    struct NodoPrestito *next;
} NodoPrestito;

/* ---------- coda FIFO per le notifiche di scadenza ---------- */
typedef struct NodoNotifica {
    char               messaggio[512];
    struct NodoNotifica *next;
} NodoNotifica;

typedef struct CodaNotifiche {
    NodoNotifica *testa;
    NodoNotifica *coda;
    int           dimensione;
} CodaNotifiche;

extern CodaNotifiche notifiche;

/* array globale di tutti i prestiti (attivi + storici) */
extern Prestito **tutti_prestiti;
extern int        num_prestiti;
extern int        cap_prestiti;

void     prestiti_init(void);
void     prestiti_free(void);

Prestito *prestito_nuovo(int libro_id, int utente_id);
int       prestito_restituisci(int prestito_id);
void      prestiti_verifica_scadenze(void);
void      prestiti_stampa_scaduti(void);
Prestito *prestito_cerca_id(int id);

/* coda notifiche */
void coda_notifiche_enqueue(const char *msg);
char *coda_notifiche_dequeue(void); /* restituisce stringa allocata, caller deve free() */
int   coda_notifiche_vuota(void);

#endif /* PRESTITI_H */
