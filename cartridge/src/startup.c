#include <stdint.h>
#include <stdbool.h>

#include "api.h"

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void) {
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val) {
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val) {
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void) {
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void) {
    asm volatile ("csrci mstatus, 0x8");
}

extern volatile int global;
extern volatile uint32_t controller_status;

#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))

#define CATRIDGE_STATUS_REGISTER 0x4000001C

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

#define MODE_CONTROL_REGISTER 0x500F6780
#define GRAPHICS_MEMORY       0x50000000

#define SMALL_SPRITE_PALETTE_ADDR      0x500F3000
#define MEDIUM_SPRITE_PALETTE_ADDR     0x500F2000
#define LARGE_SPRITE_PALETTE_ADDR      0x500F1000
#define BACKGROUND_SPRITE_PALETTE_ADDR 0x500F0000

void init(void) {
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    while(Base < End) {
        *Base++ = *Source++;
    }
    Base = _bss;
    End = _ebss;
    while(Base < End) {
        *Base++ = 0;
    }
}

uint32_t small_sprite_counter = 0;
uint32_t medium_sprite_counter = 0;
uint32_t large_sprite_counter = 0;

int setGraphicsMode() {
    // default mode: 0, text mode
    volatile uint32_t *mode_control_register = (volatile uint32_t*) MODE_CONTROL_REGISTER;
    // printf("%x", curr_mode);
    *mode_control_register |= 0x01;

    return 0;
}

/*int setTextMode() {
    // default mode: 0, text mode
    volatile uint32_t *mode_control_register = (volatile uint32_t*) MODE_CONTROL_REGISTER;
    // printf("%x", curr_mode);
    *mode_control_register &= 0x00;
    
    return 0;
}*/

// TODO: -> another file: GRAPHICS MODE
// for each type, have set color palette, draw, erase, move, and change color

int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    // each palette is of 1 KiB size 1 KiB = 1024 B = 0x400
    // each entry is worth 4 B -> 0x4
    // color is for the RGB val, entry_number is for which entry in the palette to change
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    MemMove((unsigned char*) palette, (unsigned char*) &color, 4);
    return 1;
}

void changeSmallSpriteColor() {
    // TODO: is there really a diff btw set and change?
}

bool small_sprite_slot_in_use[256] = {false};

int16_t drawSmallSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
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

void eraseSmallSprite(uint8_t slot) {
    if (slot >= 256) {
        return;
    }

    uint8_t* small_data_addr = (volatile uint32_t *)(SMALL_SPRITE_DATA_ADDR + slot * SMALL_SPRITE_DATA_SIZE);
    for (int k = 0; k < 256; k++) {
        *(small_data_addr + k) = 0;
    }

    uint32_t *small_control_addr = (volatile uint32_t *)(SMALL_SPRITE_CONTROL_ADDR + slot * SMALL_SPRITE_CONTROL_SIZE);
    *small_control_addr = 0;

    // Mark slot as available
    small_sprite_slot_in_use[slot] = false;
}

void moveSmallSprite(uint8_t slot, uint32_t sprite_control_structure, uint8_t sprite_color) {
    // TODO: can I move the obj directly or erase the one now and redraw it at the the new site?
    // first get rid of the current one
    eraseSmallSprite(slot);
    // redraw it somewhere else
    drawSmallSprite(sprite_control_structure, sprite_color);
    return;
}


int setMediumColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    //each palette is of 1 KiB size => 0x400
    // each entry is of 4B in the memory map

    volatile uint32_t *palette = (volatile uint32_t *) (MEDIUM_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    *palette = color;
    return 1;
}

bool medium_sprite_slot_in_use[64] = {false};

int16_t drawMediumSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    int slot = -1;
    for (int i = 0; i < 64; i++) {
        if (!medium_sprite_slot_in_use[i]) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        return -1; // No available slot
    }

    medium_sprite_slot_in_use[slot] = true;

    uint8_t index = (sprite_control_structure >> 24) & 0xFF;
    uint8_t* medium_data_addr = (volatile uint32_t *)(MEDIUM_SPRITE_DATA_ADDR + slot * MEDIUM_SPRITE_DATA_SIZE);
    for (int k = 0; k < 64; k++) {
        *(medium_data_addr + k) = sprite_color;
    }

    uint32_t *medium_control_addr = (volatile uint32_t *)(MEDIUM_SPRITE_CONTROL_ADDR + slot * MEDIUM_SPRITE_CONTROL_SIZE);
    *medium_control_addr = sprite_control_structure;
    medium_sprite_counter++;

    return slot;
}

void eraseMediumSprite(uint8_t slot) {
    if (slot >= 64) {
        return;
    }

    uint8_t* medium_data_addr = (volatile uint32_t *)(MEDIUM_SPRITE_DATA_ADDR + slot * MEDIUM_SPRITE_DATA_SIZE);
    for (int k = 0; k < 64; k++) {
        *(medium_data_addr + k) = 0;
    }

    uint32_t *medium_control_addr = (volatile uint32_t *)(MEDIUM_SPRITE_CONTROL_ADDR + slot * MEDIUM_SPRITE_CONTROL_SIZE);
    *medium_control_addr = 0;

    // Mark slot as available
    medium_sprite_slot_in_use[slot] = false;
}

void changeMediumSpriteColor() {
    //TODO
}

int setLargeColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    //each palette is of 1 KiB size => 0x400
    // each entry is of 4B in the memory map
    volatile uint32_t *palette = (volatile uint32_t *) (LARGE_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    *palette = color;
    return 1;
}

bool large_sprite_slot_in_use[64] = {false};

int16_t drawLargeSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    int slot = -1;
    for (int i = 0; i < 64; i++) {
        if (!large_sprite_slot_in_use[i]) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        return -1; // No available slot
    }

    medium_sprite_slot_in_use[slot] = true;

    uint8_t index = (sprite_control_structure >> 24) & 0xFF;
    uint8_t* large_data_addr = (volatile uint32_t *)(LARGE_SPRITE_DATA_ADDR + slot * LARGE_SPRITE_DATA_SIZE);
    for (int k = 0; k < 64; k++) {
        *(large_data_addr + k) = sprite_color;
    }

    uint32_t *large_control_addr = (volatile uint32_t *)(LARGE_SPRITE_CONTROL_ADDR + slot * LARGE_SPRITE_CONTROL_SIZE);
    *large_control_addr = sprite_control_structure;
    large_sprite_counter++;

    return slot;
}

void eraseLargeSprite(uint8_t slot) {
    if (slot >= 64) {
        return;
    }

    uint8_t* large_data_addr = (volatile uint32_t *)(LARGE_SPRITE_DATA_ADDR + slot * LARGE_SPRITE_DATA_SIZE);
    for (int k = 0; k < 64; k++) {
        *(large_data_addr + k) = 0;
    }

    uint32_t *large_control_addr = (volatile uint32_t *)(LARGE_SPRITE_CONTROL_ADDR + slot * LARGE_SPRITE_CONTROL_SIZE);
    *large_control_addr = 0;

    // Mark slot as available
    large_sprite_slot_in_use[slot] = false;
}

void changeLargeSpriteColor() {
    // TODO
}

void c_interrupt_handler(void) {
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;
}

int setBackgroundColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    // each palette is of 1 KiB size 1 KiB = 1024 B = 0x400
    // each entry is worth 4 B -> 0x4 in the memory map
    // color is for the RGB val, entry_number is for which entry in the palette to change
    volatile uint32_t *palette = (volatile uint32_t *) (BACKGROUND_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    *palette = color;
    return 1;
}

int changeBackgroundColorPalette() {
    // TODO: is there really a diff btw set and change?
    return 1;
}

bool background_sprite_slot_in_use_0[4] = {false}; // only 4 parts in "pixel" mode
bool background_sprite_slot_in_use_1[8] = {false}; // only 8 parts in "tile" mode


int drawBackgroundSprite(uint32_t sprite_control_structure, uint8_t sprite_color, uint32_t mode){
    int slot = -1;
    if (mode == 0) // if "pixel" mode
    {
        for (int i = 0; i < 4; i++) 
        {
            if (!background_sprite_slot_in_use_0[i]) 
            {
            slot = i;
            break;
            }
        }

        if (slot == -1) 
        {
            return -1; // No available slot
        }

        background_sprite_slot_in_use_0[slot] = true;

        uint8_t index = (sprite_control_structure >> 24) & 0xFF;
        uint8_t* background_data_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_DATA_ADDR + slot * BACKGROUND_SPRITE_DATA_SIZE_0);
        for (int k = 0; k < 4; k++) 
        {
            *(background_data_addr + k) = sprite_color;
        }
    }
    else // "tile" mode
    {
        for (int i = 0; i < 8; i++) 
        {
            if (!background_sprite_slot_in_use_1[i]) 
            {
            slot = i;
            break;
            }
        }

        if (slot == -1) 
        {
            return -1; // No available slot
        }

        background_sprite_slot_in_use_1[slot] = true;

        uint8_t index = (sprite_control_structure >> 24) & 0xFF;
        uint8_t* background_data_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_DATA_ADDR + slot * BACKGROUND_SPRITE_DATA_SIZE_1);
        for (int k = 0; k < 8; k++) 
        {
            *(background_data_addr + k) = sprite_color;
        }
    }

    uint32_t *background_control_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_CONTROL_ADDR + slot * BACKGROUND_SPRITE_CONTROL_SIZE);
    *background_control_addr = sprite_control_structure;
    //background_sprite_counter++;

    return slot;
}

void eraseBackgroundSprite(uint8_t slot, uint32_t mode){
    if (mode == 0)
    {
        if (slot >= 4) 
        {
        return;
        }
        uint8_t* background_data_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_DATA_ADDR + slot * BACKGROUND_SPRITE_DATA_SIZE_0);
        for (int k = 0; k < 4; k++) 
        {
            *(background_data_addr + k) = 0;
        }
    }
    else
    {
        if (slot >= 8) 
        {
        return;
        }
        uint8_t* background_data_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_DATA_ADDR + slot * BACKGROUND_SPRITE_DATA_SIZE_1);
        for (int k = 0; k < 8; k++) 
        {
            *(background_data_addr + k) = 0;
        }
    }
    
    uint32_t *background_control_addr = (volatile uint32_t *)(BACKGROUND_SPRITE_CONTROL_ADDR + slot * BACKGROUND_SPRITE_CONTROL_SIZE);
    *background_control_addr = 0;

    // Mark slot as available
    if (mode == 0)
    {
        background_sprite_slot_in_use_0[slot] = false;
    }
    else
    {
        background_sprite_slot_in_use_1[slot] = false;
    }
}

// TODO: ===================================================-> another file: TEXT MODE

#define TEXT_PALETTE_ADDR 0x500F6700


int setTextMode() {
    // default mode: 0, text mode
    volatile uint32_t *mode_control_register = (volatile uint32_t*) MODE_CONTROL_REGISTER;
    // printf("%x", curr_mode);
    *mode_control_register &= 0x00;
    
    return 0;
}


void drawText(char *input_str){
    for (int i = 0; i < strlen(input_str); i++)
    {
        int ascii = (int) input_str[i];
        if (ascii >= 33 && ascii <=126)
        {
            // TODO: how to write to screen?
        }
    }
    
}

void eraseText(){
    //TODO
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

