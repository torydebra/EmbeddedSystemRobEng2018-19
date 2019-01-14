/* 
 * File:   readFromUartTask.h
 * Author: tori & fafux
 *
 * Created on 13 gennaio 2019, 10.49
 */

#ifndef READFROMUARTTASK_H
#define	READFROMUARTTASK_H

#define MAX_CHAR_READ 25
//def of return int of processMessage
#define ERROR 0
#define REF_0 1
#define REF_1 2
#define SAT_0 3
#define SAT_1 4
#define ENA_0 5
#define ENA_1 6

int processMessage(char* type, char* payload);
int readFromUartTask(void);
void sendMC(short int retProc);

#endif	/* READFROMUARTTASK_H */

