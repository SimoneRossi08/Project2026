#ifndef LIBRI_H
#define LIBRI_H

#define MAX_TITOLO    256
#define MAX_AUTORE    128
#define MAX_GENERE     64
#define MAX_ISBN       20

typedef struct Libro {
    int    id;
    char   titolo[MAX_TITOLO];
    char   autore[MAX_AUTORE];
    char   genere[MAX_GENERE];
    char   isbn[MAX_ISBN];
    int    anno;
    int    copie_totali;
    int    copie_disponibili;
    int    volte_prestato;   /* per le statistiche */
} Libro;

/* catalogo globale */
extern Libro **catalogo;
extern int     num_libri;
extern int     cap_libri;

void   libri_init(void);
void   libri_free(void);

Libro *libro_nuovo(const char *titolo, const char *autore,
                   const char *genere, const char *isbn,
                   int anno, int copie);

int    libro_aggiungi(Libro *l);
int    libro_elimina(int id);
Libro *libro_cerca_id(int id);

/* ricerca con corrispondenza parziale (strstr) */
void   libro_cerca_titolo(const char *q);
void   libro_cerca_autore(const char *q);
void   libro_cerca_genere(const char *q);

void   libro_modifica(int id);
void   libri_stampa_tutti(void);

#endif /* LIBRI_H */
