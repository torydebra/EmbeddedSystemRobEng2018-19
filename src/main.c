
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
#include "../header/support/temperature.h"
#include "../header/support/parser.h"
#include "../header/scheduler.h"
#include "../header/support/buffer.h"
#include "../header/support/bufferTemp.h"
#include "../header/support/button.h"
#include "../header/ledTask.h"
#include "../header/support/lcd.h"
#include "../header/support/timer.h"
#include "../header/support/uart.h"
#include "../header/support/pwm.h"

#define STATE_CONTROL 0
#define STATE_TIMEOUT 1
#define STATE_SAFE 2
#define MAX_SAFE_VEL 8000
#define MIN_SAFE_VEL -8000

Heartbeat schedInfo[MAX_TASKS];
Buffer bufReceiving;
BufferTemp buff;
Parser_state pstate;
short int boardState = STATE_CONTROL;
int maxRPM = MAX_SAFE_VEL;
int minRPM = MIN_SAFE_VEL;
int appliedN1 = 0;
int appliedN2 = 0;

int main(void) {
    //initialize data structures
    initBuf(&bufReceiving);
    initBufTemp();
    setupParser();
    initHeartbeat();
    
    //initialize peripherals
    setupUART2();
    setupADCtemp();
    setupLCD();
    setupButton();
    setupLed();
    setupPWM();
    
    //set timer for scheduler
    int heartbeat_time = 100;
    tmr1_setup_period(heartbeat_time);
    
    //set timer for timeout mode
    tmr2_setup_period(5000);
    
    // main loop
    while (1) {
        
        scheduler();
   
        
       if(tmr1_wait_period()){
            writeStringLCD("hb expired!");
            while(1);
       }
    }   
    
    return 0;
}
