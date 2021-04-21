//
// Created by Wenxin Zheng on 2021/1/9.
//

#include "printk.h"
#include "defs.h"


#ifndef ACMOS_SPR21_LOCK_H
#define ACMOS_SPR21_LOCK_H

// mutual exclusion lock
// default: spin lock
// TODO: make it to ticket lock
struct lock{
    uint locked;
    uint cpuid;
};

// lock interface
int lock_init(struct lock *lock);
void acquire(struct lock *lock);
int try_acquire(struct lock *lock);
void release(struct lock *lock);
int is_locked(struct lock *lock);

// global kernel lock
extern struct lock big_kernel_lock;
void kernel_lock_init(void);
void lock_kernel(void);
void unlock_kernel(void);

void TEST_lock_test();

// private for spin lock
int holding_lock(struct lock *lock);

#endif  // ACMOS_SPR21_LOCK_H
