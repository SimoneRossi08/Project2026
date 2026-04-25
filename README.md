# 📚 GestBib — Sistema di Gestione Biblioteca

## 📖 Descrizione del progetto
**GestBib** è un gestionale da riga di comando sviluppato in C che simula il sistema informatico di una biblioteca pubblica.  
Permette di gestire il catalogo dei libri, gli utenti iscritti e i prestiti attivi.

Il sistema include:
- Persistenza dei dati su file CSV
- Notifiche automatiche per le scadenze

---

## 🧠 Strutture dati utilizzate

### 🔗 Lista collegata
Ogni utente possiede un campo `*prestiti` che punta alla testa di una lista dinamica dei prestiti attivi.  
Ogni nodo contiene:
- un puntatore a `struct Prestito`
- un puntatore `*next` al nodo successivo  

### 📥 Coda (FIFO)
Le notifiche di scadenza sono gestite tramite una struttura `CodaNotifiche`.

- I prestiti scaduti generano notifiche inserite in coda  
- All’avvio del programma, il sistema le estrae e le mostra  

### 📦 Array dinamico
Il catalogo dei libri e l’elenco utenti sono gestiti tramite array dinamici:
- allocati con `malloc`
- ridimensionati con `realloc`

### 🧩 Struct annidate
La struttura `Prestito` contiene:
- un puntatore a `struct Libro`
- un puntatore a `struct Utente`

---

## ⚙️ Funzionalità principali

Il sistema è organizzato tramite un menu interattivo.

### 📚 Gestione libri
- Aggiunta, modifica, eliminazione  
- Ricerca per titolo, autore o genere  
- Supporto a corrispondenze parziali tramite `strstr`

### 👤 Gestione utenti
- Registrazione e cancellazione  
- Generazione automatica di ID univoco  
- Visualizzazione storico prestiti  

### 🔄 Gestione prestiti
- Creazione prestiti (con controllo copie disponibili)  
- Restituzione libri  
- Lista prestiti scaduti  
- Calcolo scadenza: data attuale + 30 giorni (tramite `<time.h>`)

### 📊 Statistiche
- Libro più prestato  
- Utente più attivo  
- Tasso di restituzione  
- Generi più richiesti  

---

## 🎯 Obiettivo del progetto
Simulare un sistema reale di gestione biblioteca, mettendo in pratica:
- gestione della memoria dinamica  
- strutture dati complesse  
- modularità del codice in C  
- gestione file e persistenza  