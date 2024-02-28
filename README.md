# Progetto di Reti Informatiche

> Progetto per il corso Reti Informatiche A.A. 23/24 dell'Università di Pisa

Valutazione

> --

Autore

> Francesco Lazzarelli

## Descrizione

Il progetto consiste nella realizzazione di una applicazione client server. Per vedere la richiesta del professore consulatre il documento di analisi.
Il file di documentazione è volutamente corto e conciso (e un pò bruttino) perchè era richiesto dal professore che fosse di massimo due pagine.

## Avvio

Per avviare è necessario scrivere da terminale:

```bash
make 
```

## Comandi

* start *room*
* look [*object* | *location*]
* take *object*
* use *object* [*object*] 
* objs 
* end

## Tema

Era necessario implementare almeno uno scenario e scrivere il codice in maniera che fosse facilmente implementabili altri scenari, per questo motivo ho deciso di implementare una sola scena: "Fuggire dalla prigione"

### Fuggire dalla prigione

Dopo aver fatto il login e aver scelto questo scenario si è davanti ad una scelta, si può partecipare come prigioniero e scappare dalla prigione oppure si può partecipare come secondino corrotto ([vedere client aggiuntivo](#client-aggiuntivo)).

#### Prigioniero

Sei in prigione. Ti trovi sul \+\+letto\+\+, davanti a te c'è uno \+\+scaffale\+\+ e una \+\+finestra\+\+. Mentre dall'altro lato della stanza c'è il tuo compagno di stanza, \+\+McLovin\+\+. 

* **look letto**: sotto il letto hai trovato un \*\*telefono\*\* e delle \*\*monete\*\*

    * \*\*telefono\*\*: Puoi usare il telefono per chiamare il secondino corrotto, client aggiuntivo, se il client non si è connesso non risponde nessuno al telefono. Altrimenti se è presente risponde il client aggiuntivo chiedendo un numero di monete
        
        * look \*\*telefono\*\*: Sul numero di telefono è salvato un solo numero.

        * use \*\*telefono\*\*: Chiamo il numero salvato. Biiiip. Biiiip. 

        * take \*\*telefono\*\*                        

    * \*\*monete\*\*

        * look \*\*monete\*\*: Sono delle monete d'oro.

        * use \*\*monete\*\*: Le monete non si possono usare da sole.

        * take \*\*monete\*\*    

* **look McLovin**: McLovin è un ex chimico, forse può esserti utile. Però ti aiuterà solo a patto di rispondere ad un indovinello.

    * Vediamo se sei stato attento. Sono di andata e di ritorno, chi sono? 

    > rispondendo bene ti dice che usando il telefono e il sapone puoi creare una bomba

* **look scaffale**: nello scaffale c'è una \*\*scatola\*\* e delle \*\*monete\*\*

    // la scatola ha un quiz perchè c'è un lucchetto. La scatola ti da il sapone e delle monete
    // il codice è il continuo di questa sequenza
    // 11
    // 21
    // 1211
    // 111221
    // le monete le posso raccogliere.

* **look finestra**: la finestra ha le \*\*sbarre\*\*.

    // le sbarre non fanno niente almeno che non si usa la bomba craftata con telefono e sapone

#### Client aggiuntivo

Il client aggiuntivo è un secondo partecipante alla escape room che ha il ruolo di secondino corrotto. Ti fa uscire se hai abbastanza soldi, può decidere quanto costa l'uscita da un minimo di 1 e max di 3.

#### Soluzioni Enigmi

1. Quiz di McLovin: tempo di Anna.

2. Lucchetto: 312211

#### Soluzioni Escape Room

Ci sono due possibli soluzioni, per la seconda soluzione è necessario che si connetta il client aggiuntivo

1. Per uscire dalla prigione è necessario far "esplodere" le sbarre alle finestre. Per poterlo fare è necessario creare \*\*bomba\*\* e usarla con \*\*sbarre\*\*.
Per creare \*\*bomba\*\* è necessario usare \*\*telefono\*\* e il \*\*sapone\*\*.

2. Il client aggiuntivo ha lo scopo di Secondino Corrotto che ti può far uscire in cambio di denaro. Usando il \*\*telefono\*\*, se il client è connesso può chiedere un numero di \*\*monete\*\* per far uscire il prigioniero. Se il client principale (prigioniero) ha il numero di monete necessarie la partita finisce.
