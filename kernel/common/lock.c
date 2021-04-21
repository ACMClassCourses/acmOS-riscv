//
// Created by Wenxin Zheng on 2021/1/9.
//

#include "lock.h"

#include <riscv.h>

static struct lock* locks[MAXLOCKS];
static int nlock = 0;

struct lock big_kernel_lock;

#include "answer_locks.h"


void kernel_lock_init(void){
    lock_init(&big_kernel_lock);
}
void lock_kernel(void){
    acquire(&big_kernel_lock);
}
void unlock_kernel(void){
    release(&big_kernel_lock);
}

void TEST_lock_test(){
    ASSERT_EQ(try_acquire(&big_kernel_lock), 0, "kernel lock: try_acquire");
    ASSERT_EQ(is_locked(&big_kernel_lock), 1, "kernel lock: is_locked");
    release(&big_kernel_lock);
    ASSERT_EQ(is_locked(&big_kernel_lock), 0, "kernel lock: is_locked");
}