/* 
 * File:   readFromUartTask.h
 * Author: tori
 *
 * Created on 13 gennaio 2019, 10.49
 */

#ifndef READFROMUARTTASK_H
#define	READFROMUARTTASK_H

int processMessage(char* type, char* payload);
int readFromUartTask(void);

#endif	/* READFROMUARTTASK_H */

