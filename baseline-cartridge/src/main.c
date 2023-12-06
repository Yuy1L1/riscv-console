#include <stdint.h>
#include <stddef.h>

#include "api.h"

volatile int global = 42;
volatile uint32_t controller_status = 0;

int main() {
    int a = 4;
    int b = 12;
    uint32_t last_global = 42;
    int countdown = 1;
    uint32_t global = 42;

    while (1) {
        int c = a + b + global;
        global = GetTicks();
        
        if (global != last_global) {
            controller_status = GetController();
        }
    }
    return 0;

}
