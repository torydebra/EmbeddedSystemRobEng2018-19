#ifndef UART_H
#define	UART_H

int send2pc(char* msg);
void setupUART2();
void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt ();

#endif	/* UART_H */

