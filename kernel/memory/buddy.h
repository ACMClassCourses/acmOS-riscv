#ifndef ACMOS_SPR21_BUDDY_H
#define ACMOS_SPR21_BUDDY_H
#include "riscv.h"
#include "list.h"

typedef struct free_list{
    struct list_head list_head;
    uint64 nr_free;
} free_list_t;

typedef struct page{
    struct list_head list_node;
    uint32 flags;
    uint32 orders;
    uint64 addr;
} page_t;

typedef struct bd_meta{
    uint64 data_head, data_tail;
    uint64 allocate, free;
    uint64 meta_head, meta_tail;
    page_t *first_meta_page, *first_page;
}bd_meta_t;

static bd_meta_t bd_meta;
static free_list_t *bd_lists;
static void* bd_base_ptr;
static uint bd_max_size;
static uint64 bd_max_entries;
#endif