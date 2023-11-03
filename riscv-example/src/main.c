#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);

#define CATRIDGE_STATUS_REGISTER 0x4000001C
uint32_t colorEntry = 0xffffffff;
void start_here(void){
    return;
}

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;

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


    while (1) {
        int c = a + b + global;
        if(global != last_global){
            if(controller_status){
                VIDEO_MEMORY[x_pos] = ' ';
                if(controller_status & 0x1){
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }
                if(controller_status & 0x2){
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(controller_status & 0x4){
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
                if(controller_status & 0x8){
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                VIDEO_MEMORY[x_pos] = 'X';
            }
            last_global = global;
        }
        uint32_t catridge_status = *((volatile uint32_t *) CATRIDGE_STATUS_REGISTER);
        // printf("%x", catridge_status);
        // check if the CIS (lowest bit) is 1
        if (catridge_status & 1 == 1){
            volatile uint32_t start_address = catridge_status; 
            // make the lowest 2 bits 00
            start_address = start_address & 0xfffffe00;
            //start the application
            start_here();
        }
    }
    setGraphicMode();
    uint32_t sprite_control_structure = 0x00F00800;
    setColorPalette(0, 0xFFFF0000, 0); // Red
    drawSprite(sprite_control_structure);

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
