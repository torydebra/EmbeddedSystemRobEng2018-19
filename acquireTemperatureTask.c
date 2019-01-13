#include "acquireTemperatureTask.h"
#include "globalVar.h"
#include "temperature.h"
#include <stdio.h>
#include "lcd.h"

void acquireTemperatureTask (){
    float temp = acqTemp();
    moveCursor(1,1);
    char c[10];
    sprintf(c, "%.2f", &temp);
    writeStringLCD(c);
    writeBufTemp(temp);
    return;
}