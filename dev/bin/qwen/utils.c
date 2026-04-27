#include <stdio.h>
#include <stdlib.h>

void bubbleSort(Libro* libri, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (strcmp(libri[j].titolo, libri[j + 1].titolo) > 0) {
                Libro temp = libri[j];
                libri[j] = libri[j + 1];
                libri[j + 1] = temp;
            }
        }
    }
}

void statistiche(Libro* libri, int *libri_size) {
    int total_copies = 0;
    for (int i = 0; i < *libri_size; i++) {
        total_copies += libri[i].copie;
    }

    printf("Totale copie disponibili: %d\n", total_copies);
}