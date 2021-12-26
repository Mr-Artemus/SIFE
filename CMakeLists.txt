cmake_minimum_required(VERSION 3.11)
project(SIFE C)

set(CMAKE_C_STANDARD 99)

add_compile_definitions(
        MAX_PATH_LENGTH=200 # Max Length for each files/directories path
)

add_executable(
        sife
        src/main.c
        src/console.h
        src/functions.c     src/functions.h
        src/wipe.c          src/wipe.h
)