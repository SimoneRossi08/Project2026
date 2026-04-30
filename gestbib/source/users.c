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

void registraUtente(Utente *utenti, int *utenti_size){
  if (*utenti_size == MAX_UTENTI){
      printf("Errore: numero massimo di utenti raggiunto.\n");
      return;
  }

  Utente new_utente;
  printf("Inserisci ID dell'utente: ");
  scanf("%d", &new_utente.id);
  printf("Inserisci nome dell'utente: ");
  scanf("%s", new_utente.nome);

  utenti[*utenti_size] = new_utente;
  (*utenti_size)++;
}

void eliminaUtente(Utente *utenti, int *utenti_size){
  if(*utenti_size == 0){
    printf("Errore: nessun utente da eliminare.\n");
    return;
  }

  int id, j;
  printf("Inserisci ID dell'utente da eliminare: ");
  scanf("%d", &id);

  Utente *trovato = NULL;
  for(int i = 0; i < *utenti_size && trovato == NULL; i++){
    if(utenti[i].id == id){
      trovato = &utenti[i];
    }
  }

  if(trovato){
    // Elimina l'utente dalla posizione corrispondente
    for(j = 0; j < (*utenti_size) - 1 && utenti[j].id != id; j++);
    if(j == (*utenti_size) - 1){
      printf("Utente non trovato.\n");
    }
    else{
      *utenti_size = *utenti_size - 1;
      for (int k = j; k < *utenti_size; k++){
        utenti[k] = utenti[k + 1];
      }
    }
  }
  else{
    printf("Utente non trovato.\n");
  }
}

void stampaUtenti(Utente *utenti, int utenti_size){
  for (int i = 0; i < utenti_size; i++){
    printf("ID: %d, Nome: %s\n", utenti[i].id, utenti[i].nome);
  }
}

Utente* cercaUtente(int id, Utente* utenti, int utenti_size){
    for(int i = 0; i < utenti_size; i++){
        if(utenti[i].id == id){
            return &utenti[i];
        }
    }
    return NULL;
}