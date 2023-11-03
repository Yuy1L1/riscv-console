#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

#define CATRIDGE_STATUS_REGISTER 0x4000001C
uint32_t colorEntry = 0xffffffff;

uint32_t GetTicks(void);
uint32_t GetController(void);

uint32_t pack_sprite_data(int index, int z, int y, int x, int palette) {
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

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);

int main() {
    int a = 4;
    int b = 12;
    uint32_t last_global = 42;
    int x_pos = 12;
    int countdown = 1;
    uint32_t global = 42;
    uint32_t controller_status = 0;

    VIDEO_MEMORY[0] = 'H';
    VIDEO_MEMORY[1] = 'e';
    VIDEO_MEMORY[2] = 'l';
    VIDEO_MEMORY[3] = 'l';
    VIDEO_MEMORY[4] = 'o';
    VIDEO_MEMORY[5] = ' ';
    VIDEO_MEMORY[6] = 'W';
    VIDEO_MEMORY[7] = 'o';
    VIDEO_MEMORY[8] = 'r';
    VIDEO_MEMORY[9] = 'l';
    VIDEO_MEMORY[10] = 'd';
    VIDEO_MEMORY[11] = '!';
    VIDEO_MEMORY[12] = 'X';
    setGraphicsMode();

    int sprite_index1 = 0;
    int z_position1 = 2;
    int y_pos1 = 200;
    int x_pos1 = 200;
    int palette_index1 = 0;
    uint32_t periodic_sprite = pack_sprite_data(sprite_index1, z_position1, y_pos1, x_pos1, palette_index1);

    int sprite_index2 = 1;
    int z_position2 = 2;
    int y_pos2 = 200;
    int x_pos2 = 300;
    int palette_index2 = 0;
    uint32_t interrupt_sprite = pack_sprite_data(sprite_index2, z_position2, y_pos2, x_pos2, palette_index2);    

    int sprite_index3 = 3;
    int z_position3 = 2;
    int y_pos3 = 100;
    int x_pos3 = 250;
    int palette_index3 = 0;
    uint32_t video_interrupt_sprite = pack_sprite_data(sprite_index3, z_position3, y_pos3, x_pos3, palette_index3);

    setSmallColorPalette(0, 0xFFFF0000, 1);
    setSmallColorPalette(0, 0xFF0000FF, 2);
    setSmallColorPalette(0, 0xFF0000FF, 3);
    uint8_t periodic_palette_color = 1;
    uint8_t interrupt_palette_color = 2;
    uint8_t video_palette_color = 3;
    drawSprite(periodic_sprite, periodic_palette_color);
    drawSprite(interrupt_sprite, interrupt_palette_color);
    drawSprite(video_interrupt_sprite, video_palette_color);
    int periodic_switcher = 0;
    while (1) {
        int c = a + b + global;
        global = GetTicks();
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
            if (controller_status) {
                VIDEO_MEMORY[x_pos] = ' ';
                if (controller_status & 0x1) {
                    if (x_pos & 0x3F) {
                        x_pos--;
                    }
                }
                if (controller_status & 0x2) {
                    if (x_pos >= 0x40) {
                        x_pos -= 0x40;
                    }
                }
                if (controller_status & 0x4) {
                    if (x_pos < 0x8C0) {
                        x_pos += 0x40;
                    }
                }
                if (controller_status & 0x8) {
                    if ((x_pos & 0x3F) != 0x3F) {
                        x_pos++;
                    }
                }
                VIDEO_MEMORY[x_pos] = 'X';
            }
            last_global = global;
        }
    }
    return 0;
}
