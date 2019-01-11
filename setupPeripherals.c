#include "setupPeripherals.h"


void setupUART2(){
    // max baud rate 110 kbps with our fcy
    // 57600 baud rate max for our pc software
    U2BRG = 1; // (7372800 / 4) / (16 * 57600) - 1
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U2TX (must be after UARTEN)
}
