
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LIBRO{
	char titolo[40];
	char autore[20];
	int anno;
	float prezzo;
};

typedef char* Stringa;
void Compattamento(Stringa buffer, int lunghezza){				
	
    if(lunghezza>0 && buffer[lunghezza-1]=='\n'){
        buffer[lunghezza-1] = '\0';
        lunghezza--;
    }
}

int main(){
	struct LIBRO *libri;
	char libro[40];
	int i, j, n=3, datato, i_costoso, i_datato, lun;
	float costoso;
	

	libri = (struct LIBRO*)malloc(n * sizeof(struct LIBRO));
    
    if (libri == NULL) {
        printf("Errore: allocazione memoria fallita\n");
        return 1;
    }

	printf("=== Lista Libri ===\n");
    for (i = 0; i < n; i++) {
        printf("Libro %d:\n", i + 1);
        printf("  Titolo: %s\n", libri[i].titolo);
        printf("  Autore: %s\n", libri[i].autore);
        printf("  Anno: %d anni\n", libri[i].anno);
        printf("  Prezzo: euro %.2f\n\n", libri[i].prezzo);
    }

    n += 1;
    libri = (struct LIBRO*)realloc(libri, n * sizeof(struct LIBRO));
    
    if (libri == NULL) {
        printf("Errore: ridimensionamento fallito\n");
        return 1;
    }
	
	strcpy(libri[3].titolo, "Orgoglio e pregiudizio");
	strcpy(libri[3].autore, "Jane Austen");
	libri[3].anno = 1813;
	libri[3].prezzo = 9;
	

    for (i = 0; i < n; i++) {
        printf("Libro %d:\n", i + 1);
        printf("  Titolo: %s\n", libri[i].titolo);
        printf("  Autore: %s\n", libri[i].autore);
        printf("  Anno: %d anni\n", libri[i].anno);
        printf("  Prezzo: euro %.2f\n\n", libri[i].prezzo);
    }
    
    //cerchiamo quello più costoso
    costoso = libri[0].prezzo;
    i_costoso = 0;
    for (i = 1; i < n; i++) {
        if( libri[i].prezzo > costoso){
        	costoso = libri[i].prezzo;
			i_costoso = i;	
		}  	
    }
    printf("Il libro piu\' costoso e\': %s\n", libri[i_costoso].titolo);
	

	datato = libri[0].anno;
    i_datato = 0;
    for (i = 1; i < n; i++) {
        if( libri[i].anno < datato){
        	datato = libri[i].anno;
			i_datato = i;	
		}  	
    }
    printf("Il libro piu\' datato e\': %s\n", libri[i_datato].titolo);
    

    printf("\nInserisci il tiolo del libro da eliminare: ");		
	fgets(libro, 50, stdin); 
	lun = strlen(libro);
	Compattamento(libro, lun);
	for (i = 0; i < n; i++) {
		if( !(strcmp(libri[i].titolo,libro)) ){
			for(j=i; j<(n-1); j++){
				strcpy(libri[j].titolo,libri[j+1].titolo);
				strcpy(libri[j].autore,libri[j+1].autore);
				libri[j].anno = libri[j+1].anno;
				libri[j].prezzo = libri[j+1].prezzo;
			}
			n-=1;	
		}  	
    }
    

    libri = (struct LIBRO*)realloc(libri, n * sizeof(struct LIBRO));
    
    if (libri == NULL) {
        printf("Errore: ridimensionamento fallito\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Libro %d:\n", i + 1);
        printf("  Titolo: %s\n", libri[i].titolo);
        printf("  Autore: %s\n", libri[i].autore);
        printf("  Anno: %d anni\n", libri[i].anno);
        printf("  Prezzo: euro %.2f\n\n", libri[i].prezzo);
    }
	
	free(libri);
	libri=NULL;
}
