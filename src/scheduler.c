#include "../header/support/globalVar.h"
#include "../header/scheduler.h"
#include "../header/acquireTemperatureTask.h"
#include "../header/averageTemperatureTask.h"
#include "../header/readFromUartTask.h"
#include "../header/sendFBKTask.h"
#include "../header/ledTask.h"
#include "../header/lcdWriteTask.h"


void initHeartbeat(){
    int i = 0;
    for (i = 0; i < MAX_TASKS; i++){
        schedInfo[i].n = 0;        
    }

    schedInfo[0].N = 1; //readFroumUArtTask
    schedInfo[1].N = 1; //acqure temp
    schedInfo[2].N = 10;// average temp & send to pc
    schedInfo[3].N = 2; //fbk message to pc
    schedInfo[4].N = 10; //leds blinking
    schedInfo[5].N = 1; //update lcd STA & RMP
   
}

void scheduler() {
	int i;
	for (i = 0; i < MAX_TASKS; i++) {
		schedInfo[i].n++;
		if (schedInfo[i].n == schedInfo[i].N) {
			switch(i) {
				case 0:
					readFromUartTask();
					break;
				case 1:
                    acquireTemperatureTask();
					break;
				case 2:
                    averageTemperatureTask();
					break;
                case 3:
                    sendFBKTask();
                    break;
                case 4:
                    ledTask();
                    break;
                case 5:
                    lcdWriteTask();
                    
			}
			schedInfo[i].n = 0;
		}
	}
}
