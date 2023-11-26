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

extern char _heap_base[];
extern char _stack[];

char* _sbrk(int numbytes) {
  static char *heap_ptr = NULL;
  char *base;

  if (heap_ptr == NULL) {
    heap_ptr = (char *)&_heap_base;
  }

  if((heap_ptr + numbytes) <=_stack) {
    base = heap_ptr;
    heap_ptr += numbytes;
    return (base);
  }
  else {
    //errno = ENOMEM;
    return NULL;
  }
}

static inline size_t align_size(size_t size) {
    const size_t alignment = 8;
    return (size + (alignment - 1)) & ~(alignment - 1);
}

void* our_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    static const size_t ALIGNMENT = 8;
    static const size_t ALLOC_BIT = 1;
    size_t ALIGNED_SIZE_T = align_size(sizeof(size_t));

    size_t blk_size = align_size(size + ALIGNED_SIZE_T);
    size_t *header = (size_t *)_sbrk(blk_size);

    if (header == (void *)-1) {
        return NULL;
    }

    // TODO(justin): Writing the header kills the program for some reason
    // *header = blk_size | ALLOC_BIT;

    return (void *)(header + 1);
}
