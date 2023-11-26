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

// extern char _heap_base[];
// extern char _stack[];

// char* _sbrk(int numbytes) {
//   static char *heap_ptr = NULL;
//   char *base;

//   if (heap_ptr == NULL) {
//     heap_ptr = (char *)&_heap_base;
//   }

//   if((heap_ptr + numbytes) <=_stack) {
//     base = heap_ptr;
//     heap_ptr += numbytes;
//     return (base);
//   }
//   else {
//     //errno = ENOMEM;
//     return NULL;
//   }
// }