#include <stdint.h>
#include <stdarg.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *CATRIDGE_STATUS_REGISTER = (volatile uint32_t *)(0x4000001C);
uint32_t GetTicks(void);
uint32_t GetController(void);

uint32_t colorEntry = 0xffffffff;

typedef void (*FunctionPtr)(void);

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;

    while (1) {
        // check if the CIS (lowest bit) is 1
        if (*CATRIDGE_STATUS_REGISTER & 0x1) {
            FunctionPtr func = (FunctionPtr)((*CATRIDGE_STATUS_REGISTER) & 0xFFFFFFFC);
            // start the application
            func();
        }
    }

    // upon successfully exit
    return 0;
}
