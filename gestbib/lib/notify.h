#ifndef NOTIFY_H
#define NOTIFY_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NodoNotifica {
    char messaggio[256];
    struct NodoNotifica* next;
} NodoNotifica;

typedef struct CodaNotifiche {
    NodoNotifica* testa;
    NodoNotifica* coda;
} CodaNotifiche;

void inizializzaCoda(CodaNotifiche* coda);
void inserisciNotifica(CodaNotifiche* coda, const char* messaggio);
void mostraNotifiche(CodaNotifiche* coda);
void liberaCoda(CodaNotifiche* coda);

#endif