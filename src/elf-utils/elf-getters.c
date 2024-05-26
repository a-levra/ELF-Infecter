#include "elf-getters.h"

Elf64_Shdr *get_section_headers_table(Elf64_Ehdr *elf_header){
return (Elf64_Shdr *) ((char *) elf_header + elf_header->e_shoff);
}

Elf64_Phdr *get_program_headers_table(Elf64_Ehdr *elf_header) {
	return (Elf64_Phdr *) ((char *) elf_header + elf_header->e_phoff);
}

char *get_section_name(Elf64_Ehdr *elf_header, Elf64_Shdr *section_header_table, int section_index) {
	if (elf_header == NULL || section_header_table == NULL) {
		printf("Unexpected NULL ptr in get_section_name\n");
		printf("\n");
		return NULL;
	}
	Elf64_Shdr strtab = section_header_table[elf_header->e_shstrndx];
	return (char *) elf_header + strtab.sh_offset + section_header_table[section_index].sh_name;
}

char *get_segment_name(Elf64_Word type) {
	switch (type) {
		case PT_NULL:
			return "NULL";
		case PT_LOAD:
			return "LOAD";
		case PT_DYNAMIC:
			return "DYNAMIC";
		case PT_INTERP:
			return "INTERP";
		case PT_NOTE:
			return "NOTE";
		case PT_SHLIB:
			return "SHLIB";
		case PT_PHDR:
			return "PHDR";
		case PT_TLS:
			return "TLS";
		case PT_GNU_EH_FRAME:
			return "GNU_EH_FRAME";
		case PT_GNU_STACK:
			return "GNU_STACK";
		case PT_GNU_RELRO:
			return "GNU_RELRO";
		default:
			return "UNKNOWN";
	}
}