//
// Created by Wenxin Zheng on 2021/3/5.
//

#ifndef ACMOS_SPR21_PROCESS_H
#define ACMOS_SPR21_PROCESS_H

#include <list.h>
#include <pagetable.h>

typedef enum state { UNUSED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE, IDLE } process_state_t;
typedef enum file_type{PUTC} file_type_t;

typedef struct process {
    struct list_head thread_list;
    process_state_t process_state;
    // 以下部分请根据自己的需要自行填充

} process_t;

// 状态可以根据自己的需要进行修改
typedef process_state_t thread_state_t;

typedef struct thread {
    struct list_head process_list_thread_node;
    thread_state_t thread_state;
    struct list_head sched_list_thread_node;
    // 以下部分请根据自己的需要自行填充

} thread_t;

process_t *alloc_proc(const char* bin, thread_t *thr);
bool load_thread(file_type_t type);
void sched_enqueue(thread_t *target_thread);
thread_t *sched_dequeue();
bool sched_empty();
void sched_start();
void sched_init();
void proc_init();
void trap_init_vec();
#endif  // ACMOS_SPR21_PROCESS_H
