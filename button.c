#include <xc.h>
#include "globalVar.h"

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt () {
    
    IFS0bits.INT0IF = 0; //reset interrupt flag for S5 button
    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    
    appliedN1 = 0;
    appliedN2 = 0;
    
    refreshPWMvalue(&appliedN1, &appliedN1);
    
    boardState = STATE_SAFE;
}

void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt () {
    
    IFS1bits.INT1IF = 0; //reset interrupt flag for S6 button
    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    
    appliedN1 = 0;
    appliedN2 = 0;
    
    refreshPWMvalue(&appliedN1, &appliedN1);
    
    boardState = STATE_SAFE;
}