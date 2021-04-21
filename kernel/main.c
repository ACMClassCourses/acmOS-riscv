//
// Created by Wenxin Zheng on 2021/1/9.
//

#include <defs.h>
#include <riscv.h>

#include "common/lock.h"
#include "common/printk.h"
#include "common/uart.h"

volatile static int started = 0;



void main(){
    if(cpuid() == 0){ // primary cpu
        uart_init();
        printk("UART Finish initialization. 1013 decimal is %x hex, %b binary, %d dec.\n",
               1013, 1013, 1013);
        printk("kernel lock address: 0x%lx\n", &big_kernel_lock);
        printk("UART String: %s\n", "Hello, world!");
        kernel_lock_init();
        TEST_lock_test();
        suspend();
        sync_synchronize();
        started = 1;
    } else {
        while(started == 0);
        sync_synchronize();
    }
}