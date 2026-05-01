#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CLINT_MTIMECMP ((volatile uint64_t*)0x02004000)
#define CLINT_MTIME    ((volatile uint64_t*)0x0200BFF8)

void set_timer(uint64_t interval) {
    *CLINT_MTIMECMP = *CLINT_MTIME + interval;
}

void __attribute__((interrupt("machine"))) reset1() {
    uint32_t cause;
    asm volatile("csrr %0, mcause" : "=r"(cause));
    
    //printf("\n");
    
    int is_irq = (cause >> 31);
    if (is_irq) {
        cause &= 0x7FFFFFFF;
        if (cause == 7) {
            //printf("Timer\n");
            set_timer(2000);
            return;
        } else {
            printf("Unknown interrupt\n");
            printf("Cause: %x\n", cause);
            exit(0);
        }
    } else {
        printf("Reset\n");
        printf("Cause: %x\n", cause);
        exit(0);
    }

    return;
}

int benchmark_main() {
    // Set the address of our interrupt/trap handler
    asm volatile ("csrw mtvec, %0" : : "r"(reset1));
    
    set_timer(2000);
    
    // Enable Machine Timer Interrupt (bit 7) in mie
    asm volatile("csrs mie, %0" : : "r"(0x80));
    
    // Enable Global Interrupts (bit 3) in mstatus
    asm volatile("csrs mstatus, %0" : : "r"(0x8));
    
    printf("Hello!\n");
    
    int index = 0;
    while(1) {
        printf("%d ", index);
        index += 1;
        if (index == 1000) break;
    }
    
    return 0;
}

