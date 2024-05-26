//
// Created by Aurelien Levra on 25/05/2024.
//

#ifndef WOODY_INCLUDE_WOODY_H_
#define WOODY_INCLUDE_WOODY_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <strings.h>
#include "elf.h"

#include "print-infos.h"
#include "elf-utils.h"

//init routine
extern char G_FLAGS[5];
enum FLAGS {NO_FLAGS_FOR_THE_MOMENT}; //TODO: add flags for woody

void check_arguments(int argc, char **argv);
int open_file(char *string);
void get_file_stat(int fd, struct stat *st);
void map_file_to_memory(int fd, struct stat *st, void **ptr);
void check_flags(int argc, char **argv);

void infection(void *binary_pointer);
Elf64_Phdr * find_last_PT_LOAD(Elf64_Ehdr *elf_header);
Elf64_Shdr * find_last_section(Elf64_Ehdr *elf_header, Elf64_Shdr *section_headers_table, Elf64_Phdr *last_pt_load);


//exit routine
void unmap_file(void *p_void, off_t size);
void unmap_file_and_exit_with_failure(void *p_void, off_t size);



#endif //WOODY_INCLUDE_WOODY_H_
