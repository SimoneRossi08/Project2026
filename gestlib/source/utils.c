#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

static int prossimo_id = 1;

int genera_id(void)
{
    return prossimo_id++;
}

/* Permette di sincronizzare l'ID dopo il caricamento da CSV */
void utils_set_prossimo_id(int id){
    if (id > prossimo_id) prossimo_id = id;
}

char *data_to_str(time_t t, char *buf, int len){
    if (t == 0) { snprintf(buf, len, "N/A"); return buf; }
    struct tm *tm_info = localtime(&t);
    strftime(buf, len, "%Y-%m-%d", tm_info);
    return buf;
}

time_t str_to_data(const char *s){
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(tm_info));
    sscanf(s, "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
    tm_info.tm_year -= 1900;
    tm_info.tm_mon  -= 1;
    return mktime(&tm_info);
}

time_t oggi(void){
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    tm_info->tm_hour = 0; tm_info->tm_min = 0; tm_info->tm_sec = 0;
    return mktime(tm_info);
}

time_t oggi_plus_giorni(int giorni){
    time_t t = oggi();
    t += (time_t)giorni * 86400;
    return t;
}

int giorni_mancanti(time_t scadenza){
    double diff = difftime(scadenza, oggi());
    return (int)(diff / 86400);
}


void leggi_stringa(const char *prompt, char *buf, int len){
    printf("  %s: ", prompt);
    fflush(stdout);
    fgets(buf, len, stdin);
    buf[strcspn(buf, "\n")] = 0;
}

int leggi_intero(const char *prompt, int min, int max){
    char buf[64];
    int  val;
    while (1) {
        printf("  %s [%d-%d]: ", prompt, min, max);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        val = atoi(buf);
        if (val >= min && val <= max) return val;
        printf("  [!] Valore non valido. Riprova.\n");
    }
}


static int cmp_titolo(const void *a, const void *b){
    return strcmp((*(Libro **)a)->titolo, (*(Libro **)b)->titolo);
}

static int cmp_autore(const void *a, const void *b){
    return strcmp((*(Libro **)a)->autore, (*(Libro **)b)->autore);
}

void libri_ordina_titolo(void){
    qsort(catalogo, num_libri, sizeof(Libro *), cmp_titolo);
    printf("  [OK] Catalogo ordinato per titolo.\n");
}

void libri_ordina_autore(void){
    qsort(catalogo, num_libri, sizeof(Libro *), cmp_autore);
    printf("  [OK] Catalogo ordinato per autore.\n");
}


void pulisci_schermo(void){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void stampa_separatore(void){
    printf("  %s\n",
           "------------------------------------------------------------------------"
           "------------------------------------");
}

void stampa_intestazione(const char *titolo){
    stampa_separatore();
    printf("   GestBib — %s\n", titolo);
    stampa_separatore();
}

void stampa_barra(const char *etichetta, int valore, int max_valore){
    int barre = (max_valore > 0) ? (valore * 40 / max_valore) : 0;
    printf("  %-25s [", etichetta);
    int i;
    for (i = 0; i < 40; i++) putchar(i < barre ? '|' : ' ');
    printf("] %d\n", valore);
}


void stampa_statistiche(void){
    stampa_intestazione("STATISTICHE");

    /* --- libro più prestato --- */
    Libro *top_libro = NULL;
    int    i;
    for (i = 0; i < num_libri; i++) {
        if (!top_libro || catalogo[i]->volte_prestato > top_libro->volte_prestato)
            top_libro = catalogo[i];
    }
    printf("\n  Libro piu' prestato:\n");
    if (top_libro)
        printf("    [%d] \"%s\" di %s — %d prestiti\n",
               top_libro->id, top_libro->titolo, top_libro->autore,
               top_libro->volte_prestato);
    else
        printf("    Nessun dato.\n");

    /* --- utente con più prestiti --- */
    Utente *top_utente = NULL;
    for (i = 0; i < num_utenti; i++) {
        if (!top_utente || utenti[i]->prestiti_totali > top_utente->prestiti_totali)
            top_utente = utenti[i];
    }
    printf("\n  Utente con piu' prestiti:\n");
    if (top_utente)
        printf("    [%d] %s — %d prestiti totali\n",
               top_utente->id, top_utente->nome, top_utente->prestiti_totali);
    else
        printf("    Nessun dato.\n");

    /* --- tasso di restituzione --- */
    int totali = 0, restituiti = 0;
    for (i = 0; i < num_prestiti; i++) {
        totali++;
        if (tutti_prestiti[i]->restituito) restituiti++;
    }
    printf("\n  Tasso di restituzione: ");
    if (totali > 0)
        printf("%d/%d (%.1f%%)\n", restituiti, totali,
               100.0 * restituiti / totali);
    else
        printf("N/A\n");

#define MAX_GENERI_STAT 32
    typedef struct { char genere[MAX_GENERE]; int cnt; } StatGenere;
    StatGenere sg[MAX_GENERI_STAT];
    int n_sg = 0, j;
    memset(sg, 0, sizeof(sg));

    for(i = 0; i < num_prestiti; i++){
        Prestito *p = tutti_prestiti[i];
        if (!p->libro) continue;
        int trovato = 0;
        for (j = 0; j < n_sg; j++) {
            if (strcmp(sg[j].genere, p->libro->genere) == 0) {
                sg[j].cnt++; trovato = 1; break;
            }
        }
        if (!trovato && n_sg < MAX_GENERI_STAT) {
            strncpy(sg[n_sg].genere, p->libro->genere, MAX_GENERE - 1);
            sg[n_sg].cnt = 1;
            n_sg++;
        }
    }

    /* ordina per cnt decrescente (bubble sort su array piccolo) */
    for (i = 0; i < n_sg - 1; i++) {
        for (j = 0; j < n_sg - i - 1; j++) {
            if (sg[j].cnt < sg[j+1].cnt) {
                StatGenere tmp = sg[j]; sg[j] = sg[j+1]; sg[j+1] = tmp;
            }
        }
    }

    int max_cnt = (n_sg > 0) ? sg[0].cnt : 1;
    printf("\n  Generi piu' richiesti:\n");
    if (n_sg == 0) {
        printf("    Nessun dato.\n");
    } else {
        for (i = 0; i < n_sg; i++)
            stampa_barra(sg[i].genere, sg[i].cnt, max_cnt);
    }

    /* --- distribuzione prestiti per libro (top 10) --- */
    printf("\n  Top libri per numero di prestiti:\n");
    /* copia puntatori e ordina */
    Libro **tmp_cat = NULL;
    if (num_libri > 0) {
        tmp_cat = (Libro **)malloc(num_libri * sizeof(Libro *));
        if (tmp_cat) {
            memcpy(tmp_cat, catalogo, num_libri * sizeof(Libro *));
            /* bubble sort discendente su volte_prestato */
            for (i = 0; i < num_libri - 1; i++) {
                for (j = 0; j < num_libri - i - 1; j++) {
                    if (tmp_cat[j]->volte_prestato < tmp_cat[j+1]->volte_prestato) {
                        Libro *t = tmp_cat[j]; tmp_cat[j] = tmp_cat[j+1]; tmp_cat[j+1] = t;
                    }
                }
            }
            int top = num_libri < 10 ? num_libri : 10;
            int max_v = (top > 0 && tmp_cat[0]->volte_prestato > 0)
                        ? tmp_cat[0]->volte_prestato : 1;
            for (i = 0; i < top; i++) {
                char label[50];
                snprintf(label, sizeof(label), "%.45s", tmp_cat[i]->titolo);
                stampa_barra(label, tmp_cat[i]->volte_prestato, max_v);
            }
            free(tmp_cat);
        }
    }

    stampa_separatore();
}
