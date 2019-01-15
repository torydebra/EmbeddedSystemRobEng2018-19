/*
 * File:   main7.c
 * Author: Fabio Fusaro & Davide Torielli
 *
 * Created on 27 novembre 2018, 14.37
 */

// DSPIC30F4011 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FPR = XT                 // Primary Oscillator Mode (XT)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define FOSC 7372800 // Frequency oscillator in Hz
#define FCY 1843200 // Frequency clock (Foscillator/4) in Hz

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

void tmr1_wait_period(){
    
    if (IFS0bits.T1IF == 1) { //check if the timer has expired
        exit (-1); //ERROR
    }
        
    while(IFS0bits.T1IF == 0){
        // Wait for the flag
    }
    
    IFS0bits.T1IF = 0; //set the flag = 0
}

void setupLCD() {
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8-bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    // 1 4 16 64 (PPRE =0)
    SPI1CONbits.SPRE = 6; // 2:1 secondary prescaler
    // 8 7 6 5 4 3 2 1(SPRE = 7)
    SPI1STATbits.SPIEN = 1; // enable SPI
    
    tmr1_setup_period(1000); // wait 1 second for the display to be ready
    tmr1_wait_period();
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

void writeLCD(char *word) {
    int i = 0;
    
    for (i=0; word[i] != '\0'; i++) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = word[i];
    }
}

void setupADC () {
    
    // min tad is 154ns
    ADCON3bits.ADCS = 63; // tad at the maximum for circuital problems 
    
    ADCON1bits.ASAM = 1; // auto start sampling
    
    // sampling rate will be 65772 Hz(16 samc)
    ADCON3bits.SAMC = 16; // samp time = 16tad 
    ADCON1bits.SSRC = 7; // auto end sampling
    
    ADCON2bits.CHPS = 0; // select channel 0
    
    ADCHSbits.CH0SA = 2; // link ch0 to AN2 (potentiometer)
    
    ADPCFG = 65535; // set all input pin as digital mode
    ADPCFGbits.PCFG2 = 0; // set AN2 input in analog
    
    ADCON1bits.ADON = 1;
}

void setupPWM () {
    
    PTPER = 9215; // FCY/(50*presc) - 1, max for PTPER = 2^15
    
    PTCONbits.PTCKPS = 1; // prescaler 1:4
    
    PWMCON1bits.PEN2H = 1; // output pin
    
    PTCONbits.PTEN = 1; // turns on the PWM
}

int main(void) {
    
    char str[16];
    double ptperRatio = 0;
    
    setupADC();
    setupLCD();
    setupPWM();
    
    tmr1_setup_period(500);
    
    while(1) {
        if (ADCON1bits.DONE) { // if ADC conversion completed
            // 0-1023 potentiometer to 10-5 ptperRatio (5% DT = PTPER/10)
            ptperRatio = (ADCBUF0*(-5.0)/1023.0) + 10.0; 
            PDC2 = (int) (PTPER/ptperRatio); // PDC2 from 5%Tpwm to 10%Tpwm
            sprintf(str, "PDC2: %d", PDC2);
            writeLCD(str);
            tmr1_wait_period();
            clearLCD(0);
        }
    }
    
    return 0;
}
