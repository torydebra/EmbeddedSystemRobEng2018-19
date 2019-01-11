#include "buffer.h"
#include <stdio.h>

int send2pc(char* type, char* payload){
    
    if (strlen(type) + strlen(payload) > 30){
        return -1;
    }
    
    char toSend[30];
    
    sprintf(toSend, "$%s,%s*", type, payload);
    int i = 0;
    for (i=0; i<strlen(toSend); i++){
        if (!U2STAbits.UTXBF){
            U2TXREG = toSend[i];
        }       
    }
     
    return 0;
}
