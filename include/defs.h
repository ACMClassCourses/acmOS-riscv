//
// Created by Wenxin Zheng on 12/6/20.
//

#ifndef ACMOS_SPR21_DEFS_H
#define ACMOS_SPR21_DEFS_H

// Type Definitions
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

typedef uint64 paddr_t;
typedef uint64 vaddr_t;

typedef uint8 byte;

typedef char bool;
#define true (1)
#define false (0)
#define NULL 0
// some constants: defined in params.h
#define NPROC 10  // maximum number of processes
#define NCPU 8  // maximum number of CPUs
#define NOFILE 16  // open files per process
#define NFILE 100  // open files per system
#define NINODE 50  // maximum number of active i-nodes
#define NDEV 10  // maximum major device number
#define ROOTDEV 0  // device number of file system root disk
#define MAXARG 32  // max exec arguments
#define MAXOPBLOCKS 10  // max # of blocks any FS op writes
#define LOGSIZE (MAXOPBLOCKS * 3)  // max data blocks in on-disk log
#define NBUF (MAXOPBLOCKS * 3)  // size of disk block cache
#define FSSIZE 2000  // size of file system in blocks
#define MAXPATH 128  // maximum file path name
#define NDISK 2
#define INIT_MAX_PAGES 16
#define BUG(str)                                                                    \
    do {                                                                            \
        printk("\033[31mKernel Panic: %s:%d %s\033[0m\n", __func__, __LINE__, str); \
        for (;;) {                                                                  \
        }                                                                           \
    } while (0)

#define BUG_FMT(fm, ...)                                             \
    do {                                                             \
        printk("\033[31mKernel Panic: %s:%d: "fm"\033[0m\n", __func__, __LINE__, ##__VA_ARGS__); \
        for (;;) {                                                   \
        }                                                            \
    } while (0)


#define DEBUG(fm, ...) printk("\033[33m[debug: %s, %s:%d]: "fm"\033[0m", __func__, __FILENAME__, __LINE__, ##__VA_ARGS__);

#define PASS_FMT(fm, ...)                                                        \
    do {                                                                         \
        printk("\033[32m[Pass: %s, %s:%d]: ", __func__, __FILENAME__, __LINE__); \
        printk(fm, __VA_ARGS__);                                                 \
        printk("\033[0m");                                                       \
    } while (0)

#define PASS(str)                                \
    do {                                         \
        printk("\033[32mPass %s\033[0m\n", str); \
    } while (0)

#define FAIL(str)                                \
    do {                                         \
        printk("\033[31mFail %s\033[0m\n", str); \
    } while (0)

#define FAIL_FMT(fm, ...)                                                        \
    do {                                                                         \
        printk("\033[31m[Fail: %s, %s:%d]: ", __func__, __FILENAME__, __LINE__); \
        printk(fm, __VA_ARGS__);                                                 \
        printk("\033[0m");                                                       \
    } while (0)

#define ASSERT_EQ(x, y, str)                                                 \
    do {                                                                     \
        if ((x) != (y)) {                                                    \
            FAIL_FMT("Expect " #x " == " #y ", receive %d, %d\n", (x), (y)); \
            for (;;) {                                                       \
            }                                                                \
        } else                                                               \
            PASS_FMT(#str "%s", "\n");                                       \
    } while (0)

#define ASSERT_EQ_ON_NEG(x, y, str)                                                 \
    do {                                                                     \
        if ((x) != (y)) {                                                    \
            FAIL_FMT("Expect " #x " == " #y ", receive %d, %d\n", (x), (y)); \
            for (;;) {                                                       \
            }                                                                \
        }                                 \
    } while (0)
#define BIT(x) (1UL << (x))
#define CLEAR_BIT(target, flag) (target) &= ~(flag)
#define SET_BIT(target, flag) (target) |= (flag)
#define offsetof(TYPE, MEMBER) ((u64) & ((TYPE *)0)->MEMBER)
#define container_of(ptr, type, field) ((type *)((void *)(ptr) - (uint64)(&(((type *)(0))->field))))

#define container_of_safe(ptr, type, field)             \
    ({                                                  \
        typeof(ptr) __ptr = (ptr);                      \
        type *__obj = container_of(__ptr, type, field); \
        (__ptr ? __obj : NULL);                         \
    })

#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define IS_ALIGNED(x, a) (((x) & ((typeof(x))(a)-1)) == 0)
#define KERNEL_PANIC(str) BUG(str)

// some platform related constant
// mainly cache
/* Different platform may have different cacheline size and may have some features like prefetch */
#define CACHE_LINE_SIZE 64
#define r_align(n, r) (((n) + (r)-1) & -(r))
#define cache_align(n) r_align(n, CACHE_LINE_SIZE)
#define pad_to_cache_line(n) (cache_align(n) - (n))

// define the maximize lock count
#define MAXLOCKS 1024

// GCC builtins
#define sync_synchronize() __sync_synchronize()
#define sync_lock_test_and_set(ptr, inc) __sync_lock_test_and_set(ptr, inc)
#define sync_lock_release(ptr) __sync_lock_release(ptr)

// Debug helper
#define KERN_FREED 0xDD
#define KERN_ALLOCED 0xCC

#ifndef KBASE
#define KBASE 0x80000000
#endif
// 30M
#define PHYSICAL_MEM_START (30 * 1024 * 1024)
#define phys_to_virt_addr(x) ((vaddr_t)((paddr_t)(x) + KBASE))
#define START_VADDR phys_to_virt_addr(PHYSICAL_MEM_START)

// string.h: symbol in common/string.c(h)
void memset(void *dst, int c, uint n);
int memcmp(const void *v1, const void *v2, uint n);
void *memmove(void *dst, const void *src, uint n);
void *memcpy(void *dst, const void *src, uint n);
int strncmp(const char *p, const char *q, uint n);
char *strncpy(char *s, const char *t, int n);
// Like strncpy but guaranteed to NUL-terminate.
char *safestrcpy(char *s, const char *t, int n);
int strlen(const char *s);

// printk.h
void printk(const char *fmt, ...);
// helper function for debugging
_Noreturn void suspend();

// list.c

struct list {
    struct list *next;
    struct list *prev;
};

#endif  // ACMOS_SPR21_DEFS_H
