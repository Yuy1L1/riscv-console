#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

void memset(unsigned char* dest, unsigned char value, int size);

void memcpy(unsigned char* dest, unsigned char* src, int count);

void memmove(unsigned char* dest, unsigned char* src, int count);

char* _sbrk(int incr);

void* our_malloc(size_t size);
#endif