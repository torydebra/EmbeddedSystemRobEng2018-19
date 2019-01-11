/*
 * File:   buffer.c
 * Author: tori
 *
 * Created on 11 gennaio 2019, 10.04
 */

#include "buffer.h"

void initBuf (Buffer *buffer){
    buffer->head = 0;
    buffer->tail = 0;
}

void writeBuf (Buffer *buffer, int c) {
    if (buffer->tail < DIMBUF) {
        buffer->data[buffer->head] = c;
        buffer->tail = (buffer->tail + 1) % DIMBUF;
    }
}

int readBuf (Buffer *buffer) {
    if (buffer->tail == buffer->head) {
        return 0; // queue is empty
    }
    else {
        int value = buffer->data[buffer->head];
        buffer->head = (buffer->head + 1) % DIMBUF;
        return value;
    }
}
