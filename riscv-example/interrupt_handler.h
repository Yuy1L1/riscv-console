#include <stdlib.h>

#define INTERRUPT_ENABLE_REGISTEER 0x40000000
#define INTERRUPT_PENDING_REGISTEER 0x40000004
#define CATRIDGE_STATUS_REGISTER 0x4000001C

void video_interrupt();
void cmd_interrupt();
/*
To be implemented in the future:
void timer_interrupt();
void cart_interrupt();
void illegal_inst_interrupt();
*/
