#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TEXT_PALETTE_ADDR 0x500F6700

int setTextMode();
void setTextColorPalette(uint16_t entry_number, uint32_t background_color, uint32_t foreground_color);
void drawText(char *input_str);
void eraseText();
