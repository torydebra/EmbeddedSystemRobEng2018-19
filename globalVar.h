#include <xc.h> // include processor files - each processor file is guarded.  
#include "scheduler.h"
#include "buffer.h"
#include "bufferTemp.h"
#include "parser.h"

#define STATE_CONTROL 0
#define STATE_TIMEOUT 1
#define STATE_SAFE 2

extern Heartbeat schedInfo[MAX_TASKS];
extern Buffer bufReceiving;
extern BufferTemp buff;
extern Parser_state pstate;
extern short int boardState;



