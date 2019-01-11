/*
 * File:   acquireTemp.c
 * Author: fafux
 *
 * Created on 10 gennaio 2019, 16.19
 */

#include "xc.h"

int acqTemp() {
    double temp = 0;
    
    if (ADCON1bits.DONE) { // if ADC conversion completed
        temp = 500.0/1023.0*ADCBUF1 - 50.0;
        return temp;
    }
}
