//
// Created by Wenxin Zheng on 2021/1/31.
//

#ifndef ACMOS_SPR21_PAGETABLE_H
#define ACMOS_SPR21_PAGETABLE_H

#include <riscv.h>
#include "mm.h"

extern pagetable_t kernel_pagetable;


void pt_init();
void _pt_disp_vpn(paddr_t pa);


void pt_kern_vmmap();
int pt_map_pages(pagetable_t pagetable, vaddr_t va, paddr_t pa, uint64 size, int perm);


int pt_unmap_addrs(pagetable_t pagetable, vaddr_t va);
int pt_map_addrs(pagetable_t pagetable, vaddr_t va, paddr_t pa, int perm);
void enable_paging();
paddr_t pt_query_address(pagetable_t pagetable, vaddr_t va);

#endif  // ACMOS_SPR21_PAGETABLE_H
