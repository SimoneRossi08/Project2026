
# original dir from docx

gestbib/
├── include/
│   ├── libri.h
│   ├── utenti.h
│   ├── prestiti.h
│   ├── file_io.h
│   └── utils.h
├── src/
│   ├── main.c          ← menu e ciclo principale
│   ├── libri.c         ← Create, Read, Update, Delete (CRUD) del catalogo
│   ├── utenti.c        ← gestione anagrafica
│   ├── prestiti.c      ← logica prestiti e scadenze
│   ├── file_io.c       ← lettura/scrittura CSV
│   └── utils.c         ← ordinamento, statistiche, stampa
└── data/
    ├── libri.csv
    ├── utenti.csv
    ├── prestiti.csv
    └── storico.csv
