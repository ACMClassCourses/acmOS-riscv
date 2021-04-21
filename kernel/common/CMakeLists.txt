cmake_minimum_required(VERSION 3.14)

add_library(${PROJECT_NAME}-common OBJECT lock.c printk.c uart.c kernelvec.S string.c)
