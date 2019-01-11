#include "uart.h"
#include "xc.h"
#include <stdio.h>
#include <string.h>

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

void setupUART2(){
    // max baud rate 110 kbps with our fcy
    // 57600 baud rate max for our pc software
    U2BRG = 11; // (7372800 / 4) / (16 * 9600) - 1
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U2TX (must be after UARTEN)
}