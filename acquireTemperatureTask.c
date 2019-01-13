#include "acquireTemperatureTask.h"
#include "globalVar.h"
#include "temperature.h"
#include <stdio.h>
#include "lcd.h"

void acquireTemperatureTask (){
    float temp = acqTemp();
    writeBufTemp(temp);
    return;
}