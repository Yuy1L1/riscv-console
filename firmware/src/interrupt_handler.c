#include <stdint.h>
#include <stdio.h>
#include "interrupt_handler.h"
#include "memory_management.h"
#include "sprite_management.h"

volatile int video_counter = 0;

#define SMALL_SPRITE_PALETTE_ADDR 0x500F3000
volatile uint32_t *INTERRUPT_ENABLE_REGISTER = (volatile uint32_t*)(0x40000000);
volatile uint32_t *INTERRUPT_PENDING_REGISTER = (volatile uint32_t*)(0x40000004);

void delay_ms(unsigned int milliseconds) {
    unsigned int factor = 1000;
    volatile unsigned int count;

    for (; milliseconds > 0; milliseconds--) {
        for (count = factor; count > 0; count--) {
            // Empty loop with volatile count to prevent optimization.
        }
    }
}

/*int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    memmove((unsigned char*) palette, (unsigned char*) &color, 4);
    return 1;
}*/

uint32_t counter = 0;
uint8_t currentColorIndex = 0;

void video_interrupt() {
    video_counter += 1;
    uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
    // need to write 1 to the video bit in IPR, which is the second bit.
    interrupt_pending_register = interrupt_pending_register & 0x00000002;
}

void handle_cmd_interrupt() {
    if ((*INTERRUPT_PENDING_REGISTER >> 2) & 1 && (*INTERRUPT_ENABLE_REGISTER >> 2) & 1) {
        setSmallColorPalette(0, 0xFF00F0FF, 2); // cyan
        *INTERRUPT_PENDING_REGISTER |= (1 << 2);
        delay_ms(500);
        setSmallColorPalette(0, 0xFFFF00FF, 2); // magenta
    }
}

void handle_video_interrupt() {
    if ((*INTERRUPT_PENDING_REGISTER >> 1) & 1 && (*INTERRUPT_ENABLE_REGISTER >> 1) & 1) {
        counter++;
        if (counter % 5 == 0) {
            switch (currentColorIndex % 4) {
                case 0:
                    setSmallColorPalette(0, 0xFF800000, 3);
                    break;
                case 1:
                    setSmallColorPalette(0, 0xFF008000, 3);
                    break;
                case 2:
                    setSmallColorPalette(0, 0xFF000080, 3);
                    break;
                case 3:
                    setSmallColorPalette(0, 0xFFFF0000, 3);
                    break;
            }
            currentColorIndex++; // Move to the next color for the next cycle
        }
        *INTERRUPT_PENDING_REGISTER |= (1 << 1);
    }
}
