#include "../../header/support/temperature.h"
#include "xc.h"
#include "../../header/support/lcd.h"

void setupADCtemp() {
    
    // min tad is 154ns
    ADCON3bits.ADCS = 63; // tad at the maximum for circuital problems 
    
    ADCON1bits.ASAM = 1; // auto start sampling
    
    ADCON3bits.SAMC = 15; // samp time = 15tad 
    ADCON1bits.SSRC = 7; // auto end sampling
    
    ADCON2bits.CHPS = 0; // select channel 0
    
    ADCHSbits.CH0SA = 3; // link ch0 to AN3 (temperature)
    ADCHSbits.CH0NA = 0;
    
    ADPCFG = 0xFFFF; // set all input pin as digital mode
    ADPCFGbits.PCFG3 = 0; // set AN3 input in analog
    
    ADCON1bits.ADON = 1;
}

float acqTemp() {
    float temp = 0.0;
    
    if (ADCON1bits.DONE) { // if ADC conversion completed
        temp = 500.0/1023.0*ADCBUF0 - 50.0;
        return temp;
    }
    
    return 0;
}