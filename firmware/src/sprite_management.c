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
