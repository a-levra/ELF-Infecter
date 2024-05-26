cmake_minimum_required(VERSION 3.28)
project(woody C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(include/elf-utils)

add_executable(woody
        include/woody.h
        include/elf.h
        src/main.c
        src/init_and_exit_routine.c
        src/parse_args.c
        src/print-infos/print_ELF_file_overview.c
        include/print-infos.h
        src/elf-utils/elf-getters.c
        include/elf-utils/elf-utils.h
        include/elf-utils/elf-getters.h)
