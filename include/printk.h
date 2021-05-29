//
// Created by Wenxin Zheng on 2021/1/9.
//
// The design is same to ChCore

#include "uart.h"

#ifndef ACMOS_SPR21_PRINTK_H
#define ACMOS_SPR21_PRINTK_H

void printk(const char* fmt, ...);
// helper function for debugging
_Noreturn void suspend();

#endif  // ACMOS_SPR21_PRINTK_H
