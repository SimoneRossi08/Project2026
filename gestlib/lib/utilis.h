#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/* ---- date helpers ---- */
char  *data_to_str(time_t t, char *buf, int len);   /* "YYYY-MM-DD" */
time_t str_to_data(const char *s);
time_t oggi(void);
time_t oggi_plus_giorni(int giorni);
int    giorni_mancanti(time_t scadenza);

/* ---- input ---- */
void   leggi_stringa(const char *prompt, char *buf, int len);
int    leggi_intero(const char *prompt, int min, int max);

/* ---- ordinamento catalogo ---- */
void   libri_ordina_titolo(void);
void   libri_ordina_autore(void);

/* ---- statistiche e barre ---- */
void   stampa_statistiche(void);
void   stampa_barra(const char *etichetta, int valore, int max_valore);

/* ---- miscellanea ---- */
void   pulisci_schermo(void);
void   stampa_separatore(void);
void   stampa_intestazione(const char *titolo);
int    genera_id(void);     /* ID univoco incrementale globale */

#endif /* UTILS_H */
