#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "books.h"

void print_book_info(Book book) {
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("ISBN: %s\n", book.isbn);
}

typedef struct {
    int id;
    char titolo[100];
    char autore[100];
    char genere[50];
    int copieTotali;
    int copieDisponibili;
    int voltePrestato;
} Libro;
struct Prestito;   // forward declaration

typedef struct NodoPrestito {
    struct Prestito *prestito;
    struct NodoPrestito *next;
} NodoPrestito;
typedef struct {
    int id;
    char nome[50];
    char cognome[50];
    char email[100];
    int prestitiStorici;
    NodoPrestito *prestiti;   // testa lista prestiti attivi
} Utente;
typedef struct Prestito {
    int id;
    Libro *libro;
    Utente *utente;
    char dataInizio[11];      // formato YYYY-MM-DD
    char dataScadenza[11];
    int restituito;           // 0 = no, 1 = sì
} Prestito;
typedef struct Notifica {
    char messaggio[200];
    struct Notifica *next;
} Notifica;

typedef struct {
    Notifica *front;
    Notifica *rear;
} CodaNotifiche;
typedef struct {
    Libro **libri;
    int numLibri;
    int capLibri;

    Utente **utenti;
    int numUtenti;
    int capUtenti;

    Prestito **prestiti;
    int numPrestiti;
    int capPrestiti;

    CodaNotifiche notifiche;

    int nextIdLibro;
    int nextIdUtente;
    int nextIdPrestito;
} Biblioteca;
