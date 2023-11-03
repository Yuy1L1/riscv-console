#include <stdint.h>
#include <stdio.h>
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

#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))

volatile uint32_t *INTERRUPT_ENABLE_REGISTER = (volatile uint32_t*) (0x40000000);
volatile uint32_t *INTERRUPT_PENDING_REGISTER = (volatile uint32_t*) (0x40000004);

void delay_ms(unsigned int milliseconds) {
    unsigned int factor = 1000; // This factor needs calibration
    volatile unsigned int count;

    for (; milliseconds > 0; milliseconds--) {
        for (count = factor; count > 0; count--) {
            // Empty loop with volatile count to prevent optimization.
        }
    }
}

void init(void) {
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

    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable
    // Enable command interrupts
    *INTERRUPT_ENABLE_REGISTER = *INTERRUPT_ENABLE_REGISTER | (1 << 2);
    // Enable video interrupts
    // *INTERRUPT_ENABLE_REGISTER = *INTERRUPT_ENABLE_REGISTER | (1 << 1);
    
    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
}

extern volatile int global;
extern volatile uint32_t controller_status;

#define SMALL_SPRITE_PALETTE_ADDR 0x500F3000
int setSmallColorPalette(uint32_t palette_number, uint32_t color, uint32_t entry_number) {
    // each palette is of 1 KiB size 1 KiB = 1024 B = 0x400
    // each entry is worth 4 B -> 0x4
    // color is for the RGB val, entry_number is for which entry in the palette to change
    volatile uint32_t *palette = (volatile uint32_t *) (SMALL_SPRITE_PALETTE_ADDR + (0x400) * palette_number + (0x4) * entry_number);  
    *palette = color;
    return 1;
}

void c_interrupt_handler(void){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;
    
    if ((*INTERRUPT_PENDING_REGISTER >> 2) & 1 && (*INTERRUPT_ENABLE_REGISTER >> 2) & 1) {
        setSmallColorPalette(0, 0xFF00F0FF, 2); // cyan
        *INTERRUPT_PENDING_REGISTER = 4;
        delay_ms(500);
        setSmallColorPalette(0, 0xFFFF00FF, 2); // magenta
    }
}

// void cmd_interrupt() {
//     interrupt_pending_register = interrupt_pending_register & 0x00000003;
// }

// uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
// if (((interrupt_pending_register) & 0x4) >> 2) {
//     setSmallColorPalette(0, 0xFFFFFFFF, 2)
// }


volatile int video_counter = 0;

void video_interrupt() {
    video_counter += 1;
    uint32_t interrupt_pending_register = *((volatile uint32_t * ) INTERRUPT_PENDING_REGISTER);
    // need to write 1 to the video bit in IPR, which is the second bit.
    interrupt_pending_register = interrupt_pending_register & 0x00000002;
}

uint32_t c_system_call(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call) {
    if (call == 1) {
        return global;
    } else if (call == 2) {
        return controller_status;
    }

    return -1;
}