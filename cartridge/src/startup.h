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
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
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
}

uint32_t small_sprite_counter = 0;
uint32_t medium_sprite_counter = 0;
uint32_t large_sprite_counter = 0;

int setGraphicsMode() {
}

int setTextMode() {
}

int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
}

// drawSprite(Sprite sprite, int x, int y, int z)
uint16_t drawSmallSprite(uint32_t sprite_control_structure, uint8_t sprite_color) {
}

void c_interrupt_handler(void){
}

