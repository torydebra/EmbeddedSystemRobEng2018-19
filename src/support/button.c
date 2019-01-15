#include <xc.h>
#include "../header/support/globalVar.h"
#include "../header/support/lcd.h"
#include "../header/support/pwm.h"
#include "../header/support/button.h"

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt () {
    
    interruptManage();
}

void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt () {
    
    interruptManage();
    
}

void setupButton() {
    IFS0bits.INT0IF = 0; //reset interrupt flag for S5 button
    IFS1bits.INT1IF = 0; //reset interrupt flag for S6 button
    IEC0bits.INT0IE = 1; //enable interrupt for S5 button
    IEC1bits.INT1IE = 1; //enable interrupt for S6 button
}

void interruptManage(){

    IEC0bits.INT0IE = 0; //disable interrupt for S5 button
    IEC1bits.INT1IE = 0; //disable interrupt for S6 button
    IFS0bits.INT0IF = 0; //reset interrupt flag for S5 button
    IFS1bits.INT1IF = 0; //reset interrupt flag for S6 button
    
    TMR2 = 0;
    T2CONbits.TON = 0;
    IEC0bits.T2IE = 0; //disable interrupt for timeout mode
    IFS0bits.T2IF = 0; //set the flag = 0
    
    int n1, n2 = 0;
    refreshPWMvalue(&n1, &n2);
    appliedN1 = 0;
    appliedN2 = 0;
    
    boardState = STATE_SAFE; 
}