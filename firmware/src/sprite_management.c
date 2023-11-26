#include "sprite_managment.h"

uint32_t small_sprite_counter = 0;
uint32_t medium_sprite_counter = 0;
uint32_t large_sprite_counter = 0;
uint32_t background_sprite_counter = 0;

int setGraphicMode(){
    // default mode: 0, text mode
    uint32_t curr_mode = *((volatile uint32_t *) MODE_CONTROL_REGISTER);
    printf("%x", curr_mode);
    curr_mode |=1;
    return 0;
}

int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    // each palette is of 1 KiB size 1 KiB = 1024 B = 0x400
    // each entry is worth 4 B -> 0x4
    // color is for the RGB val, entry_number is for which entry in the palette to change
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    MemMove((unsigned char*) palette, (unsigned char*) &color, 4);
    return 1;
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

// medium sprite

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



// large sprite
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



// background sprite
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
    background_sprite_counter++;

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

