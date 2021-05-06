cmake_minimum_required(VERSION 3.14)

add_library(${PROJECT_NAME}-memory OBJECT mm.c pagetable.c)
