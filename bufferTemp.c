/*
 * File:   bufferTemp.c
 * Author: fafux
 *
 * Created on 11 gennaio 2019, 10.04
 */

#include "bufferTemp.h"

void initBufTemp (BufferTemp *buffer){
    buffer->index = 0;
    buffer->data = [0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0];
}

void writeBufTemp (BufferTemp *buffer, float c) {
    buffer->data[buffer->index] = c;
    buffer->index = (buffer->index + 1) % DIMBUFTEMP;
}