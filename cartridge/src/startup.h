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
