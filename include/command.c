#include "command.h"

void commandSwitcher(int socket, char *message, char* type)
{
    struct mex substringed_mex = substringMessage(message); 

    if(substringed_mex.ok == true) {


    }
    /* se il messaggio è stao inviato con un formato non accettato gli rispondiamo */
    else {

    }
}
