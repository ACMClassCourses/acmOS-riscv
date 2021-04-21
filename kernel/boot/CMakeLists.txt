cmake_minimum_required(VERSION 3.14)

add_library(${PROJECT_NAME}-boot OBJECT start.c kernelvec.S)
