/*
 * File:   main.c
 * Author: Fabio Fusaro & Davide Torielli & Suman Pal
 *
 * Created on 16 ottobre 2018, 14.37
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
#define FOSC 7372800 // Frequency oscillator in Hz

void tmr2_wait_ms(int ms) {
    
    if (IFS0bits.T2IF == 1) { //check if the timer has expired
        LATBbits.LATB1 = 1 - LATBbits.LATB1; //turn on the LED as error
    }
    
    T2CONbits.TON = 0;
    TMR2 = 0;
    IFS0 = 0;
    
    T2CONbits.TCKPS = 0;
    
    //maximum time 2330 seconds with FOSC = 7372800 Hz otherwise we exceed the unsigned long int
    unsigned long int pr2 = (unsigned long int)(FOSC/4) * (ms/1000.0);
    
    //calculation of the optimal prescaler (TCKPS)w for every value of ms
    while (pr2 > 65535) {
        pr2 = pr2/8;
        T2CONbits.TCKPS += 1;
        
        if (T2CONbits.TCKPS == 2) {
            pr2 = pr2/4;
            T2CONbits.TCKPS = 3;
            
            if (pr2 > 65535) {
                // ERROR
                exit (-1);
            }
        }
        
    }
    
    PR2 = pr2;
    
    // Start the timer
    T2CONbits.TON = 1;
    
    while(IFS0bits.T2IF == 0){
        // Wait for the flag
    }
    
    IFS0bits.T2IF = 0; //set the flag = 0
    
    T2CONbits.TON = 0; //end the timer
    TMR2 = 0; //reset the timer
}

int main(void) {
    
    TRISBbits.TRISB0 = 0; //set the pin as output (LED pin)
    LATBbits.LATB0 = 0; //initialization of the LED bit
    TRISBbits.TRISB1 = 0; //set the pin as output (other LED pin)
    LATBbits.LATB1 = 0; //initialization of the other LED bit
    
    while (1) {
        
        LATBbits.LATB0 = 1 - LATBbits.LATB0; //swap the turn ON/OFF of the LED
        
        tmr2_wait_ms(1000);
        LATBbits.LATB0 = 1 - LATBbits.LATB0; //swap the turn ON/OFF of the LED
        
        tmr2_wait_ms(5000);
        LATBbits.LATB0 = 1 - LATBbits.LATB0; //swap the turn ON/OFF of the LED
        
        tmr2_wait_ms(500);
        LATBbits.LATB0 = 1 - LATBbits.LATB0; //swap the turn ON/OFF of the LED
        
        tmr2_wait_ms(1000); //wait for checking the turn OFF of the LED
    }
    
    return 0;
}


