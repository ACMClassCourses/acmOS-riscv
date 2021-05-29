cmake_minimum_required(VERSION 3.14)

add_library(${PROJECT_NAME}-proc OBJECT process.c trap.c elf.c)
