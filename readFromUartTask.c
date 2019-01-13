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
#include "readFromUartTask.h"
#include "pwm.h"

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
            
            if (boardState == STATE_TIMEOUT){
                boardState = STATE_CONTROL; //exit timeout mode
            }
            sscanf(payload, "%d,%d", &n1, &n2);     
            if(!refreshPWMvalue(n1, n2)){
                appliedN1 = n1;
                appliedN2 = n2;
                
                //send ACK POS (1)
                return REF_1;         
            } else {              
                return REF_0;            
            } 
            
            /** TODO qui??? */
            // blink led D3 and stop D4
        }
    
    } else if (strcmp(type, "HLSAT") == 0){
        
        sscanf(payload, "%d,%d", &min, &max);
        if(!refreshPWMRange(min,max)){
            if (!refreshPWMvalue(appliedN1, appliedN2)){
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
            writeCharLCD(c);
            retParse = parse_byte(&pstate, c);
           
            if (retParse == NEW_MESSAGE){
                writeCharLCD('K');
                retProc = processMessage(pstate.msg_type, pstate.msg_payload);
                sendMC(retProc);
                updateLCD(retProc);
            } 
        }        
    }
   
   return 0;
}

void updateLCD(int retProc){

    //if A new ref arrives, update LCD
    if (retProc == REF_1){
        char strLCD[16];
        sprintf(strLCD, "RPM: %d,%d", appliedN1, appliedN2);
        moveCursor(2,1);
        writeStringLCD(strLCD);
        moveCursor(1,1);
        writeStringLCD("STA:C"); //to possible notify exit from timeout
    } 
}
