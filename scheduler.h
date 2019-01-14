#ifndef SCHEDULER_H
#define	SCHEDULER_H

#define MAX_TASKS (4)

typedef struct {
	int n;
	int N;
} Heartbeat;

void initHeartbeat();
void scheduler();

#endif	/* SCHEDULER_H */
