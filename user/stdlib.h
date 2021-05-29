#include "answer_printk.h"
typedef __builtin_va_list va_list;
#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

#define SYS_EXIT 1
#define SYS_PUTC 2
#define SYS_YIELD 3

// 参考 asm volatile 的插入方式，将以下3个syscall函数插入完整
// 参考资料：https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
// 注意：我们是 RISCV 架构，最后进入内核态需要 ecall 指令
void sys_putc(char val){
    asm volatile("":::"memory"); 
}

void sys_exit(int value){
    asm volatile("":::"memory"); 
}

void sys_yield(){
    asm volatile("":::"memory"); 
}

void printk_format(const char* format, va_list args) {
    int d, i, base;
    char c, *s;
    unsigned long p;
    long l;
    int escape_mode = 0;

    /* Iterate over the format list. */
    for (i = 0; format[i] != 0; i++) {
        /* Handle simple characters. */
        if (!escape_mode && format[i] != '%') {
            if (format[i] == '\n') {
                sys_putc('\n');
                sys_putc('\r');
            } else {
                sys_putc(format[i]);
            }
            continue;
        }

        /* Handle the percent escape character. */
        if (format[i] == '%') {
            if (!escape_mode) {
                /* Entering escape mode. */
                escape_mode = 1;
            } else {
                /* Already in escape mode; print a percent. */
                sys_putc(format[i]);
                escape_mode = 0;
            }
            continue;
        }

        /* Handle the modifier. */
        switch (format[i]) {
            /* Ignore printf modifiers we don't support. */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '.':
                break;

                /* String. */
            case 's':
                s = va_arg(args, char*);
                printk_write_string(s);
                escape_mode = 0;
                break;

                /* Pointers. */
            case 'p':
                p = va_arg(args, unsigned long);
                printk_write_num(16, p, 0);
                escape_mode = 0;
                break;

                /* Hex number. */
            case 'x':
                d = va_arg(args, int);
                printk_write_num(16, d, 0);
                escape_mode = 0;
                break;

                /* Decimal number. */
            case 'd':
                d = va_arg(args, int);
                if (d >= 0)
                    printk_write_num(10, d, 0);
                else /* negtive number */
                    printk_write_num(10, -d, 1);
                escape_mode = 0;
                break;
            case 'u':
                d = va_arg(args, int);
                printk_write_num(10, d, 0);
                escape_mode = 0;
                break;

                /* Hex number. */
            case 'b':
                d = va_arg(args, int);
                printk_write_num(2, d, 0);
                escape_mode = 0;
                break;

                /* Character. */
            case 'c':
                c = va_arg(args, int);
                sys_putc(c);
                escape_mode = 0;
                break;

                /* Long number. */
            case 'l': {
                int neg = 0;

                l = va_arg(args, long);

                switch (format[++i]) {
                    case 'd':
                        base = 10;
                        if (l < 0) {
                            neg = 1;
                            l = -l;
                        }
                        break;
                    case 'u':
                        base = 10;
                        break;

                    case 'x':
                        base = 16;
                        break;

                    case 'b':
                        base = 2;
                        break;

                    default:
                        sys_putc('?');
                        continue;
                }

                printk_write_num(base, l, neg);
                escape_mode = 0;
                break;
            }

                /* Unknown. */
            default:
                sys_putc('?');
                escape_mode = 0;
                break;
        }
    }
}

void print(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    printk_format(fmt, va);
    va_end(va);
}