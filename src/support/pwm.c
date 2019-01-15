/*
 * File:   refreshPWMtask.c
 * Author: tori & fafux
 *
 * Created on 11 gennaio 2019, 15.16
 */

#include "xc.h"
#include "../../header/support/pwm.h"
#include "../../header/support/globalVar.h"

void setupPWM () {
    
    PTPER = 1843; // FCY/(1000*presc) - 1, max for PTPER = 2^15
    
    PTCONbits.PTCKPS = 0; // prescaler 1:1
    
    PWMCON1bits.PEN2H = 1; // output pin
    
    PWMCON1bits.PEN3H = 1;
    
    PTCONbits.PTEN = 1; // turns on the PWM
    
    refreshPWMvalue(&appliedN1,&appliedN2);
}

int saturateRPM(int n) {
    
    if (n > maxRPM) {
        n = maxRPM;
    } else if (n < minRPM) {
        n = minRPM;
    }
    
    return n;
}

int refreshPWMvalue(int* n1, int* n2) {
    
    long int ptper = PTPER; //need long to overflow
    *n1 = saturateRPM(*n1);
    *n2 = saturateRPM(*n2);
    
    PDC2 = ((( *n1) - MIN_ABS) *ptper*2)/(MAX_ABS - MIN_ABS);
    PDC3 = (( (*n2) - MIN_ABS) *ptper*2)/(MAX_ABS - MIN_ABS);
    
    return 0;
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

short int refreshPWMRange(int min, int max) {
    
    if (max < min){
        return 1;
    } else if (min < MIN_SAFE_VEL || max > MAX_SAFE_VEL) {
        return 1;
    } else if (sign(min)*sign(max) > 0){
        return 1;
    }

    minRPM = min;
    maxRPM = max;
    return 0;
}