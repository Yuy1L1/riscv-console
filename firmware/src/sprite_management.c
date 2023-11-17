#include "sprite_managment.h"


uint32_t small_sprite_counter = 0;
uint32_t medium_sprite_counter = 0;
uint32_t large_sprite_counter = 0;

int setGraphicMode(){
    // default mode: 0, text mode
    uint32_t curr_mode = *((volatile uint32_t *) MODE_CONTROL_REGISTER);
    printf("%x", curr_mode);
    curr_mode |=1;
    return 0;
}


// drawSprite(Sprite sprite, int x, int y, int z)
uint16_t drawSprite(uint32_t sprite_control_structure){

    uint32_t x, y, z, width, height, palette;
    palette = sprite_control_structure & 0b11;
    x = (((1 << 10) - 1) & (sprite_control_structure >> 10));
    y = (((1 << 9) - 1) & (sprite_control_structure >> 19));
    z = (((1 << 3) - 1) & (sprite_control_structure >> 22));
    width = x - 16;
    height = y - 16;

    // check if sprite is large, medium or small
    if (width < 16 && height < 16){
        // dynamically draw small sprite 
        // uint8_t small_data_addr = *((volatile uint8_t * )(SMALL_SPRITE_DATA_ADDR + small_sprite_counter * SMALL_SPRITE_SIZE));
        // for (int i = 0; i < 16; i++)
        // {
        //     for (int j = 0; j < 16; j++)
        //     {
        //         // if there is color, need to color within bounds of width and height
        //         if (i <  width && j < height){
        //             small_data_addr[i * 64 + j] = 0x00000000; // transparent, 
        //             // TODO: color can be customized in the future
        //         }
        //         else
        //         {
        //             small_data_addr[i * 64 + j] = 0;
        //         }
        //     }
        // }

        uint32_t * small_control_addr = (volatile uint32_t *)(SMALL_SPRITE_CONTROL_ADDR + small_sprite_counter * SMALL_SPRITE_CONTROL_SIZE);
        small_control_addr[0] = sprite_control_structure;
        small_sprite_counter ++;
    }
    // medium sprite
    if (width < 64 && height < 64){
        uint32_t * medium_control_addr = (volatile uint32_t *)(MEDIUM_SPRITE_CONTROL_ADDR + medium_sprite_counter * MEDIUM_SPRITE_CONTROL_SIZE);
        medium_control_addr[0] = sprite_control_structure;
        medium_control_addr ++;
    }
    // a large sprite
    if (width > 64 && height > 64)
    {
        uint32_t * large_control_addr = (volatile uint32_t *)(LARGE_SPRITE_CONTROL_ADDR + large_sprite_counter * LARGE_SPRITE_CONTROL_SIZE);
        large_control_addr[0] = sprite_control_structure;
        large_sprite_counter ++;
    }
    // successful
    return 0;
}
