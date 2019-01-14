#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded. 

#define FOSC 7372800 // Frequency oscillator in Hz

int set_prescaler(int ms, int *tckps);
void tmr1_setup_period(int ms);
int tmr1_wait_period();

void tmr2_setup_period(int ms);
int tmr2_wait_period();
void tmr2_start_timer();
void tmr2_reset_timer();
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt () ;

#endif	/* TIMER_H */

