#ifndef FILE_IO_H
#define FILE_IO_H

#define FILE_LIBRI    "data/libri.csv"
#define FILE_UTENTI   "data/utenti.csv"
#define FILE_PRESTITI "data/prestiti.csv"
#define FILE_STORICO  "data/storico.csv"

/* Caricamento all'avvio */
int carica_libri(void);
int carica_utenti(void);
int carica_prestiti(void);
int carica_storico(void);

/* Salvataggio alla chiusura */
int salva_libri(void);
int salva_utenti(void);
int salva_prestiti(void);
int salva_storico(void);

/* Wrapper: carica tutto / salva tutto */
void carica_tutto(void);
void salva_tutto(void);

/* utility CSV */
void csv_escape(const char *src, char *dst, int dst_len);

#endif /* FILE_IO_H */
