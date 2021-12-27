cmake_minimum_required(VERSION 3.11)
project(SIFE C)

set(CMAKE_C_STANDARD 99)

add_compile_definitions(
    MAX_PATH_LENGTH=200 # Max Length for each files/directories path
)

if(UNIX AND NOT APPLE)
    add_compile_definitions(
        _GNU_SOURCE         # ISO C89, ISO C99, POSIX.1, POSIX.2, BSD, SVID, X/Open, LFS, and GNU extensions.
    )
endif()

add_executable(
    sife
    src/main.c
    src/console.h
    src/functions.c     src/functions.h
    src/wipe.c          src/wipe.h
)

target_link_libraries(sife m)