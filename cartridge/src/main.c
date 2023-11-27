#include <stdint.h>
#include <stddef.h>

#include "api.h"

volatile int global = 42;
volatile uint32_t controller_status = 0;

#define CATRIDGE_STATUS_REGISTER 0x4000001C
uint32_t colorEntry = 0xffffffff;
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);

// changed from pack sprite data to pack small sprite data
uint32_t pack_small_sprite_data(int index, int z, int y, int x, int palette) {
    uint32_t packed_data = 0;

    // masks for each variable
    uint32_t index_mask = 0xFF;       // Index fits in 8 bits
    uint32_t z_mask = 0x7;            // Z fits in 3 bits
    uint32_t y_mask = 0x1FF;          // Y fits in 9 bits
    uint32_t x_mask = 0x3FF;          // X fits in 10 bits
    uint32_t palette_mask = 0x3;      // Palette fits in 2 bits
    index &= index_mask;
    z &= z_mask;
    y &= y_mask;
    x &= x_mask;
    palette &= palette_mask;

    packed_data |= (index << 24); // Index in bits 31..24
    packed_data |= (z << 21);     // Z in bits 23..21
    packed_data |= (y << 12);     // Y in bits 20..12
    packed_data |= (x << 2);      // X in bits 11..2
    packed_data |= palette;       // Palette in bits 1..0
    return packed_data;
}

// the func to pack both medium and large sprite data
uint32_t pack_med_large_sprite_data( int reserved, int index, int z, int y, int x, int palette) {
    uint32_t packed_data = 0;
    // masks for each variable
    uint32_t reserved_mask = 0x7;     // reserve fits 3 bits
    uint32_t index_mask = 0x1F;       // Index fits in 5 bits
    uint32_t z_mask = 0x7;            // Z fits in 3 bits
    uint32_t y_mask = 0x1FF;          // Y fits in 9 bits
    uint32_t x_mask = 0x3FF;          // X fits in 10 bits
    uint32_t palette_mask = 0x3;      // Palette fits in 2 bits
    reserved &= reserved_mask;
    index &= index_mask;
    z &= z_mask;
    y &= y_mask;
    x &= x_mask;
    palette &= palette_mask;

    packed_data |= (reserved << 30);  // reserved in bits 31..30
    packed_data |= (index << 24); // Index in bits 29..24
    packed_data |= (z << 21);     // Z in bits 23..21
    packed_data |= (y << 12);     // Y in bits 20..12
    packed_data |= (x << 2);      // X in bits 11..2
    packed_data |= palette;       // Palette in bits 1..0
    return packed_data;
}

uint32_t pack_background_sprite_data(int mode, int index1, int index2, int z, int y, int x, int palette) {
    uint32_t packed_data = 0;

    uint32_t mode_mask = 0x1;       // mode 1 bit
    uint32_t z_mask = 0x7;          // Z fits in 3 bits
    uint32_t y_mask = 0x3FF;        // Y fits in 10 bits
    uint32_t x_mask = 0x3FF;        // X fits in 10 bits
    uint32_t palette_mask = 0x3;    // Palette fits in 2 bits

    mode &= mode_mask;
    z &= z_mask;
    y &= y_mask;
    x &= x_mask;
    palette &= palette_mask;

    if (mode == 0) {
        // Mode 0: Pixel mode
        uint32_t reserved_mask = 0x3;     // index1 (reserved) fits 2 bits
        uint32_t px_index_mask = 0xF;     // index2 (px index) fits in 4 bits

        index1 &= reserved_mask;
        index2 &= px_index_mask;

        packed_data |= (index1 << 31);    // reserved in bits 31..30
        packed_data |= (index2 << 25);    // px index in bits 28..25
    } else {
        // Mode 1: Tile mode
        uint32_t ti_idx_mask = 0x7;       // index1 (ti index) fits 3 bits
        uint32_t sub_index_mask = 0x7;    // index2 (sub index) fits in 3 bits

        index1 &= ti_idx_mask;
        index2 &= sub_index_mask;

        packed_data |= (mode << 31);      // mode in bit 31
        packed_data |= (index1 << 28);    // ti index in bits 30..28
        packed_data |= (index2 << 25);    // sub index in bits 27..25
    }

    packed_data |= (z << 22);     // Z in bits 24..22
    packed_data |= (y << 12);     // Y in bits 21..12
    packed_data |= (x << 2);      // X in bits 11..2
    packed_data |= palette;       // Palette in bits 1..0
    return packed_data;
}

#define SMALL_SPRITE_DATA_ADDR  0x500E0000
#define SMALL_SPRITE_DATA_SIZE  0x100 
#define SMALL_SPRITE_CONTROL_ADDR  0x500F6300
#define SMALL_SPRITE_CONTROL_SIZE  0x4 
#define SMALL_SPRITE_PALETTE_ADDR      0x500F3000
uint32_t small_sprite_counter = 0;


int setSmallColorPalette2(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    MemMove((unsigned char*) palette, (unsigned char*) &color, 4);
    return 1;
}

bool small_sprite_slot_in_use[256] = {false};

uint16_t drawSmallSprite2(uint32_t sprite_control_structure, uint8_t sprite_color) {
    int slot = -1;
    for (int i = 0; i < 256; i++) {
        if (!small_sprite_slot_in_use[i]) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        return -1; // No available slot
    }

    small_sprite_slot_in_use[slot] = true;

    uint8_t index = (sprite_control_structure >> 24) & 0xFF;
    uint8_t* small_data_addr = (volatile uint32_t *)(SMALL_SPRITE_DATA_ADDR + slot * SMALL_SPRITE_DATA_SIZE);
    for (int k = 0; k < 256; k++) {
        *(small_data_addr + k) = sprite_color;
    }

    uint32_t *small_control_addr = (volatile uint32_t *)(SMALL_SPRITE_CONTROL_ADDR + slot * SMALL_SPRITE_CONTROL_SIZE);
    *small_control_addr = sprite_control_structure;
    small_sprite_counter++;

    return slot;
}

int main() {
    int a = 4;
    int b = 12;
    uint32_t last_global = 42;
    int x_pos = 12;
    int countdown = 1;
    uint32_t global = 42;

    setGraphicsMode();

    int sprite_index1 = 0;
    int z_position1 = 2;
    int y_pos1 = 200;
    int x_pos1 = 200;
    int palette_index1 = 0;
    uint32_t periodic_sprite = pack_small_sprite_data(sprite_index1, z_position1, y_pos1, x_pos1, palette_index1);

    int sprite_index2 = 1;
    int z_position2 = 2;
    int y_pos2 = 200;
    int x_pos2 = 300;
    int palette_index2 = 0;
    uint32_t interrupt_sprite = pack_small_sprite_data(sprite_index2, z_position2, y_pos2, x_pos2, palette_index2);    

    int sprite_index3 = 2;
    int z_position3 = 2;
    int y_pos3 = 100;
    int x_pos3 = 250;
    int palette_index3 = 0;
    uint32_t video_interrupt_sprite = pack_small_sprite_data(sprite_index3, z_position3, y_pos3, x_pos3, palette_index3);

    setSmallColorPalette(0, 0xFFFF0000, 1);
    setSmallColorPalette(0, 0xFF0000FF, 2);
    setSmallColorPalette(0, 0xFF0000FF, 3);

    uint8_t periodic_palette_color = 1;
    uint8_t interrupt_palette_color = 2;
    uint8_t video_palette_color = 3;
    int16_t interrupt_sprite_id = -1;
    drawSmallSprite2(periodic_sprite, periodic_palette_color); // Periodically
    interrupt_sprite_id = drawSmallSprite2(interrupt_sprite, interrupt_palette_color); // CMD button.
    drawSmallSprite2(video_interrupt_sprite, video_palette_color); //Every time the screen refreshes.
    int periodic_switcher = 0;
    int controllerHasUpdate = 0;

    while (1) {
        int c = a + b + global;
        global = GetTicks();
        
        controllerHasUpdate = 0;
        if (global != last_global) {
            if (global % 20 == 0) {
                if (periodic_switcher) {
                    setSmallColorPalette(0, 0xFFFFFFFF, 1);
                    periodic_switcher = 0;
                } else {
                    setSmallColorPalette(0, 0xFFFF0000, 1);
                    periodic_switcher = 1;
                }
            }
            controller_status = GetController();
            if (controller_status & 0x1) {
                x_pos2 -= 10;
                controllerHasUpdate = 1;
            }

            if (controller_status & 0x2) {
                y_pos2 -= 10;
                controllerHasUpdate = 1;
            }

            if (controller_status & 0x4) {
                y_pos2 += 10;
                controllerHasUpdate = 1;
            }

            if (controller_status & 0x8) {
                x_pos2 += 10;
                controllerHasUpdate = 1;
            }
            if (controllerHasUpdate) {
                // Enable this line to enable deleting sprites.
                eraseSmallSprite(interrupt_sprite_id);
                interrupt_sprite = pack_small_sprite_data(sprite_index2, z_position2, y_pos2, x_pos2, palette_index2);
                interrupt_sprite_id = drawSmallSprite2(interrupt_sprite, interrupt_palette_color);
            }
            
            last_global = global;
        }
        //TODO: call up funcs related to medium and large size sprites
    }
    return 0;

}
