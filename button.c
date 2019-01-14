#include <xc.h>
#include "globalVar.h"
#include "lcd.h"

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt () {
    
    IFS0bits.INT0IF = 0; //reset interrupt flag for S5 button
    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    IEC1bits.U2RXIE = 0; //disable interrupt for UART buffer
    
    int n1, n2 = 0;
    refreshPWMvalue(&n1, &n2);
    appliedN1 = 0;
    appliedN2 = 0;
    
    boardState = STATE_SAFE;
    
    moveCursor(1,1);
    writeStringLCD("STA:H");
    
    IEC1bits.U2RXIE = 1; //enable interrupt for UART buffer
}

void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt () {
    
    IFS1bits.INT1IF = 0; //reset interrupt flag for S6 button
    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    IEC1bits.U2RXIE = 0; //disable interrupt for UART buffer
    
    int n1, n2 = 0;
    refreshPWMvalue(&n1, &n2);
    appliedN1 = 0;
    appliedN2 = 0;
    
    boardState = STATE_SAFE;
    
    moveCursor(1,1);
    writeStringLCD("STA:H");
    
    IEC1bits.U2RXIE = 1; //enable interrupt for UART buffer
}

void setupButton() {
    IEC0bits.INT0IE = 1; //enable interrupt for S5 button
    IEC1bits.INT1IE = 1; //enable interrupt for S6 button
}