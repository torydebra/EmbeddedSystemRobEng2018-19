/*
 * File:   main5-1.c
 * Author: Fabio Fusaro & Davide Torielli
 *
 * Created on 13 novembre 2018, 14.37
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

void writeLCD(int word) {
    
    while(SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = word;
    
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

void setupUART2(){
    // max baud rate 110 kbps with our fcy
    // 57600 baud rate max for our pc software
    U2BRG = 1; // (7372800 / 4) / (16 * 57600) - 1
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U2TX (must be after UARTEN)
}

int main(void) {

    int value = 0;
    
    setupLCD();
    setupUART2();
    while(1) {
        if (U2STAbits.URXDA == 1) {//notifies if there are characters to be read
            value = U2RXREG; // read from UART2
            writeLCD(value); // send to LCD
            U2TXREG = value; // send back to UART2
        }
    }
    
    return 0;
}
