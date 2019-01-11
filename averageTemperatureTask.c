#include "bufferTemp.h"
#include "globalVar.h"

float averageTemp() {
    
    float average = 0.0;
    int i = 0;
    
    for (i=0; i < DIMBUFTEMP; i++){
       average = (average + buff.data[i]);
    }
    average = average/DIMBUFTEMP;
    
    return average;
}
