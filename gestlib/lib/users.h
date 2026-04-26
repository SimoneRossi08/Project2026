#ifndef UTENTI_H
#define UTENTI_H

#include "prestiti.h"   /* forward-include per NodoPrestito */

#define MAX_NOME    128
#define MAX_EMAIL   128
#define MAX_TEL      32

typedef struct Utente {
    int    id;
    char   nome[MAX_NOME];
    char   email[MAX_EMAIL];
    char   telefono[MAX_TEL];
    int    prestiti_totali;   /* storico cumulativo */

    /* lista collegata dei prestiti ATTIVI */
    struct NodoPrestito *prestiti;
} Utente;

extern Utente **utenti;
extern int      num_utenti;
extern int      cap_utenti;

void    utenti_init(void);
void    utenti_free(void);

Utente *utente_nuovo(const char *nome, const char *email, const char *tel);
int     utente_aggiungi(Utente *u);
int     utente_elimina(int id);
Utente *utente_cerca_id(int id);
Utente *utente_cerca_nome(const char *nome);

void    utente_stampa_storico(int id);
void    utenti_stampa_tutti(void);

#endif /* UTENTI_H */
