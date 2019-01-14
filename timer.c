#include "timer.h"
#include <stdlib.h>

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

void tmr1_setup_period(int ms) {
    T1CONbits.TON = 0;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    
    int t1tckps = 0;

    PR1 = set_prescaler(ms, &(t1tckps));
    
    T1CONbits.TCKPS = t1tckps;
    
    T1CONbits.TON = 1;

}

void tmr2_setup_period(int ms) {
    T2CONbits.TON = 0;
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    
    int t2tckps = 0;

    PR2 = set_prescaler(ms, &(t2tckps));
    
    T2CONbits.TCKPS = t2tckps;
    
    T2CONbits.TON = 1;

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
