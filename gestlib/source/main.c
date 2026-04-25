#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

static void menu_libri(void);
static void menu_utenti(void);
static void menu_prestiti(void);
static void menu_ricerca(void);

extern void utils_set_prossimo_id(int id);

static void sincronizza_id(void)
{
    int i, max_id = 0;
    for (i = 0; i < num_libri;   i++) if (catalogo[i]->id       > max_id) max_id = catalogo[i]->id;
    for (i = 0; i < num_utenti;  i++) if (utenti[i]->id         > max_id) max_id = utenti[i]->id;
    for (i = 0; i < num_prestiti;i++) if (tutti_prestiti[i]->id > max_id) max_id = tutti_prestiti[i]->id;
    utils_set_prossimo_id(max_id + 1);
}

static void menu_aggiungi_libro(void)
{
    char titolo[256], autore[128], genere[64], isbn[20];
    int  anno, copie;

    stampa_intestazione("AGGIUNGI LIBRO");
    leggi_stringa("Titolo",  titolo, sizeof(titolo));
    leggi_stringa("Autore",  autore, sizeof(autore));
    leggi_stringa("Genere",  genere, sizeof(genere));
    leggi_stringa("ISBN",    isbn,   sizeof(isbn));
    anno  = leggi_intero("Anno pubblicazione", 1000, 2100);
    copie = leggi_intero("Numero copie",       1,    9999);

    Libro *l = libro_nuovo(titolo, autore, genere, isbn, anno, copie);
    if (l) {
        libro_aggiungi(l);
        printf("\n  [OK] Libro aggiunto con ID %d.\n", l->id);
    }
}

static void menu_libri(void){
    int scelta;
    do {
        stampa_intestazione("GESTIONE LIBRI");
        printf("  1. Visualizza catalogo\n");
        printf("  2. Aggiungi libro\n");
        printf("  3. Modifica libro\n");
        printf("  4. Elimina libro\n");
        printf("  5. Ordina per titolo\n");
        printf("  6. Ordina per autore\n");
        printf("  0. Torna al menu principale\n");
        scelta = leggi_intero("Scelta", 0, 6);

        switch(scelta){
            case 1: libri_stampa_tutti();    break;
            case 2: menu_aggiungi_libro();   break;
            case 3: {
                int id = leggi_intero("ID libro da modificare", 1, 999999);
                libro_modifica(id);
                break;
            }
            case 4:{
                int id = leggi_intero("ID libro da eliminare", 1, 999999);
                if (libro_elimina(id))
                    printf("  [OK] Libro eliminato.\n");
                break;
            }
            case 5: libri_ordina_titolo();   break;
            case 6: libri_ordina_autore();   break;
        }
    } while (scelta != 0);
}


static void menu_aggiungi_utente(void)
{
    char nome[128], email[128], tel[32];

    stampa_intestazione("REGISTRA UTENTE");
    leggi_stringa("Nome completo", nome,  sizeof(nome));
    leggi_stringa("Email",         email, sizeof(email));
    leggi_stringa("Telefono",      tel,   sizeof(tel));

    Utente *u = utente_nuovo(nome, email, tel);
    if (u){
        utente_aggiungi(u);
        printf("\n  [OK] Utente registrato con ID %d.\n", u->id);
    }
}

static void menu_utenti(void)
{
    int scelta;
    do{
        stampa_intestazione("GESTIONE UTENTI");
        printf("  1. Visualizza tutti gli utenti\n");
        printf("  2. Registra nuovo utente\n");
        printf("  3. Visualizza storico prestiti utente\n");
        printf("  4. Cancella utente\n");
        printf("  0. Torna al menu principale\n");
        scelta = leggi_intero("Scelta", 0, 4);

        switch (scelta) {
            case 1: utenti_stampa_tutti();  break;
            case 2: menu_aggiungi_utente(); break;
            case 3: {
                int id = leggi_intero("ID utente", 1, 999999);
                utente_stampa_storico(id);
                break;
            }
            case 4: {
                int id = leggi_intero("ID utente da cancellare", 1, 999999);
                if (utente_elimina(id))
                    printf("  [OK] Utente rimosso.\n");
                break;
            }
        }
    }while(scelta != 0);
}


static void menu_prestiti(void){
    int scelta;
    do {
        stampa_intestazione("GESTIONE PRESTITI");
        printf("  1. Nuovo prestito\n");
        printf("  2. Registra restituzione\n");
        printf("  3. Visualizza prestiti scaduti\n");
        printf("  4. Lista tutti i prestiti\n");
        printf("  0. Torna al menu principale\n");
        scelta = leggi_intero("Scelta", 0, 4);

        switch(scelta){
            case 1:
                int lid = leggi_intero("ID libro",   1, 999999);
                int uid = leggi_intero("ID utente",  1, 999999);
                prestito_nuovo(lid, uid);
                break;
            case 2: 
                int pid = leggi_intero("ID prestito da restituire", 1, 999999);
                prestito_restituisci(pid);
                break;
            case 3: 
                prestiti_stampa_scaduti(); 
                break;
            case 4:
                int    i;
                char   bp[32], bs[32];
                stampa_separatore();
                printf("  %-6s %-35s %-20s %-12s %-12s %s\n",
                       "ID","Titolo","Utente","Prestito","Scadenza","Stato");
                stampa_separatore();
                for (i = 0; i < num_prestiti; i++) {
                    Prestito *p = tutti_prestiti[i];
                    data_to_str(p->data_prestito, bp, sizeof(bp));
                    data_to_str(p->data_scadenza, bs, sizeof(bs));
                    printf("  [%4d] %-35s %-20s %s  %s  %s\n",
                           p->id,
                           p->libro  ? p->libro->titolo  : "?",
                           p->utente ? p->utente->nome    : "?",
                           bp, bs,
                           p->restituito ? "RESTITUITO" : "ATTIVO");
                if(num_prestiti == 0) printf("  Nessun prestito registrato.\n");
                stampa_separatore();
                printf("  Totale: %d\n", num_prestiti);
                break;
            }
        }
    }while(scelta != 0);
}

static void menu_ricerca(void){
    int scelta;
    char q[256];

    do{
        stampa_intestazione("RICERCA");
        printf("  1. Cerca per titolo\n");
        printf("  2. Cerca per autore\n");
        printf("  3. Cerca per genere\n");
        printf("  4. Cerca utente per nome\n");
        printf("  0. Torna al menu principale\n");
        scelta = leggi_intero("Scelta", 0, 4);

        switch(scelta){
            case 1:
                leggi_stringa("Titolo (parziale)", q, sizeof(q));
                libro_cerca_titolo(q);
                break;
            case 2:
                leggi_stringa("Autore (parziale)", q, sizeof(q));
                libro_cerca_autore(q);
                break;
            case 3:
                leggi_stringa("Genere (parziale)", q, sizeof(q));
                libro_cerca_genere(q);
                break;
            case 4:
                leggi_stringa("Nome utente (parziale)", q, sizeof(q));
                Utente *u = utente_cerca_nome(q);
                if (u) {
                    printf("  Trovato: [%d] %s | %s | %s\n",
                           u->id, u->nome, u->email, u->telefono);
                } else {
                    printf("  Nessun utente trovato.\n");
                }
                break;
        }
    } while(scelta != 0);
}

int main(void){
    /* inizializzazione strutture */
    libri_init();
    utenti_init();
    prestiti_init();

    /* directory data (ignora errore se già esiste) */
    system("mkdir -p data");

    /* carica dati */
    carica_tutto();
    sincronizza_id();

    /* controlla scadenze e popola la coda notifiche */
    prestiti_verifica_scadenze();

    /* ---- mostra notifiche in coda all'avvio ---- */
    if (!coda_notifiche_vuota()) {
        stampa_intestazione("NOTIFICHE DI SCADENZA");
        char *msg;
        while ((msg = coda_notifiche_dequeue()) != NULL) {
            printf("  [!] %s\n", msg);
            free(msg);
        }
        stampa_separatore();
        printf("  Premi INVIO per continuare...");
        getchar();
    }

    /* ---- menu principale ---- */
    int scelta;
    do {
        stampa_intestazione("MENU PRINCIPALE");
        printf("  1. Gestione libri\n");
        printf("  2. Gestione utenti\n");
        printf("  3. Gestione prestiti\n");
        printf("  4. Ricerca\n");
        printf("  5. Statistiche\n");
        printf("  0. Esci e salva\n");
        scelta = leggi_intero("Scelta", 0, 5);

        switch (scelta) {
            case 1: menu_libri();            break;
            case 2: menu_utenti();           break;
            case 3: menu_prestiti();         break;
            case 4: menu_ricerca();          break;
            case 5: stampa_statistiche();    break;
        }
    } while (scelta != 0);

    /* salva tutto prima di uscire */
    salva_tutto();

    /* libera memoria */
    prestiti_free();
    utenti_free();
    libri_free();

    printf("\n  Arrivederci!\n\n");
    return EXIT_SUCCESS;
}
