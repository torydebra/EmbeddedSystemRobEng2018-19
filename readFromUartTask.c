/*
 * File:   readFromUartTask.c
 * Author: tori
 *
 * Created on 10 gennaio 2019, 16.04
 */
#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define MAX_CHAR_READ 25

int processMessage(char* type, char* payload){
    
    int n1, n2, min, max = 0;
    
    if (strcmp(type, "HLREF") == 0){
        
        //if (boardState != SAFE){
            sscanf(payload, "%d,%d", n1, n2);
            
            //if(refreshPWMvalue(n1, n2)){ //if return not zero error
                //send ACK NEG (0)
                //send2pc("MCACK", "REF,0");
            
            //} else {
                send2pc("MCACK", "REF,1");
            //} 
            
            /** TODO */
            // blink led D3 and stop D4
            // state = CONTROL
        //}
    
    } else if (strcmp(type, "HLSAT") == 0){
        
        //sscanf("%d , %d", min, max); //TDODODODOAds
        //if(refreshPWMrange(min,max));
        //send ACK
    
    } else if(strcmp(type, "HLENA") == 0) {
       //if (state == SAFE){
        //refreshPWMvalue(0,0);
        //state = CONTROL;
        //send ACK
        //}
        
    } else {
        // ERRROR
    }

}

int readFromUartTask(void) {
 
    int value = 0;
    char c;
    short int i = 0;
    short int retParse = 0;
    
    for (i=0; i < MAX_CHAR_READ; i++){
        
        int bufVal = readBuf(&bufReceiving);
        if (bufVal != 0) { // first check if there are characters to be read in the buffer 
            value = bufVal;

        } else if (U2STAbits.URXDA == 1) { //notifies if there are characters to be read    
            value = U2RXREG; 
        }
        
        c = sprintf("%c", value); //"convert" into the correspondend ascii
        retParse = parse_byte(pstate, c);
        
        if (retParse == NEW_MESSAGE){
            processMessage(pstate.msg_type, pstate.msg_payload);
        }         
        
    }
   
   return 0;
}

