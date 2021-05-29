//
// Created by Wenxin Zheng on 2021/1/31.
//

#include "pagetable.h"
#include "memlayout.h"
#include "riscv.h"
// Switch h/w page table register to the kernel's page table,
// and enable paging.

extern char endTextSect[]; // indicate the end of the text section
pagetable_t kernel_pagetable;
#include "answer_pgt.h"

void pt_init() {
    kernel_pagetable = (pagetable_t)mm_kalloc();
    memset(kernel_pagetable, 0, BD_LEAF_SIZE);
    pt_kern_vmmap();

    enable_paging();
}

void pt_kern_vmmap(){
    int ret = pt_map_pages(kernel_pagetable, UART0, UART0, PGSIZE, PTE_R | PTE_W);
    uint64 idx = UART0;
    for(; idx < UART0 + PGSIZE; idx = idx + PGSIZE / 2){
        paddr_t mapped_pa = pt_query_address(kernel_pagetable, idx);
        ASSERT_EQ(idx,  mapped_pa,"kernel map");
    }
    ASSERT_EQ(ret, 0, "map UART0 page");
    ret = pt_map_pages(kernel_pagetable, KERNBASE, KERNBASE, (uint64)endTextSect - KERNBASE, PTE_R | PTE_X);
    ASSERT_EQ(ret, 0, "map kernel text");
    idx = 0;
    for(; idx < (uint64)endTextSect - KERNBASE; idx = idx + PGSIZE / 2){
        paddr_t mapped_pa = pt_query_address(kernel_pagetable, idx + KERNBASE);
        ASSERT_EQ(idx + KERNBASE,  mapped_pa,"kernel map");
    }
    ret = pt_map_pages(kernel_pagetable, (uint64)endTextSect, (uint64)endTextSect, PHYSTOP - (uint64)endTextSect, PTE_R | PTE_W);
    ASSERT_EQ(ret, 0, "map kernel RAM");
    idx = 0;
    for(; idx < PHYSTOP - (uint64)endTextSect; idx = idx + PGSIZE / 2){
        paddr_t mapped_pa = pt_query_address(kernel_pagetable, idx + (uint64)endTextSect);
        ASSERT_EQ_ON_NEG(idx + (uint64)endTextSect,  mapped_pa,"kernel RAM map");
    }
    pt_unmap_addrs(kernel_pagetable, (uint64)kernel_pagetable);
    paddr_t unmapped_pa = pt_query_address(kernel_pagetable, (uint64) kernel_pagetable);
    ASSERT_EQ(unmapped_pa, 0, "unmap");
    pt_map_addrs(kernel_pagetable, (uint64)kernel_pagetable, (uint64)kernel_pagetable, PTE_R | PTE_W);
}