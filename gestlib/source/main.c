#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int opt;
    
    do{
        printf("---MENU---\n");
        printf("Inserire un'opzione:\n");
        printf("1. Gestione Libri\n");
        printf("2. Gestione Utenti\n");
        printf("3. Gestione Prestiti\n");
        printf("4. Statistiche\n");
        printf("5. Uscita\n");
        scanf("%d", &opt);

        switch(opt){
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                printf("Uscita dal programma.\n");
                return 0;
            default:
                printf("Errore! Opzione non valida.\n");
        }
    }while(opt!=5);

    return 0;
}