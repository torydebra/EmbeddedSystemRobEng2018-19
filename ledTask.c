#include <xc.h>
#include "globalVar.h"

void setupLed(){
    // setup LEDs as outputs
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
}

void ledTask() {
    LATBbits.LATB0 = !LATBbits.LATB0;
    
    if (boardState == STATE_TIMEOUT){
        LATBbits.LATB1 = !LATBbits.LATB1;
    } else {
        LATBbits.LATB1 = 0; //switch off led4
    }
}