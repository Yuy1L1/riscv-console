#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include <stdlib.h>
extern volatile uint32_t *INTERRUPT_ENABLE_REGISTER;
extern volatile uint32_t *INTERRUPT_PENDING_REGISTER;


void video_interrupt();
void handle_cmd_interrupt();
void handle_video_interrupt();
void c_interrupt_handler();
/*
To be implemented in the future:
void timer_interrupt();
void cart_interrupt();
void illegal_inst_interrupt();
*/

#endif