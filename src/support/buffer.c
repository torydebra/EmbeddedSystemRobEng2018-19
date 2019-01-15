/*
 * File:   buffer.c
 * Author: tori & fafux
 *
 * Created on 11 gennaio 2019, 10.04
 */
#include "xc.h"
#include "buffer.h"

void initBuf (Buffer *buffer){
    buffer->head = 0;
    buffer->tail = 0;
}

void writeBuf (Buffer *buffer, int c) {
    if (buffer->tail < DIMBUF) {
        buffer->data[buffer->tail] = c;
        buffer->tail = (buffer->tail + 1) % DIMBUF;
    }
}

int readBuf (Buffer *buffer, int* value) {
    IEC1bits.U2RXIE = 0; // disable interrupt
    if (buffer->tail == buffer->head) {
        IEC1bits.U2RXIE = 1; 
        return -1; // queue is empty (TODO OR FULL...)
    }
    else {
        *value = buffer->data[buffer->head];
        buffer->head = (buffer->head + 1) % DIMBUF;
        IEC1bits.U2RXIE = 1; 
        return 0;
    }
}
