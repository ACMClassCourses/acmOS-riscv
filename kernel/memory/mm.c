//
// Created by Wenxin Zheng on 2021/1/11.
//

#include "mm.h"
#include "buddy.h"
#include <memlayout.h>

#include "defs.h"
#include "riscv.h"
extern char end[];  // Indicate the end ptr of the image bss file
static char kernel_page_initialized[BUDDY_PAGE_SIZE * INIT_MAX_PAGES] = {0};
volatile static int allocated_page = 0;
static uint64 kernel_page_available[INIT_MAX_PAGES] = {0};
void kern_page_init() {
    allocated_page = 0;
    for (int i = 0; i < INIT_MAX_PAGES; ++i)
        kernel_page_available[i] = (uint64)(&kernel_page_initialized[i]);
}

void kern_page_test() {
    void* addr = kern_page_malloc();
    if((uint64) addr < (uint64) kernel_page_initialized) FAIL_FMT("kern_page_malloc: %s\n", "return address invalid");
    kern_page_free(addr);
    printk("Kernel initial page pool init! Page address: %lx ", addr);
    void* addr2 = kern_page_malloc();
    if((uint64) addr2 < (uint64) kernel_page_initialized) FAIL_FMT("kern_page_malloc: %s\n", "return address invalid");
    kern_page_free(addr2);
    printk("Page address: %lx\n", addr2);
    ASSERT_EQ(addr, addr2, "kernel page test address");
}

uint64 TEST_buddy_get_page_counts();
void TEST_buddy_test();
#include "answer_buddy.h"
#include "answer_mm.h"

// This function is set to initialize the kernel page table
// and set up for buddy system
void mm_init() {
    void* start_ptr = (void*)ROUNDUP((paddr_t)end, BD_LEAF_SIZE);
    void* end_ptr = (void*)PHYSTOP;
    bd_base_ptr = (void*)start_ptr;
    uint64 bd_alloc_size = end_ptr - bd_base_ptr;
    uint64 remaining_size = bd_alloc_size;
    DEBUG("buddy: memory size: %d bytes [%lx <-> %lx], about %d megabytes.\n", bd_alloc_size,
          start_ptr, end_ptr, bd_alloc_size / 1048576);
    uint64 bd_block_cnt = bd_alloc_size / BD_LEAF_SIZE;
    bd_max_size = 1;
    for (uint64 cur_block_cnt = 1; cur_block_cnt <= bd_block_cnt; cur_block_cnt *= 2, bd_max_size++)
        ;
    // Special situation: the meta block is bigger than one block, add size
    if (bd_alloc_size > BD_BLOCK_SIZE(BD_MAXSIZE)) bd_max_size += 1;
    DEBUG("buddy: memory length %d.\n", bd_max_size);

    bd_lists = (free_list_t*)start_ptr;
    start_ptr += sizeof(free_list_t) * bd_max_size;
    remaining_size -= sizeof(free_list_t) * bd_max_size;

    memset(bd_lists, 0, sizeof(free_list_t) * bd_max_size);
    uint i = 0;
    do {
        init_list_head(&bd_lists[i].list_head);
        bd_lists[i].nr_free = 0;
        ++i;
    } while (i < bd_max_size);

    bd_max_entries = remaining_size / (sizeof(page_t) + BD_LEAF_SIZE);
    DEBUG(
        "buddy: manage %d entries, meta area: %d Bytes, guard area: %d Bytes, allocation area: %d "
        "Bytes\n",
        bd_max_entries, bd_max_entries * sizeof(page_t),
        remaining_size - bd_max_entries * (sizeof(page_t) + BD_LEAF_SIZE),
        bd_max_entries * BD_LEAF_SIZE);

    bd_meta.meta_head = (uint64)start_ptr;
    bd_meta.meta_tail = (uint64)start_ptr + bd_max_entries * sizeof(page_t);
    bd_meta.first_meta_page = (page_t*)start_ptr;
    start_ptr += (remaining_size - bd_max_entries * BD_LEAF_SIZE);
    bd_meta.data_head = (uint64)start_ptr;
    bd_meta.data_tail = (uint64)end_ptr;
    bd_meta.first_page = (page_t*)start_ptr;
    bd_meta.allocate = 0;
    bd_meta.free = bd_max_entries;

    memset((void*)bd_meta.meta_head, 0, sizeof(page_t) * bd_max_entries);
    DEBUG(
        "buddy: meta info: [0x%lx <-Meta-> 0x%lx] [0x%lx <-Data-> 0x%lx]. Allocated: %d. Free: "
        "%d\n",
        bd_meta.meta_head, bd_meta.meta_tail, bd_meta.data_head, bd_meta.data_tail,
        bd_meta.allocate, bd_meta.free);
    for (i = 0; i < bd_max_entries; ++i) {
        page_t* meta_page = (page_t*)(bd_meta.meta_head + i * sizeof(page_t));
        init_list_head(&meta_page->list_node);
        meta_page->addr = bd_meta.data_head + i * BD_LEAF_SIZE;
        buddy_free_page(meta_page);
    }
    uint64 page_equ_entries = 0;
    for (i = 0; i < bd_max_size; ++i) page_equ_entries += (bd_lists[i].nr_free * (1 << i));
    ASSERT_EQ(page_equ_entries, bd_max_entries, "page merge test(initial)");
    TEST_buddy_test();
    // After initialize the page table and buddy system, enable paging
}

uint64 TEST_buddy_get_page_counts(){
    int i;
    uint64 page_equ_entries = 0;
    for (i = 0; i < bd_max_size; ++i) page_equ_entries += (bd_lists[i].nr_free * (1 << i));
    return page_equ_entries;
}

void TEST_buddy_test(){
    int idx = 0;
    for(; idx < 1000; ++idx){
        void* ptr = mm_kalloc();
        ASSERT_EQ_ON_NEG(TEST_buddy_get_page_counts() + 1, bd_max_entries, "page allocation");
        mm_kfree(ptr);
        ASSERT_EQ_ON_NEG(TEST_buddy_get_page_counts(), bd_max_entries, "page free");
    }
    int cnt = bd_max_entries;
    for(; cnt > 0; cnt--) {
        mm_kalloc();
    }
    ASSERT_EQ(TEST_buddy_get_page_counts(), 0, "page allocation test");
    for(cnt = 0; cnt < bd_max_entries; ++cnt) mm_kfree((void*)(bd_meta.data_head + PGSIZE * cnt));
    ASSERT_EQ(TEST_buddy_get_page_counts(), bd_max_entries, "page free test");
}

void uart_memory_init() {}
