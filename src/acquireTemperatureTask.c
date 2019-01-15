#include "../header/acquireTemperatureTask.h"
#include "../header/support/globalVar.h"
#include "../header/support/temperature.h"
#include <stdio.h>
#include "../header/support/lcd.h"

void acquireTemperatureTask (){
    float temp = acqTemp();
    writeBufTemp(temp);
    return;
}