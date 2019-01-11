#include "setupClass.h"
#include "parser.h"
#include "buffer.h"

void setupParser(){
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;
}

void setupBuffer(){
    initBuf(bufReceiving);
    
}
