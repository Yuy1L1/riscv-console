#ifndef SPRITE_MANAGEMENT_H
#define SPRITE_MANAGEMENT_H

#include <stdlib.h>
#include <stdio.h>

#define SMALL_SPRITE_DATA_ADDR 0x500E0000
#define MEDIUM_SPRITE_DATA_ADDR 0x500D0000
#define LARGE_SPRITE_DATA_ADDR 0x50090000

#define SMALL_SPRITE_SIZE 0x100  // data size of 256B as specified in video.md
#define MEDIUM_SPRITE_SIZE 0x400 // data size of 1KiB as specified in video.md
#define LARGE_SPRITE_SIZE 0x1000 // data size of 4KiB as specified in video.md

#define SMALL_SPRITE_CONTROL_ADDR 0x500F6300
#define MEDIUM_SPRITE_CONTROL_ADDR 0x500F5F00
#define LARGE_SPRITE_CONTROL_ADDR 0x500F5B00

#define SMALL_SPRITE_CONTROL_SIZE 0x4 
#define MEDIUM_SPRITE_CONTROL_SIZE 0x4 
#define LARGE_SPRITE_CONTROL_SIZE 0x4

#define MODE_CONTROL_REGISTER 0x500FF414
#define GRAPHICS_MEMORY 0x50000000

int setGraphicMode();
uint16_t drawSprite(uint32_t sprite_control_structure);

#endif