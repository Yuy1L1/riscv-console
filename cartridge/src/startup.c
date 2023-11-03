#include <stdint.h>

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
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

#define MODE_CONTROL_REGISTER 0x500F6780
#define GRAPHICS_MEMORY 0x50000000

#define SMALL_SPRITE_PALETTE_ADDR 0x500F3000

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    while(Base < End){
        *Base++ = *Source++;
    }
    Base = _bss;
    End = _ebss;
    while(Base < End){
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

int setTextMode() {
    // default mode: 0, text mode
    volatile uint32_t *mode_control_register = (volatile uint32_t*) MODE_CONTROL_REGISTER;
    // printf("%x", curr_mode);
    *mode_control_register &= 0x00;
    
    return 0;
}

int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    // each palette is of 1 KiB size 1 KiB = 1024 B = 0x400
    // each entry is worth 4 B -> 0x4
    // color is for the RGB val, entry_number is for which entry in the palette to change
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    *palette = color;
    return 1;
}

// drawSprite(Sprite sprite, int x, int y, int z)
uint16_t drawSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
    uint32_t* small_data_addr = (volatile uint32_t *)(SMALL_SPRITE_DATA_ADDR + small_sprite_counter * SMALL_SPRITE_SIZE);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            small_data_addr[i * 16 + j] = sprite_color;
        }
    }

    uint32_t *small_control_addr = (volatile uint32_t *)(SMALL_SPRITE_CONTROL_ADDR + small_sprite_counter * SMALL_SPRITE_CONTROL_SIZE);
    *small_control_addr = sprite_control_structure;
    small_sprite_counter++;
    // }
    // // medium sprite
    // if (width < 64 && height < 64){
    //     uint32_t * medium_control_addr = (volatile uint32_t *)(MEDIUM_SPRITE_CONTROL_ADDR + medium_sprite_counter * MEDIUM_SPRITE_CONTROL_SIZE);
    //     medium_control_addr[0] = sprite_control_structure;
    //     medium_control_addr ++;
    // }
    // // a large sprite
    // if (width > 64 && height > 64)
    // {
    //     uint32_t * large_control_addr = (volatile uint32_t *)(LARGE_SPRITE_CONTROL_ADDR + large_sprite_counter * LARGE_SPRITE_CONTROL_SIZE);
    //     large_control_addr[0] = sprite_control_structure;
    //     large_sprite_counter ++;
    // }
    // // successful
    return 0;
}

void c_interrupt_handler(void){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;
}

