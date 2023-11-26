#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "thread.h"
#include "memory_management.h"
#include <stdlib.h>

extern int num_of_threads;


int get_num_of_thread();
void initMutex();
void mutexLock();
void mutextUnlock();

#endif

