#include <stdio.h>
#include <stdlib.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/InputOutput.h"
#include "../lib/utilis.h"

#define MAX_LIBRI 100
#define MAX_UTENTI 100
#define MAX_PRESTITI 100

void bubbleSort(Libro* libri, int size){
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - 1 - i; j++){
            if(strcmp(libri[j].titolo, libri[j + 1].titolo) > 0){
                Libro temp = libri[j];
                libri[j] = libri[j + 1];
                libri[j + 1] = temp;
            }
        }
    }
}

void statistiche(Libro* libri, int *libri_size){
    int copietotali = 0;
    for (int i = 0; i < *libri_size; i++){
        copietotali += libri[i].copie;
    }

    printf("Totale copie disponibili: %d\n", copietotali);
}