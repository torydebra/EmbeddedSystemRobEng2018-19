#ifndef BUFFER_H
#define	BUFFER_H

#define DIMBUF 100 // Maximum buffer dimension
// 9600 bps == 960 bytes per second, and we read UART buf every 100 ms
// so max bytes to read at each period = 960*0.1 = 96 bytes

typedef struct {
    int data[DIMBUF];
    int head;
    int tail;
} Buffer;

void initBuf (Buffer *buffer);

void writeBuf (Buffer *buffer, int c);

int readBuf (Buffer *buffer, int* value);

int sizeBuf (Buffer *buffer);

#endif	/* BUFFER_H */

