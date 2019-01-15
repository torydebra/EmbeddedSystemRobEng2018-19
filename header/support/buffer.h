#ifndef BUFFER_H
#define	BUFFER_H

#define DIMBUF 100 // Maximum buffer dimension

typedef struct {
    int data[DIMBUF];
    int head;
    int tail;
} Buffer;

void initBuf (Buffer *buffer);

void writeBuf (Buffer *buffer, int c);

int readBuf (Buffer *buffer, int* value);


#endif	/* BUFFER_H */

