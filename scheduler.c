#include "scheduler.h"
#include "globalVar.h"

void initHeartbeat(Heartbeat* schedInfo){
    int i = 0;
    for (i = 0; i < MAX_TASKS; i++){
        schedInfo[i].n = 0;        
    }

    schedInfo[0].N = 1;
  
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
					break;
				case 2:
					break;
			}
			schedInfo[i].n = 0;
		}
	}
}
