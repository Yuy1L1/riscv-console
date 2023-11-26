#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TEXT_PALETTE_ADDR     0x500F6700
#define MODE_CONTROL_REGISTER 0x500F6780
#define GRAPHICS_MEMORY       0x50000000

int setTextMode();
void setTextColorPalette(uint16_t entry_number, uint32_t background_color, uint32_t foreground_color);
void drawText(char *input_str);
void eraseText();
