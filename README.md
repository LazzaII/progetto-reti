# Progetto di Reti Informatiche

> Progetto per il corso Reti Informatiche A.A. 23/24 dell'Università di Pisa

Valutazione

> --

Autore

> Francesco Lazzarelli

## Descrizione

Il progetto consiste nella realizzazione di una applicazione client-server. Per vedere la richiesta del professore consulatre il documento di analisi.
Il file di documentazione è volutamente corto e conciso (e un pò bruttino) perchè era richiesto dal professore che fosse di massimo due pagine.

## Avvio

Per avviare è necessario scrivere da terminale:

```bash
~$ make
~$ ./exec2024.sh 
```

Il comando make lo usiamo per compilare, mentre l'esecuzione dello script ci permette di avviare con un solo comando il server e i due client.

## Comandi

* start *room* *type*
* look [*object* | *location*]
* take *object*
* use *object* [*object*] 
* objs 
* end
* stop (Solo per il server)

## Tema

Era necessario implementare almeno uno scenario per l'escape room e scrivere il codice in maniera che fossero facilmente implementabili altri scenari, per questo motivo ho deciso di implementare un solo scenario: "Prison Break".

### Prison Break

Dopo aver fatto il login e aver scelto questo scenario si è davanti ad una scelta, si può partecipare come prigioniero e scappare dalla prigione oppure si può partecipare come secondino corrotto ([vedere client aggiuntivo](#client-aggiuntivo)).

#### Prigioniero

Sei in prigione. Ti trovi sul \+\+letto\+\+, davanti a te c'è uno \+\+scaffale\+\+ e una \+\+finestra\+\+. Mentre dall'altro lato della stanza c'è il tuo compagno di stanza, \*\*McLovin\*\*. 

* **look letto**: sotto il letto hai trovato un \*\*telefono\*\* e delle \*\*monete1\*\*

    * telefono: Puoi usare il telefono per chiamare il secondino corrotto, client aggiuntivo, se il client non si è connesso non risponde nessuno al telefono. Altrimenti se è presente risponde il client aggiuntivo chiedendo un numero di monete
        
        * look telefono: Sul numero di telefono è salvato un solo numero.

        * use telefono: Chiamo il numero salvato. Biiiip. Biiiip. (Qui il gioco cambia a seconda della presenza del secondino corrotto)

        * take telefono                        

    * monete1

        * look monete1: Sono delle monete d'oro.

        * use monete1: É uscita testa

        * take monete1   

* **look McLovin**: McLovin è un ex chimico, forse può esserti utile. Però ti aiuterà solo a patto di rispondere ad un indovinello.

    * Vediamo se sei stato attento. Sono di andata e di ritorno, chi sono? 

        Costruire una bomba è molto facile, hai provato con il t----on- il s--o--?

* **look scaffale**: nello scaffale c'è una \*\*scatola\*\* e delle \*\*monete2\*\*.

    * look scatola: La scatola è bloccata da un lucchetto a codice. (use scatola) Il codice del lucchetto è il continuo di questa sequenza
        11
        21
        1211
        111221

        dentro la scatola c'è del \*\*sapone\*\* e altre \*\*monete3\*\*.

        * sapone

            * use sapone: Ancora non è il momento di lavarsi.

            * take sapone (implicito)

            * use sapone telefono (o use telefono sapone): Hai smontato il telefono è hai preso la batteria. Tramite la batteria e il sapone hai costruito una \*\*bomba\*\*

                * take bomba (implicito)

                * use bomba sbarre (o use sbarre bomba): Complimenti sei riuscito ad evadere!

        * monete3

            * look monete3: Sono delle monete d'oro.

            * use monete3: É uscita croce.

            * take monete3 (implicito)

    * monete2

        * look monete2: Sono delle monete d'oro.

        * use monete2: É uscita croce.

        * take monete2

* **look finestra**: la finestra ha le \*\*sbarre\*\*.

    * take sbarre: non si possono prendere

    * use bomba sbarre (o use sbarre bomba): Complimenti sei riuscito ad evadere!

#### Client aggiuntivo

Il client aggiuntivo è un secondo partecipante alla escape room che ha il ruolo di secondino corrotto. Ti fa uscire se hai abbastanza soldi, può decidere quanto costa l'uscita da un minimo di 1 e max di 3.

Il client aggiuntivo viene inserito nella prima partita disponibile.

#### Soluzioni Enigmi

1. Quiz di McLovin: Anna.

2. Lucchetto: 312211

#### Soluzioni Escape Room

Ci sono due possibli soluzioni, per la seconda soluzione è necessario che si connetta il client aggiuntivo

1. Per uscire dalla prigione è necessario far "esplodere" le sbarre alla finestra. Per poterlo fare è necessario creare \*\*bomba\*\* e usarla con \*\*sbarre\*\*.
Per creare \*\*bomba\*\* è necessario usare \*\*telefono\*\* e il \*\*sapone\*\*.

2. Il client aggiuntivo ha lo scopo di Secondino Corrotto che ti può far uscire in cambio di denaro. Usando il \*\*telefono\*\*, se il client è connesso può chiedere un numero di \*\*monete\*\* per far uscire il prigioniero. Se il client principale (prigioniero) ha il numero di monete necessarie la partita finisce.
