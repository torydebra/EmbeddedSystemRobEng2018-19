/*
 * File:   bufferTemp.c
 * Author: fafux & tori
 *
 * Created on 11 gennaio 2019, 10.04
 */

#include "../../header/support/bufferTemp.h"
#include "../../header/support/globalVar.h"

void initBufTemp (){
    buff.index = 0;
    //buffer->data[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
}

void writeBufTemp (float c) {
    buff.data[buff.index] = c;
    buff.index = (buff.index + 1) % DIMBUFTEMP;
}
