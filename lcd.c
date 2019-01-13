#include "lcd.h"
#include "timer.h"

void writeLCD(char word) {
    //int i = 0;
    //for (i=0; word[i] != '\0'; i++) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = word;
    //}
}

void setupLCD() {
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8-bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    // 1 4 16 64 (PPRE =0)
    SPI1CONbits.SPRE = 0; //8:1 secondary prescaler
    // 8 7 6 5 (SPRE = 3)
    SPI1STATbits.SPIEN = 1; // enable SPI
    
    tmr1_setup_period(1000); // wait 1 second for the display to be ready
    tmr1_wait_period();
}