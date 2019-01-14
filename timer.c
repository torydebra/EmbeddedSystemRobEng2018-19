#include "timer.h"
#include "lcd.h"
#include "xc.h"
#include "pwm.h"
#include <stdlib.h>
#include "globalVar.h"

int set_prescaler(int ms, int *tckps) {
    
    //maximum time 2330 seconds with FOSC = 7372800 Hz otherwise we exceed the unsigned long int
    unsigned long int pr = (unsigned long int)(FOSC/4) * (ms/1000.0);
    
    //calculation of the optimal prescaler (TCKPS)w for every value of ms
    while (pr > 65535) {
        pr = pr/8;
        *tckps = *tckps + 1;
        
        if (*tckps == 2) {
            pr = pr/4;
            *tckps = 3;
            
            if (pr > 65535) {
                // ERROR
                exit (-1);
            }
        }
    }
    
    return pr; 
}


/* TIMER 1: for the scheduler */
void tmr1_setup_period(int ms) {
    T1CONbits.TON = 0;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    
    
    int t1tckps = 0;

    PR1 = set_prescaler(ms, &(t1tckps));
    
    T1CONbits.TCKPS = t1tckps;
    IEC0bits.T2IE = 0; //enable interrupt
    T1CONbits.TON = 1;

}



int tmr1_wait_period(){
    
    if (IFS0bits.T1IF == 1) { //check if the timer has expired
        return -1; //ERROR
    }
        
    while(IFS0bits.T1IF == 0){
        // Wait for the flag
    }
    
    IFS0bits.T1IF = 0; //set the flag = 0
    return 0;
}


/** TIMER 2: the one for the timeout mode*/
void tmr2_setup_period(int ms) {
    T2CONbits.TON = 0;
    TMR2 = 0;
    
    int t2tckps = 0;

    PR2 = set_prescaler(ms, &(t2tckps));
    
    T2CONbits.TCKPS = t2tckps;
    IEC0bits.T2IE = 1; //enable interrupt
    T2CONbits.TON = 1;

}

int tmr2_wait_period(){
    
    if (IFS0bits.T2IF == 1) { //check if the timer has expired
        return -1; //ERROR
    }
        
    while(IFS0bits.T2IF == 0){
        // Wait for the flag
    }
    
    IFS0bits.T2IF = 0; //set the flag = 0
    return 0;
}

void tmr2_reset_timer(){
    TMR2 = 0;
}

void tmr2_start_timer(){
    IEC0bits.T2IE = 1;
    TMR2 = 0;
    IFS0bits.T2IF = 0; //set the flag = 0
    T2CONbits.TON = 1;
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt () {
    
    IEC0bits.T2IE = 0;
    
    boardState = STATE_TIMEOUT;
    int n1, n2 = 0;
    refreshPWMvalue(&n1, &n2);
    appliedN1 = 0;
    appliedN2 = 0;
    
    IFS0bits.T2IF = 0; //reset interrupt flag for timer 2
    T2CONbits.TON = 0; //stop the timer
    TMR2 = 0; //reset the timer 
}
