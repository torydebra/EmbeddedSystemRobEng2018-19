#ifndef BUFFERTEMP_H
#define	BUFFERTEMP_H

#define DIMBUFTEMP 10 // Maximum buffer dimension

typedef struct {
    float data[DIMBUFTEMP];
    int index;
} BufferTemp;

void initBuf (BufferTemp *buffer);

void writeBufTemp (BufferTemp *buffer, float c);



#endif	/* BUFFERTEMP_H */

