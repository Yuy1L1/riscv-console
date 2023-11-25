#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include "memory_management.h"

void memset(unsigned char* dest, unsigned char value, int size) {
    for (int i = 0; i < size; i++) {
        (dest)[i] = value;
    }
    return;
}

void memcpy(unsigned char* dest, unsigned char* src, int count) {
    for (int i = 0; i < count; i++) {
        (dest)[i] = (src)[i];
    }
    return;
}

void memmove(unsigned char* dest, unsigned char* src, int count) {
    if (src > dest) {
        for (int i = 0; i < count; i++) {
            (dest)[i] = (src)[i];
        }
    } else if (src < dest) {
        for (int i = count; i > 0; i--) {
            (dest)[i] = (src)[i];
        }
    }
    return;
}
