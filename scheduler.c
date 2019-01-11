#include "scheduler.h"

void scheduler() {
	int i;
	for (i = 0; i < MAX_TASKS; i++) {
		schedInfo[i].n++;
		if (schedInfo[i].n == schedInfo[i].N) {
			switch(i) {
				case 0:
					//recRef(); // t=100ms
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
