cmake_minimum_required(VERSION 3.16)
project(acmOS_spr21 C ASM)

message(STATUS "operating system: ${CMAKE_SYSTEM}")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CROSS_COMPILE_PREFIX "riscv64-linux-gnu-")
else()
    set(CROSS_COMPILE_PREFIX "riscv64-unknown-elf-")
endif ()
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER "${CROSS_COMPILE_PREFIX}gcc")
set(CMAKE_ASM_COMPILER "${CROSS_COMPILE_PREFIX}gcc")

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "ASM Compiler: ${CMAKE_ASM_COMPILER}")


execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-file-name=include
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE C_COMPILER_INCLUDE
)

set(CMAKE_C_FLAGS_RELEASE "-O1 -g2")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

set(CMAKE_C_FLAGS
        "-Werror -Wall -O -fno-omit-frame-pointer -ggdb -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
set(CMAKE_LINK_LIBRARY_FLAG "-z max-page-size=4096")

set(KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/kernel")
set(link_script "${KERNEL_PATH}/kernel.ld")
set(USER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/user")

set(KERNEL_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(KERNEL_ASM_PATH "${KERNEL_PATH}/asm")
set(KERNEL_BOOT_PATH "${KERNEL_PATH}/boot")
set(KERNEL_COMMON_PATH "${KERNEL_PATH}/common")

include_directories("${KERNEL_INCLUDE_PATH}")
add_subdirectory("${KERNEL_PATH}")
add_subdirectory("${KERNEL_BOOT_PATH}")
add_subdirectory("${KERNEL_ASM_PATH}")
add_subdirectory("${KERNEL_COMMON_PATH}")


add_executable(kernel.img
        $<TARGET_OBJECTS:${PROJECT_NAME}-asm>
        $<TARGET_OBJECTS:${PROJECT_NAME}-arch>
        $<TARGET_OBJECTS:${PROJECT_NAME}-common>
        $<TARGET_OBJECTS:${PROJECT_NAME}-boot>
        )

set_property(
        TARGET kernel.img
        APPEND_STRING
        PROPERTY
        LINK_FLAGS
        "-T ${link_script}"
)
