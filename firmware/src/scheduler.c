#include <stdlib.h>
#include <stdio.h>

#include "scheduler.h"


/*int switchThread(struct TCB* currentThread, struct TCB* nextThread) {
    void* switchedPointer = SwitchThread(currentThread->stack_base,nextThread->stack_base);
    return;
}*/ //Probably doesn't work, just wishful thinking.

int get_num_of_threads() {
    return num_of_threads;
}

int set_num_threads(int thread_num){
    max_threads = thread_num;
}

Mutex* initMutex() {
    Mutex* this_mutex = our_malloc(sizeof(Mutex));
    this_mutex->lock = 0;
    return this_mutex;
}

void mutexLock() {
    //There should be like a while loop here, but we'll need to call an assembly function.
}

void mutexUnlock() {
    //Another assembly function.
}