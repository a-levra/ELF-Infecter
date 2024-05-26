//
// Created by Aurelien Levra on 26/05/2024.
//

#ifndef WOODY_INCLUDE_ELF_UTILS_ELF_GETTERS_H_
#define WOODY_INCLUDE_ELF_UTILS_ELF_GETTERS_H_

#include "elf-utils.h"

Elf64_Shdr *get_section_headers_table(Elf64_Ehdr *elf_header);
Elf64_Phdr *get_program_headers_table(Elf64_Ehdr *elf_header);
char *get_section_name(Elf64_Ehdr *elf_header, Elf64_Shdr *section_header_table, int section_index);
#endif //WOODY_INCLUDE_ELF_UTILS_ELF_GETTERS_H_
