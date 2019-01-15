/*
 * File:   readFromUartTask.c
 * Author: tori & fafux
 *
 * Created on 10 gennaio 2019, 16.04
 */
#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "../header/support/parser.h"
#include "../header/support/uart.h"
#include "../header/support/globalVar.h"
#include "../header/support/lcd.h"
#include "../header/readFromUartTask.h"
#include "../header/support/pwm.h"
#include "../header/support/timer.h"

int readFromUartTask(void) {
 
    int value = 0;
    char c;
    short int i = 0;
    short int retParse = 0;
    short int retProc = 0;
    short int arrived = 0;
    short int retBuf = 0;
    
    for (i=0; i < MAX_CHAR_READ; i++){
        
        arrived = 0;
        retBuf = readBuf(&bufReceiving, &value);
        if (retBuf != -1) { // first check if there are characters to be read in the buffer
            arrived = 1;
        } else if (U2STAbits.URXDA == 1) { //notifies if there are characters to be read 
            value = U2RXREG; 
            arrived = 1;
        }       
        if (arrived){
            c = value; //"convert" into the correspondend ascii
            retParse = parse_byte(&pstate, c);
           
            if (retParse == NEW_MESSAGE){
                retProc = processMessage(pstate.msg_type, pstate.msg_payload);
                sendMC_enableInterrupt(retProc);
                
                
            } 
        }        
    } 
   return 0;
}



int processMessage(char* type, char* payload){
    
    int n1, n2, min, max = 0;
    
    if (strcmp(type, "HLREF") == 0){
 
        if (boardState != STATE_SAFE){
            
            if (boardState == STATE_TIMEOUT){         
                boardState = STATE_CONTROL; //exit timeout mode
            }
            sscanf(payload, "%d,%d", &n1, &n2);     
            if(!refreshPWMvalue(&n1, &n2)){
                appliedN1 = n1;
                appliedN2 = n2;
                
                return REF_1;  //send ACK POS (1)        
            } else {              
                return REF_0;            
            } 
        }
        return REF_0; //safe mode does not accept new references
    
    } else if (strcmp(type, "HLSAT") == 0){
        
        sscanf(payload, "%d,%d", &min, &max);
        if(!refreshPWMRange(min,max)){
            if (!refreshPWMvalue(&appliedN1, &appliedN2)){
                return SAT_1;
            } 
        } 
        return SAT_0; //error
          
    } else if(strcmp(type, "HLENA") == 0) {
        if (boardState == STATE_SAFE){
            
            boardState = STATE_CONTROL;
            return ENA_1;
        } else {
            return ENA_0;
        } 
    } else{
        //error
    }

    return ERROR;
}

void sendMC_enableInterrupt(short int retProc){

    switch(retProc){
        
        case REF_0 :
            send2pc("MCACK,REF,0");
            break;
        case REF_1 :
            send2pc("MCACK,REF,1");
            tmr2_start_timer(); //reset timer because ref is arrived
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
            //re-enable interrupts
            tmr2_start_timer(); //reenable interrupts for timeout mode
            setupButton(); //reenable interrupts for safe mode
            break;
        default:
            break;            
    }
    return;
}