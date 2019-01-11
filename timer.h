#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded. 

#define FOSC 7372800 // Frequency oscillator in Hz

int set_prescaler(int ms, int *tckps);
void tmr1_setup_period(int ms);
void tmr1_wait_period();

#endif	/* TIMER_H */

