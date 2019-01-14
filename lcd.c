#include "lcd.h"
#include "timer.h"
#include <xc.h>

void writeCharLCD(char word) {
    
    while(SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = word;

}

void writeStringLCD(char* str){
    int i = 0;
    for (i=0; str[i] != '\0'; i++) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = str[i];
    }
}

void moveCursor(int row, int column){
    
    if (row < 1 || row > 2 || column < 1 || column > 16){
        return ;
    }
    
    if (row == 1){
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = 0x80 + column-1;
    } else{
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = 0xC0 + column-1;
    }
    return;
    
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
    
    writeStringLCD("STA:C TEM:0.0");
    moveCursor(2,1);
    writeStringLCD("RPM:0,0");
}

void clearLCD(short int row) {
    // if row==0 clear both rows
    int j=0;
    if (row == 1 || row == 0) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = 0x80; // move the cursor at the beginning of the line
        for(j=0; j < 16; j++) {
            while(SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = ' '; // clean the screen before writing another word
        }
    }
    if (row == 2 || row == 0) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = 0xC0; // move the cursor at the beginning of the line
        for(j=0; j < 16; j++) {
            while(SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = ' '; // clean the screen before writing another word
        }
    }
    while(SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;
}