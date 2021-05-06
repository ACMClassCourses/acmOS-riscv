//
// Created by Wenxin Zheng on 2021/1/11.
//

#ifndef ACMOS_SPR21_MM_H
#define ACMOS_SPR21_MM_H

#include "defs.h"
#include "list.h"


// static int nsizes;     // the number of entries in bd_sizes array

#define LEAF_SIZE     16                         // The smallest block size
#define MAXSIZE       (nsizes-1)                 // Largest index in bd_sizes array
#define BLK_SIZE(k)   ((1L << (k)) * LEAF_SIZE)  // Size of block at size k
#define HEAP_SIZE     BLK_SIZE(MAXSIZE)
#define NBLK(k)       (1 << (MAXSIZE-k))         // Number of block at size k
#define ROUNDUP(n,sz) (((((n)-1)/(sz))+1)*(sz))  // Round up to the next multiple of sz

// BD_LEAF_SIZE as 4096 bytes, same size with a page
#define BD_LEAF_SIZE     4096
#define BD_MAXSIZE       (bd_max_size-1)
// Size of block at size k
#define BD_BLOCK_SIZE(k) ((1L << (k)) * BD_LEAF_SIZE)

#define BD_PAGE_IN_USE (1L)
#define BD_PAGE_FREE (1L << 1)


void mm_init();
void uart_memory_init();
void* mm_kalloc();

// Kernel Memory Function
void kern_page_init();
void kern_page_test();
void* kern_page_malloc();
void kern_page_free(void* ptr);
extern void pt_init();

#endif  // ACMOS_SPR21_MM_H
