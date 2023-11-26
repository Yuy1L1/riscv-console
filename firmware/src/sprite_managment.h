#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef SPRITE_MANAGEMENT_H
#define SPRITE_MANAGEMENT_H

#define SMALL_SPRITE_DATA_ADDR  0x500E0000
#define MEDIUM_SPRITE_DATA_ADDR 0x500D0000
#define LARGE_SPRITE_DATA_ADDR  0x50090000
#define BACKGROUND_SPRITE_DATA_ADDR 0x50000000

#define SMALL_SPRITE_DATA_SIZE  0x100 
#define MEDIUM_SPRITE_DATA_SIZE 0x400 
#define LARGE_SPRITE_DATA_SIZE 0x1000
#define BACKGROUND_SPRITE_DATA_SIZE_0 0x24000
#define BACKGROUND_SPRITE_DATA_SIZE_1 0x12000

#define SMALL_SPRITE_CONTROL_ADDR  0x500F6300
#define MEDIUM_SPRITE_CONTROL_ADDR 0x500F5F00
#define LARGE_SPRITE_CONTROL_ADDR  0x500F5B00
#define BACKGROUND_SPRITE_CONTROL_ADDR 0x500F5A00

#define SMALL_SPRITE_CONTROL_SIZE  0x4 
#define MEDIUM_SPRITE_CONTROL_SIZE 0x4 
#define LARGE_SPRITE_CONTROL_SIZE  0x4
#define BACKGROUND_SPRITE_CONTROL_SIZE 0x4

#define SMALL_SPRITE_PALETTE_ADDR      0x500F3000
#define MEDIUM_SPRITE_PALETTE_ADDR     0x500F2000
#define LARGE_SPRITE_PALETTE_ADDR      0x500F1000
#define BACKGROUND_SPRITE_PALETTE_ADDR 0x500F0000

#define MODE_CONTROL_REGISTER 0x500FF414
#define GRAPHICS_MEMORY 0x50000000

int setGraphicMode();
// small sprite
int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
void changeSmallSpriteColor();
uint16_t drawSprite(uint32_t sprite_control_structure);
void eraseSmallSprite(uint8_t slot);
void moveSmallSprite(uint8_t slot, uint32_t sprite_control_structure, uint8_t sprite_color);
// medium sprite
int setMediumColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int16_t drawMediumSprite(uint32_t sprite_control_structure, uint8_t sprite_color);
void eraseMediumSprite(uint8_t slot);
void changeMediumSpriteColor();
// large sprite
int setLargeColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int16_t drawLargeSprite(uint32_t sprite_control_structure, uint8_t sprite_color);
void eraseLargeSprite(uint8_t slot);
void changeLargeSpriteColor();
// background sprite
int setBackgroundColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number);
int changeBackgroundColorPalette();
int drawBackgroundSprite(uint32_t sprite_control_structure, uint8_t sprite_color, uint32_t mode);
void eraseBackgroundSprite(uint8_t slot, uint32_t mode);


#endif
