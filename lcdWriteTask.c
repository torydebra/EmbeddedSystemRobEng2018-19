#include "xc.h"
#include "lcd.h"
#include "globalVar.h"
#include <stdio.h>

void lcdWriteTask(){
    
    char str[16];
    
    switch (boardState){
        case STATE_CONTROL:
            moveCursor(1,5);
            writeCharLCD('C');
            break;
        case STATE_TIMEOUT:
            moveCursor(1,5);
            writeCharLCD('T');
            break;
        case STATE_SAFE:
            moveCursor(1,5);
            writeCharLCD('H');
            break;
        default:
            moveCursor(1,5);
            writeCharLCD('?');
    }
    
    //2nd row
    clearLCD(2,5);
    moveCursor(2,5);
    sprintf(str, "%d,%d", appliedN1, appliedN2);
    writeStringLCD(str);
}
