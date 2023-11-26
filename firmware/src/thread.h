#ifndef THREAD_H
#define THREAD_H

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

int initThread(int priority, uint32_t memsize, void* param, void (*threadFunction)(void *));

#endif