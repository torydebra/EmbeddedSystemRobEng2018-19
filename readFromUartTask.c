/*
 * File:   readFromUartTask.c
 * Author: tori & fafux
 *
 * Created on 10 gennaio 2019, 16.04
 */
#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "uart.h"
#include "globalVar.h"
#include "lcd.h"

#define MAX_CHAR_READ 25
//def of return int of processMessage
#define ERROR 0
#define REF_0 1
#define REF_1 2
#define SAT_0 3
#define SAT_1 4
#define ENA_0 5
#define ENA_1 6

int sendMC(int retProc){

    switch(retProc){
        
        case REF_0 :
            send2pc("MCACK,REF,0");
            break;
        case REF_1 :
            send2pc("MCACK,REF,1");
            break;
        case SAT_0 :
            send2pc("MCACK,SAT,0");
            break;  
        case SAT_1 :
            send2pc("MCACK,SAT,1");
            break;
        case ENA_0 :
            send2pc("MCACK,ENA,0");
            break;    
        case ENA_1 :
            send2pc("MCACK,ENA,1");
            break;
        default:
            break;            
    }
}

int processMessage(char* type, char* payload){
    
    int n1, n2, min, max = 0;
    
    if (strcmp(type, "HLREF") == 0){
        
        if (boardState != STATE_SAFE){
            
            boardState = STATE_CONTROL; //exit timeout mode
            sscanf(payload, "%d,%d", &n1, &n2);     
            if(refreshPWMvalue(n1, n2)){
                //send ACK POS (0)
                return REF_1         
            } else {              
                return REF_0            
            } 
            
            /** TODO qui??? */
            // blink led D3 and stop D4
        }
    
    } else if (strcmp(type, "HLSAT") == 0){
        
        sscanf("%d , %d", min, max); //TDODODODOAds
        if(refreshPWMrange(min,max)){
            return SAT_1;
        } else {
            return SAT_0;
        }
    
    } else if(strcmp(type, "HLENA") == 0) {
        if (state == STATE_SAFE){
        //refreshPWMvalue(0,0);
        //state = CONTROL;
        //send ACK
        }
        return ENA_1;
        
    } else{
        //error
    }

    return ERROR;
}

int readFromUartTask(void) {
 
    int value = 0;
    char c;
    short int i = 0;
    short int retParse = 0;
    short int retProc = 0;
    short int arrived = 0;
    
    for (i=0; i < MAX_CHAR_READ; i++){
        
        arrived = 0;
        int bufVal = readBuf(&bufReceiving);
        if (bufVal != 0) { // first check if there are characters to be read in the buffer
            value = bufVal;
            arrived = 1;

        } else if (U2STAbits.URXDA == 1) { //notifies if there are characters to be read    
            value = U2RXREG; 
            arrived = 1;
        }
             
        if (arrived){
            c = value; //"convert" into the correspondend ascii
            writeLCD(c);
            retParse = parse_byte(&pstate, c);
           
            if (retParse == NEW_MESSAGE){
                writeLCD('K');
                retProc = processMessage(pstate.msg_type, pstate.msg_payload);
                sendMC(retProc);
            } 
        }        
    }
   
   return 0;
}

