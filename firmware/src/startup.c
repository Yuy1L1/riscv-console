#include <stdint.h>
#include <stdio.h>

#include "memory_management.h"
#include "sprite_management.h"
#include "interrupt_handler.h"
#include "thread.h"

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

void do_nothing() {
    delay_ms(1000);
    int *p = NULL;  // Initialize a pointer to NULL
    *p = 10;        // Try to write to the NULL pointer
    return;
}

void init(void) {
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    int new_thread = initThread(1, 100, NULL, &do_nothing);

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
    *INTERRUPT_ENABLE_REGISTER = *INTERRUPT_ENABLE_REGISTER | (1 << 1);
    
    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
}

extern volatile int global;
extern volatile uint32_t controller_status;

void c_interrupt_handler(void) {
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;

    handle_cmd_interrupt();
    handle_video_interrupt();
}

uint32_t c_system_call(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call) {
    if (call == 1) {
        return global;
    } else if (call == 2) {
        return controller_status;
    } else if (call == 3) {
        memmove((unsigned char*)arg0, (unsigned char*)arg1, (int)arg2);
        return 1;
    } else if (call == 4) {
        memset((unsigned char*)arg0, (unsigned char)arg1, (int)arg2);
    } else if (call == 5) {
        memcpy((unsigned char*)arg0, (unsigned char*)arg1, (int)arg2);
    } else if (call == 6) {
        // TODO -> implement the following syscalls
        // Functionality for OPERATION_SETTING
    } else if (call == 7) {
        // Functionality for PHYSICS_SIMULATION
    } else if (call == 8) {
        // Functionality for SECURITY_CHECK
    } else if (call == 9) {
        // Functionality for IMPORT_EXPORT
    } else if (call == 10) {
        // Functionality for ADD_HOTKEY
    } else if (call == 11) {
        // Functionality for MULTIPLE_KEY_CHECK
    } else if (call == 12) {
        // Functionality for REMOVE_HOTKEY
    } else if (call == 13) {
        handle_cmd_interrupt();
    } else if (call == 14) {
        handle_video_interrupt();
    } else if (call == 15) {
        // Functionality for CARTRIDGE_INTERRUPT
    } else if (call == 16) {
        // Functionality for ILLEGAL_INST_INTERRUPT
    } else if (call == 17) {
        // Functionality for TIMER_INTERRUPT
    } else if (call == 18) {
        return setGraphicsMode();
    } else if (call == 19) {
        return setSmallColorPalette((uint32_t)arg0, (uint32_t)arg1, (uint32_t)arg2);
    } else if (call == 20) {
        // changeSmallSpriteColor();
    } else if (call == 21) {
        return drawSmallSprite((uint32_t)arg0, (uint8_t)arg1);
    } else if (call == 22) {
        eraseSmallSprite((uint8_t)arg0);
    } else if (call == 23) {
        moveSmallSprite((uint8_t)arg0, arg1, (uint8_t)arg2);
    } else if (call == 24) {
        return setMediumColorPalette(arg0, arg1, arg2);
    } else if (call == 25) {
        return drawMediumSprite(arg0, (uint8_t)arg1);
    } else if (call == 26) {
        eraseMediumSprite((uint8_t)arg0);
    } else if (call == 27) {
        changeMediumSpriteColor();
    } else if (call == 28) {
        return setLargeColorPalette(arg0, arg1, arg2);
    } else if (call == 29) {
        return drawLargeSprite(arg0, (uint8_t)arg1);
    } else if (call == 30) {
        eraseLargeSprite((uint8_t)arg0);
    } else if (call == 31) {
        changeLargeSpriteColor();
    } else if (call == 32) {
        return setBackgroundColorPalette(arg0, arg1, arg2);
    } else if (call == 33) {
        return changeBackgroundColorPalette();
    } else if (call == 34) {
        return drawBackgroundSprite(arg0, (uint8_t)arg1, arg2);
    } else if (call == 35) {
        eraseBackgroundSprite((uint8_t)arg0, arg2);
    } else if (call == 36) {
        // EMULATOR
    } else if (call == 37) {
        // CHECK_LOCATION
    } else if (call == 38) {
        // CHECK_CONTROLLER
    } else if (call == 39) {
        // NUM_CONTROLLERS
    } else if (call == 40) {
        // RAND_NUM
    } else if (call == 41) {
        // GET_TICKS_UTILS
    } else if (call == 42) {
        // GET_CONTROLLER_UTILS
    } else if (call == 43) {
        // LOAD_PREVIOUS_STATE
    } else if (call == 44) {
        // SET_TEXT_MODE
    } else if (call == 45) {
        // INIT_THREAD
        return initThread((int)arg0, arg1, (void*)arg2, (void (*)(void *))arg3);
    }
    return -1;
}