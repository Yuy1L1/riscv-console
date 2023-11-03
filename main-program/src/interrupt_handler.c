#include "interrupt_handler.h"
volatile int video_counter = 0;

// void cmd_interrupt() {
//     uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
//     interrupt_pending_register = interrupt_pending_register & 0x00000003;
// }

// void video_interrupt() {
//     video_counter += 1;
//     uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
//     // need to write 1 to the video bit in IPR, which is the second bit.
//     interrupt_pending_register = interrupt_pending_register & 0x00000002;
// }

