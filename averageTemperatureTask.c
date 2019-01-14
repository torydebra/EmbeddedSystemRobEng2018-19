#include "bufferTemp.h"
#include "globalVar.h"
#include "lcd.h"
#include "averageTemperatureTask.h"
#include <stdio.h>
#include "uart.h"

float averageTemp() {
    
    float average = 0.0;
    int i = 0;
    
    for (i=0; i < DIMBUFTEMP; i++){
       average = (average + buff.data[i]);
    }
    average = average/DIMBUFTEMP;
    
    return average;
}

int averageTemperatureTask() {
    
    char avTemp[20];
    float aver = averageTemp();
    sprintf(avTemp, "MCTEM,%4.2f", aver);
    send2pc(avTemp);
    
    sprintf(avTemp, "%4.2f", aver);
    moveCursor(1, 11);
    writeStringLCD(avTemp);
    
    return 0;
}