#include <stdint.h>
#include <stdarg.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
uint32_t GetTicks(void);
uint32_t GetController(void);

#define CATRIDGE_STATUS_REGISTER 0x4000001C
#define INTERRUPT_PENDING_REGISTER 0x40000004
uint32_t colorEntry = 0xffffffff;

void start_here(void) {
    return;
}


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

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;
    int cur_mode = 0;

    // VIDEO_MEMORY[0] = 'H';
    // VIDEO_MEMORY[1] = 'e';
    // VIDEO_MEMORY[2] = 'l';
    // VIDEO_MEMORY[3] = 'l';
    // VIDEO_MEMORY[4] = 'o';
    // VIDEO_MEMORY[5] = ' ';
    // VIDEO_MEMORY[6] = 'W';
    // VIDEO_MEMORY[7] = 'o';
    // VIDEO_MEMORY[8] = 'r';
    // VIDEO_MEMORY[9] = 'l';
    // VIDEO_MEMORY[10] = 'd';
    // VIDEO_MEMORY[11] = '!';
    // VIDEO_MEMORY[12] = 'X';
    // printf("Hello, World! The value of global is %d and colorEntry is %x.", global, colorEntry);
    #define MODE_CONTROL_REGISTER 0x500F6780
    
    volatile uint32_t *mode_control_register = (volatile uint32_t *)MODE_CONTROL_REGISTER;
    uint32_t curr_mode = *mode_control_register;

    // Extract mode bit (bit 0) and refresh rate (bits 7..1)
    uint32_t mode_bit = curr_mode & 0x1;
    uint32_t refresh_rate = (curr_mode >> 1) & 0x7F;

    // Print the current mode and refresh rate
    
    int sprite_index = 0;
    int z_position = 2;
    int y_pos = 200;
    int x_pos2 = 200;
    int palette_index = 0;

    setGraphicsMode();
    cur_mode = 1;
    uint32_t sprite_control_structure = pack_sprite_data(sprite_index, z_position, y_pos, x_pos2, palette_index);    
    setSmallColorPalette(0, 0xFFFFFFFF, 0);
    drawSprite(sprite_control_structure);                    
    while (1) {
        int c = a + b + global;
        global = GetTicks();
        if (global != last_global) {
            uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
            if (((interrupt_pending_register) & 0x4) >> 2) {
                if (global/10 % 2 == 0) {
                    // if (cur_mode == 0) {
                    //     setGraphicsMode();
                    //     cur_mode = 1;
                    //     uint32_t sprite_control_structure = pack_sprite_data(sprite_index, z_position, y_pos, x_pos2, palette_index);    
                    //     setSmallColorPalette(0, 0xFFFFFFFF, 0);
                    //     drawSprite(sprite_control_structure);                    
                    // } else {
                    //     setTextMode();
                    //     cur_mode = 0;
                    // }
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
        uint32_t catridge_status = *((volatile uint32_t *) CATRIDGE_STATUS_REGISTER);
        // printf("%x", catridge_status);
        // check if the CIS (lowest bit) is 1
        if (catridge_status & 1 == 1) {
            volatile uint32_t start_address = catridge_status; 
            // make the lowest 2 bits 00
            start_address = start_address & 0xfffffe00;
            //start the application
            start_here();
        }
    }

    // upon successfully exit
    return 0;
}

// void launch(){
//     while(1){
//         uint32_t catridge_status = *((volatile uint32_t *) CATRIDGE_STATUS_REGISTER);
//         print("%x", catridge_status);
//         // check if the CIS (lowest bit) is 1
//         if (catridge_status & 1 == 1){
//             volatile uint32_t start_address = catridge_status; 
//             // make the lowest 2 bits 00
//             start_address = start_address & 0xfffffe00;
//             //start the application
//             start_here();
//         }
//     }
// }
