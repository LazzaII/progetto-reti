#include "command.h"

char* commandList() {
    return "Lista dei comandi disponibili:"
           "\t- look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggeto/location in questione.\n"
           "\t- take obj: raccoglie l'oggetto <obj> indicato come parametro."
           "\t- use obj1 [obj2]: permette di utilizzare l'oggeto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggeto <obj2>."
           "\t- objs: rende la lista degli oggetti raccolti."
           "\t- end: termina la partita ed esce dal gioco.";
}

