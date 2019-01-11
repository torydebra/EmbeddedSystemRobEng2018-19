#include "setupClass.h"
#include "parser.h"
#include "buffer.h"

void setupParser(){
    parser_state pstate;
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;
}

void setupBuffer(){
    Buffer bufReceiving;
    initBuf(bufReceiving);
}
