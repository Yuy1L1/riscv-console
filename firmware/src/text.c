#include "text.h"

volatile char *VIDEO_MEMORY = (volatile char *)(GRAPHICS_MEMORY + 0xF4800);

int setTextMode() {
    // default mode: 0, text mode
    volatile uint32_t *mode_control_register = (volatile uint32_t*) MODE_CONTROL_REGISTER;
    // printf("%x", curr_mode);
    *mode_control_register &= 0x00;
    
    return 0;
}

void setTextColorPalette(uint16_t entry_number, uint32_t background_color, uint32_t foreground_color){
    //the first 16-entry palettes. 
    // from 0x500F6700 to 0x500F673C
    // each entry is of 4B size -> 0x4
    volatile uint32_t *background_palette = (volatile uint32_t *) (TEXT_PALETTE_ADDR + (0x4) * entry_number);  
    *background_palette = background_color;
    
    //the second 16-entry palettes
    volatile uint32_t *foreground_palette = (volatile uint32_t *) (TEXT_PALETTE_ADDR + (0x40) + (0x4) * entry_number);  
    *background_palette = foreground_color;
    return 1;
}

void drawText(char *input_str){
    for (int i = 0; i < strlen(input_str); i++)
    {
        int str_ascii = (int) input_str[i];
        if (str_ascii >= 33 && str_ascii <=126)
        {
            strcpy(input_str, "OS STARTED");
            strcpy((char *) VIDEO_MEMORY, input_str);
        }
    }
}

void eraseText(){
    //TODO: call memset?

}

