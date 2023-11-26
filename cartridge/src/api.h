#ifndef __INCLUDE_STDINT__
#define __INCLUDE_STDINT__
#include <stdint.h>
#endif

#ifndef API_H
#define API_H

#define GET_TICKS 0x00000001
#define CONTROLLER_STATUS 0x00000002
#define MEMORY_MOVE 0x00000003

uint32_t systemcall(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call);

uint32_t GetTicks();

uint32_t GetController();

uint32_t MemMove(unsigned char* dest, unsigned char* src, int count);

#endif