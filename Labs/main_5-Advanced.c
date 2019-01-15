/*
 * File:   main5-Advanced.c
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
#include <string.h>
#define FOSC 7372800 // Frequency oscillator in Hz
#define FCY 1843200 // Frequency clock (Foscillator/4) in Hz
#define DIMBUF 100 // Maximum buffer dimension
int counter = 0; // DISABILITARE INTERRUPT PERCHE TORI SCASSA IL CAZZOOOOO

typedef struct {
    int data[DIMBUF];
    int head;
    int tail;
} Buffer;

Buffer buf;

void initBuf (Buffer *buffer){
    buffer->head = 0;
    buffer->tail = 0;
}

void writeBuf (Buffer *buffer, int c) {
    if (buffer->tail < DIMBUF) {
        buffer->data[buffer->tail] = c;
        buffer->tail = (buffer->tail + 1) % DIMBUF;
    }
}

int readBuf (Buffer *buffer) {
    if (buffer->tail == buffer->head) {
        return 0; // queue is empty
    }
    else {
        int value = buffer->data[buffer->head];
        buffer->head = (buffer->head + 1) % DIMBUF;
        return value;
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt () {
    
    IFS1bits.U2RXIF = 0; // reset interrupt flag
    int val = U2RXREG;
    writeBuf(&buf, val); // store value in buffer
}

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

void writeLCD(int word, short int line) {
    
    while(SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = word;
    
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
    U2BRG = 11; // (7372800 / 4) / (16 * 9600) - 1
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U2TX (must be after UARTEN)
}

short int UART2toLCD() {
    int value= 0;
    short int flag = 0;
    value = U2RXREG; // read from UART2
    if (value == 13 || value == 10) { // LF=10, CR=13
        clearLCD(1);
    } else {
        writeLCD(value,1); // send to LCD   
        counter ++;
        flag = 1;
    }
    return flag;
}

void shiftCursor(short int line) {
    if ((counter % 16) == 0) { // line is full
        clearLCD(line);
    }
    else {
        if (line == 1){
            while(SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = 0x80 + (counter % 16); // move the cursor at the beginning of the line
        }
        if (line == 2){
            while(SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = 0xC0; // move the cursor at the beginning of the second line
        }
    }
    
}
void writeLCDChar(char *word) {
    int i = 0;
    
    while(SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0xC0;
    
    for (i=0; word[i] != '\0'; i++) {
        while(SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = word[i];
    }
}

short int BUFtoLCD (int bufVal){
    short int flag = 0;
    if (bufVal == 13 || bufVal == 10) { // LF=10, CR=13
        clearLCD(1);
    } else {
        writeLCD(bufVal, 1);
        counter ++;
        flag = 1;
    }
    return flag;
}

int main(void) {
    initBuf(&buf);
    char str[16];
    int bufVal = 0;
    short int flag = 0;
    
    // s5pressed to avoid problem due to continuosly pressure of s5 button 
    short int s5pressed = 1;
    // s6pressed to avoid problem due to continuosly pressure of s6 button 
    short int s6pressed = 1;
    
    TRISEbits.TRISE8 = 1; // button S5 as input
    TRISDbits.TRISD0 = 1; // button S6 as input
    setupLCD();
    setupUART2();
    IEC1bits.U2RXIE = 1; // enable interrupt
    U2STAbits.URXISEL = 11; // interrupt arrives when UART2 receiver is full
    
    while(1) {
        
        if (PORTEbits.RE8 == 0 && s5pressed == 1) {
            U2TXREG = counter; // send back to UART2
        }
        s5pressed = PORTEbits.RE8;
        
        if (PORTDbits.RD0 == 0 && s6pressed == 1) {
            clearLCD(0);
            counter = 0;
        }
        s6pressed = PORTDbits.RD0;
        
        bufVal = readBuf(&buf);
        if (bufVal != 0) { // first check if there are characters to be read in the buffer 
            flag = BUFtoLCD (bufVal);
            
        } else if (U2STAbits.URXDA == 1) { //notifies if there are characters to be read    
            flag = UART2toLCD(); 
        }
        
        if (flag) { // if so, we have to update the character counter 
            shiftCursor(2);
            sprintf(str, "Char Recv: %d", counter);
            writeLCDChar(str);
            shiftCursor(1);
        }
        
        flag = 0;
    }
    
    return 0;
}
