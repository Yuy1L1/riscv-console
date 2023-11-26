#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "thread.h"
#include "memory_management.h"
#include <stdlib.h>

//int switchThread(struct TCB* currentThread, struct TCB* nextThread); Probably doesn't work, just wishful thinking.
int get_num_of_threads();
int set_num_threads(int thread_num);
void initMutex();
void mutexLock();
void mutextUnlock();

#endif

