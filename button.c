#include <xc.h>
#include "globalVar.h"
#include "lcd.h"
#include "pwm.h"
#include "button.h"

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt () {
    
    IFS0bits.INT0IF = 0; //reset interrupt flag for S5 button
    interruptManage();
}

void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt () {
    
    IFS1bits.INT1IF = 0; //reset interrupt flag for S6 button
    interruptManage();
    
}

void setupButton() {
    IEC0bits.INT0IE = 1; //enable interrupt for S5 button
    IEC1bits.INT1IE = 1; //enable interrupt for S6 button
}

void interruptManage(){

    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    
    TMR2 = 0;
    T2CONbits.TON = 0;
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    IFS0bits.T2IF = 0; //set the flag = 0
//    IEC1bits.U2RXIE = 0; //disable interrupt for UART buffer
    
    int n1, n2 = 0;
    refreshPWMvalue(&n1, &n2);
    appliedN1 = 0;
    appliedN2 = 0;
    
    boardState = STATE_SAFE;
    
//    IEC1bits.U2RXIE = 1; //enable interrupt for UART buffer
}