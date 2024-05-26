//
// Created by Aurelien Levra on 26/05/2024.
//


#ifndef WOODY_INCLUDE_PRINT_INFOS_H_
#define WOODY_INCLUDE_PRINT_INFOS_H_

#include "woody.h"
typedef struct s_elf_part {
void *start;
void *end;
int size;
int alignment;
char *name;
} t_elf_part;

void print_ELF_file_overview(Elf64_Ehdr * elf_header);
void print_ELF_header(Elf64_Ehdr *elf_header);
int scan_elf_parts(Elf64_Ehdr *elf_header,
					 Elf64_Shdr *section_headers_table,
					 Elf64_Phdr *Program_headers_table,
					 t_elf_part **elf_parts);

void display_elf_parts(t_elf_part *elf_parts, int size);
#endif //WOODY_INCLUDE_PRINT_INFOS_H_
