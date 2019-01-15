#ifndef BUFFERTEMP_H
#define	BUFFERTEMP_H

#define DIMBUFTEMP 10 // Maximum buffer dimension

typedef struct {
    float data[DIMBUFTEMP];
    int index;
} BufferTemp;

void initBufTemp ();

void writeBufTemp (float c);



#endif	/* BUFFERTEMP_H */

