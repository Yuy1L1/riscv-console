#include "api.h"

uint32_t GetTicks() {
    return systemcall(0, 0, 0, 0, 0, GET_TICKS);
}

uint32_t GetController() {
    return systemcall(0, 0, 0, 0, 0, CONTROLLER_STATUS);
}

uint32_t MemMove(unsigned char* dest, unsigned char* src, int count) {
    return systemcall((uint32_t)dest, (uint32_t) src, (uint32_t) count, 0, 0, MEMORY_MOVE);
}