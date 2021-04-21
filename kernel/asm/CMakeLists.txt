cmake_minimum_required(VERSION 3.14)

file(
        GLOB
        tmpfiles
        "boot.S"
)

list(APPEND files ${tmpfiles})
add_library(${PROJECT_NAME}-asm OBJECT boot.S)

