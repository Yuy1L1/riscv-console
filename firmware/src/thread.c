#include <stdlib.h>
#include <stdio.h>

#include "thread.h"
#include "memory_management.h"
volatile int next_thread_id = 0;
int num_of_threads = 0;
int max_threads = ARBITRARY_THREAD_LIMIT;

int initThread(int priority, uint32_t memsize, void* param, void (*threadFunction)(void *)) {
    if (num_of_threads >= max_threads) {
        return -1; //Too many threads.
    }
    uint8_t* stack_base = our_malloc(memsize);
    struct TCB* tcb = our_malloc(sizeof(struct TCB));
    tcb->state = THREAD_NEW;
    tcb->priority = priority;
    tcb->threadID = next_thread_id++;
    tcb->stack_base = stack_base;
    tcb->stack_pointer = InitThread(tcb->stack_base+memsize, param);
    num_of_threads++;
}

