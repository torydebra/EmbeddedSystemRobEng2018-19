#include "globalVar.h"
#include "scheduler.h"
#include "acquireTemperatureTask.h"
#include "averageTemperatureTask.h"
#include "readFromUartTask.h"


void initHeartbeat(){
    int i = 0;
    for (i = 0; i < MAX_TASKS; i++){
        schedInfo[i].n = 0;        
    }

    schedInfo[0].N = 1;
    schedInfo[1].N = 1;
    schedInfo[2].N = 10;
  
}

void scheduler() {
	int i;
	for (i = 0; i < MAX_TASKS; i++) {
		schedInfo[i].n++;
		if (schedInfo[i].n == schedInfo[i].N) {
			switch(i) {
				case 0:
					//readFromUartTask();
					break;
				case 1:
                    acquireTemperatureTask();
					break;
				case 2:
                    averageTemperatureTask();
					break;
			}
			schedInfo[i].n = 0;
		}
	}
}
