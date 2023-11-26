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