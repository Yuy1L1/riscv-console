#include <stdint.h>
#include <stdio.h>

#include "memory_management.h"
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
    return 0;
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
    }

    return -1;
}