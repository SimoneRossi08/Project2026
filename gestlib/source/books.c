#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/books.h"
#include "../lib/utilis.h"

Libro **catalogo   = NULL;
int     num_libri  = 0;
int     cap_libri  = 0;

#define CAP_INIZIALE 16

void libri_init(void){
    cap_libri  = CAP_INIZIALE;
    num_libri  = 0;
    catalogo   = (Libro **)malloc(cap_libri * sizeof(Libro *));
    if (!catalogo) { perror("malloc catalogo"); exit(EXIT_FAILURE); }
}

void libri_free(void){
    int i;
    for (i = 0; i < num_libri; i++) free(catalogo[i]);
    free(catalogo);
    catalogo  = NULL;
    num_libri = cap_libri = 0;
}


static void catalogo_espandi(void){
    cap_libri *= 2;
    catalogo   = (Libro **)realloc(catalogo, cap_libri * sizeof(Libro *));
    if (!catalogo) { perror("realloc catalogo"); exit(EXIT_FAILURE); }
}


Libro *libro_nuovo(const char *titolo, const char *autore,const char *genere, const char *isbn,int anno, int copie){
    Libro *l = (Libro *)calloc(1, sizeof(Libro));
    if (!l) { perror("calloc Libro"); return NULL; }

    l->id                = genera_id();
    strncpy(l->titolo,  titolo,  MAX_TITOLO  - 1);
    strncpy(l->autore,  autore,  MAX_AUTORE  - 1);
    strncpy(l->genere,  genere,  MAX_GENERE  - 1);
    strncpy(l->isbn,    isbn,    MAX_ISBN    - 1);
    l->anno              = anno;
    l->copie_totali      = copie;
    l->copie_disponibili = copie;
    l->volte_prestato    = 0;
    return l;
}

int libro_aggiungi(Libro *l){
    if (!l) return -1;
    if (num_libri >= cap_libri) catalogo_espandi();
    catalogo[num_libri++] = l;
    return l->id;
}

int libro_elimina(int id){
    int i;
    for (i = 0; i < num_libri; i++) {
        if (catalogo[i]->id == id) {
            free(catalogo[i]);
            catalogo[i] = catalogo[--num_libri];
            return 1;
        }
    }
    printf("  [!] Libro con ID %d non trovato.\n", id);
    return 0;
}


Libro *libro_cerca_id(int id){
    int i;
    for (i = 0; i < num_libri; i++)
        if (catalogo[i]->id == id) return catalogo[i];
    return NULL;
}

static void stampa_libro(const Libro *l){
    printf("  [%4d] %-40s | %-25s | %-15s | %d | copie: %d/%d | prestiti: %d\n",
           l->id, l->titolo, l->autore, l->genere,
           l->anno, l->copie_disponibili, l->copie_totali, l->volte_prestato);
}

void libro_cerca_titolo(const char *q){
    int i, trovati = 0;
    printf("\n  Ricerca per titolo: \"%s\"\n", q);
    stampa_separatore();
    for (i = 0; i < num_libri; i++) {
        if (strstr(catalogo[i]->titolo, q)) {
            stampa_libro(catalogo[i]);
            trovati++;
        }
    }
    if (!trovati) printf("  Nessun risultato.\n");
    stampa_separatore();
}

void libro_cerca_autore(const char *q){
    int i, trovati = 0;
    printf("\n  Ricerca per autore: \"%s\"\n", q);
    stampa_separatore();
    for (i = 0; i < num_libri; i++) {
        if (strstr(catalogo[i]->autore, q)) {
            stampa_libro(catalogo[i]);
            trovati++;
        }
    }
    if (!trovati) printf("  Nessun risultato.\n");
    stampa_separatore();
}

void libro_cerca_genere(const char *q){
    int i, trovati = 0;
    printf("\n  Ricerca per genere: \"%s\"\n", q);
    stampa_separatore();
    for (i = 0; i < num_libri; i++) {
        if (strstr(catalogo[i]->genere, q)) {
            stampa_libro(catalogo[i]);
            trovati++;
        }
    }
    if (!trovati) printf("  Nessun risultato.\n");
    stampa_separatore();
}


void libro_modifica(int id){
    Libro *l = libro_cerca_id(id);
    if (!l) { printf("  [!] Libro ID %d non trovato.\n", id); return; }

    char buf[MAX_TITOLO];
    int  n;

    printf("  Modifica libro [%d] — %s\n", l->id, l->titolo);
    printf("  (lascia vuoto per mantenere il valore attuale)\n\n");

    printf("  Titolo   [%s]: ", l->titolo);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (buf[0]) strncpy(l->titolo, buf, MAX_TITOLO - 1);

    printf("  Autore   [%s]: ", l->autore);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (buf[0]) strncpy(l->autore, buf, MAX_AUTORE - 1);

    printf("  Genere   [%s]: ", l->genere);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (buf[0]) strncpy(l->genere, buf, MAX_GENERE - 1);

    printf("  ISBN     [%s]: ", l->isbn);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (buf[0]) strncpy(l->isbn, buf, MAX_ISBN - 1);

    printf("  Anno     [%d]: ", l->anno);
    fgets(buf, sizeof(buf), stdin);
    n = atoi(buf);
    if (n > 0) l->anno = n;

    printf("  Copie totali [%d]: ", l->copie_totali);
    fgets(buf, sizeof(buf), stdin);
    n = atoi(buf);
    if (n > 0) {
        int diff = n - l->copie_totali;
        l->copie_totali = n;
        l->copie_disponibili += diff;
        if (l->copie_disponibili < 0) l->copie_disponibili = 0;
    }

    printf("  [OK] Libro aggiornato.\n");
}


void libri_stampa_tutti(void){
    int i;
    if (num_libri == 0) { printf("  Catalogo vuoto.\n"); return; }
    stampa_separatore();
    printf("  %-6s %-40s %-25s %-15s %-6s %s\n",
           "ID", "Titolo", "Autore", "Genere", "Anno", "Copie disp/tot");
    stampa_separatore();
    for (i = 0; i < num_libri; i++) stampa_libro(catalogo[i]);
    stampa_separatore();
    printf("  Totale libri in catalogo: %d\n", num_libri);
}
