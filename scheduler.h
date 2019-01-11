#ifndef SCHEDULER_H
#define	SCHEDULER_H

#define MAX_TASKS (10)

typedef struct {
	int n;
	int N;
} Heartbeat;

void scheduler();

#endif	/* SCHEDULER_H */
