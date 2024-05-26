#include "woody.h"

char *get_segment_name(Elf64_Word type);

void print_ELF_file_overview(Elf64_Ehdr * elf_header){
	if (elf_header == NULL) {
		printf("Unexpected NULL ptr in print_ELF_file_overview\n");
		printf("\n");
		return;
	}
	print_ELF_header(elf_header);
	Elf64_Shdr *section_headers_table = get_section_headers_table(elf_header);
	Elf64_Phdr *program_headers_table = get_program_headers_table(elf_header);
	t_elf_part * elf_parts = (t_elf_part *) malloc(sizeof(t_elf_part) * (elf_header->e_shnum + 1));
	if (elf_parts == NULL) {
		printf("Malloc failed in print_ELF_file_overview\n");
		perror("Malloc");
		printf("\n");
		return;
	}
	if (scan_elf_parts(elf_header, section_headers_table, program_headers_table, &elf_parts) == -1) {
		fprintf(stderr, "Failed to scan elf parts\n");
		return;
	}
	display_elf_parts(elf_parts, elf_header->e_shnum + elf_header->e_phnum + 1);
	free(elf_parts);
}


void print_ELF_header(Elf64_Ehdr *elf_header) {
	if (elf_header == NULL || elf_header->e_ident == NULL) {
		printf("Unexpected NULL ptr in print_ELF_header\n");
		printf("\n");
		return;
	}
	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (int i = 0; i < EI_NIDENT; ++i) {
		printf("%02x ", elf_header->e_ident[i]);
	}
	printf("\n");
//	printf("  Class:                             %s\n", elf_header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "ELF32");
//	printf("  Data:                              %s\n", elf_header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" : "2's complement, big endian");
//	printf("  Version:                           %d\n", elf_header->e_ident[EI_VERSION]);
//	printf("  OS/ABI:                            %d\n", elf_header->e_ident[EI_OSABI]);
//	printf("  ABI Version:                       %d\n", elf_header->e_ident[EI_ABIVERSION]);
//	printf("  Type:                              %d\n", elf_header->e_type);
//	printf("  Machine:                           %d\n", elf_header->e_machine);
//	printf("  Version:                           %d\n", elf_header->e_version);
	printf("  Entry point address:               %p\n", (void *) elf_header->e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n", elf_header->e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n", elf_header->e_shoff);
//	printf("  Flags:                             %d\n", elf_header->e_flags);
	printf("  Size of this header:               %d (bytes)\n", elf_header->e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", elf_header->e_phentsize);
	printf("  Number of program headers:         %d\n", elf_header->e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", elf_header->e_shentsize);
	printf("  Number of section headers:         %d\n", elf_header->e_shnum);
//	printf("  Section header string table index: %d\n", elf_header->e_shstrndx);
}

/**
 * @brief populate a t_elf_part array that contains the start and end of each part of the ELF file
 *
 * @param elf_header
 * @param section_header_table
 * @param section_index
 *
 * @return c
 */
int scan_elf_parts(Elf64_Ehdr *elf_header,
				   Elf64_Shdr *section_headers_table,
				   Elf64_Phdr *Program_headers_table,
				   t_elf_part **elf_parts) {
	if (elf_parts == NULL || elf_header == NULL || section_headers_table == NULL) {
		fprintf(stderr, "Unexpected NULL ptr in scan_elf_parts\n");
		printf("\n");
		return -1;
	}
	(*elf_parts)[0].start = 0;
	(*elf_parts)[0].end = (void *) elf_header->e_ehsize;
	(*elf_parts)[0].name = strdup("ELF Header");
	(*elf_parts)[0].size = elf_header->e_ehsize;
	(*elf_parts)[0].alignment = 1; // Not relevant for the ELF Header
	int j = 0;
	int i = 1;
	for (i = 1; i < elf_header->e_shnum; ++i) {
		(*elf_parts)[i].start = (void *) section_headers_table[i].sh_offset;
		(*elf_parts)[i].end = (*elf_parts)[i].start + section_headers_table[i].sh_size;
		(*elf_parts)[i].name = get_section_name(elf_header, section_headers_table, i);
		(*elf_parts)[i].size = section_headers_table[i].sh_size;
		(*elf_parts)[i].alignment = section_headers_table[i].sh_addralign;
	}
	for (j = 0; j < elf_header->e_phnum; ++j) {
		(*elf_parts)[i].start = (void *) Program_headers_table[j].p_offset;
		(*elf_parts)[i].end = (*elf_parts)[i].start + Program_headers_table[j].p_filesz;
		(*elf_parts)[i].name = get_segment_name(Program_headers_table[j].p_type);
		(*elf_parts)[i].size = Program_headers_table[j].p_filesz;
		(*elf_parts)[i].alignment = Program_headers_table[j].p_align;
		i++;
	}
	return 0;
}

/**
 * @brief Display the parts of the ELF file in colorful style.
 * First colomn is the name of the part
 * Second colomn is the size of the part
 * Third and Fourth colomn is start and end address of the part
 * Last colomn is the padding of the part
 * @param elf_parts
 * @param size
 */
void display_elf_parts(t_elf_part *elf_parts, int size) {
	printf("\n");
	printf("ELF Parts:\n");
	printf("  %-30s %-10s %-18s %-18s %-18s %s\n",
		   "Name", "Size", "Size hex", "Start", "End", "Alignment");
	for (int i = 0; i < size; ++i) {
		printf("  %-30s %-10d %-18x %-18p %-18p %d\n",
			   elf_parts[i].name,
			   elf_parts[i].size, elf_parts[i].size,
			   (unsigned long) elf_parts[i].start,
			   (unsigned long) elf_parts[i].end,
			   elf_parts[i].alignment);
	}

}
