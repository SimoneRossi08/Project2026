#include <stdio.h>
#include <stdlib.h>
#include "../lib/books.h"
#include "../lib/users.h"
#include "../lib/lending.h"
#include "../lib/notify.h"
#include "../lib/utilis.h"
#include "../lib/InputOutput.h"

int main(void){
    Catalogo catalogo;
    Anagrafica anagrafica;
    CodaNotifiche coda;

    inizializzaCatalogo(&catalogo);
    inizializzaAnagrafica(&anagrafica);
    inizializzaCoda(&coda);

    caricaLibri(&catalogo);
    caricaUtenti(&anagrafica);
    caricaPrestiti(&catalogo, &anagrafica, &coda);

    mostraNotifiche(&coda);

    int opt;
    do{
        printf("\n=== GestBib ===\n");
        printf("(1) Aggiungi libro\n");
        printf("(2) Modifica libro\n");
        printf("(3) Elimina libro\n");
        printf("(4) Cerca libri\n");
        printf("(5) Registra utente\n");
        printf("(6) Elimina utente\n");
        printf("(7) Stampa utenti\n");
        printf("(8) Storico prestiti utente\n");
        printf("(9) Crea prestito\n");
        printf("(10) Restituisci prestito\n");
        printf("(11) Mostra prestiti scaduti\n");
        printf("(12) Statistiche\n");
        printf("(0) Esci\n");
        printf("Scelta: \n");
        scanf("%d", &opt);

        switch(opt){
            case 1:  
                aggiungiLibro(&catalogo);
                break;
            case 2: 
                modificaLibro(&catalogo);
                break;
            case 3: 
                eliminaLibro(&catalogo, &anagrafica); 
                break;
            case 4:  
                cercaLibri(&catalogo); 
                break;
            case 5: 
                registraUtente(&anagrafica);
                break;
            case 6: 
                eliminaUtente(&anagrafica); 
                break;
            case 7:  
                stampaUtenti(&anagrafica); 
                break;
            case 8:  
                mostraStorico(&anagrafica); 
                break;
            case 9:  
                creaPrestito(&catalogo, &anagrafica); 
                break;
            case 10: 
                restituisciPrestito(&catalogo, &anagrafica); 
                break;
            case 11: 
                mostraScaduti(&anagrafica); 
                break;
            case 12:
                mostraStatistiche(&catalogo, &anagrafica);
                break;
            case 0: 
                printf("Uscita.\n"); 
                break;
            default:
                printf("Scelta non valida.\n");
                break;
        }
    }while(opt!=0);

    salvaLibri(&catalogo);
    salvaUtenti(&anagrafica);
    salvaPrestiti(&anagrafica);

    liberaCatalogo(&catalogo);
    liberaAnagrafica(&anagrafica);
    liberaCoda(&coda);

    return 0;
}