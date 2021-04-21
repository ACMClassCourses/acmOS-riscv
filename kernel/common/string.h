//
// Created by Wenxin Zheng on 2021/1/23.
//

#ifndef ACMOS_SPR21_STRING_H
#define ACMOS_SPR21_STRING_H

#include "defs.h"

void memset(void *dst, int c, uint n);
int memcmp(const void *v1, const void *v2, uint n);
void *memmove(void *dst, const void *src, uint n);
void *memcpy(void *dst, const void *src, uint n);
int strncmp(const char *p, const char *q, uint n);
char *strncpy(char *s, const char *t, int n);
// Like strncpy but guaranteed to NUL-terminate.
char *safestrcpy(char *s, const char *t, int n);
int strlen(const char *s);

#endif  // ACMOS_SPR21_STRING_H
