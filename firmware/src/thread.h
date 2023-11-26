#ifndef THREAD_H
#define THREAD_H
#include "stdint.h"
#define ARBITRARY_THREAD_LIMIT 16
//Change as needed for default.

typedef enum {
    THREAD_NEW,
    THREAD_RUNNING,
    THREAD_READY,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} ThreadState;

struct TCB {
    void* context;
    ThreadState state;
    int priority;
    void* stack_pointer;
    uint8_t* stack_base;
    unsigned int threadID;
};
extern int num_of_threads;
extern int max_threads;

int initThread(int priority, uint32_t memsize, void* param, void (*threadFunction)(void *));

#endif