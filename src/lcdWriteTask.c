#include "xc.h"
#include "../header/support/lcd.h"
#include "../header/support/globalVar.h"
#include <stdio.h>

void lcdWriteTask(){
    
    char str[16];
    moveCursor(1,5);

    switch (boardState){
        case STATE_CONTROL:
            writeCharLCD('C');
            break;
        case STATE_TIMEOUT:
            writeCharLCD('T');
            break;
        case STATE_SAFE:
            writeCharLCD('H');
            break;
        default:
            writeCharLCD('?');
    }
    
    //2nd row
    clearLCD(2,5);
    moveCursor(2,5);
    sprintf(str, "%d,%d", appliedN1, appliedN2);
    writeStringLCD(str);
}
