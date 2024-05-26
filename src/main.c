#include "woody.h"

void add_section(Elf64_Ehdr *header, Elf64_Shdr *table, Elf64_Shdr *section);

int main(int argc, char **argv) {
	int file_descriptor;
	struct stat file_stat;
	void *binary_pointer;

	check_arguments(argc, argv);

	file_descriptor = open_file(argv[argc - 1]);
	get_file_stat(file_descriptor, &file_stat);
	map_file_to_memory(file_descriptor, &file_stat, &binary_pointer);
	close(file_descriptor);
//	check_ELF_file_integrity(binary_pointer, file_stat.st_size);
	print_ELF_file_overview((Elf64_Ehdr *)binary_pointer);
	infection(binary_pointer);
	unmap_file(binary_pointer, file_stat.st_size);
	exit(EXIT_SUCCESS);
}
/*
 * Infection workflow :
 *  - write a section that contains payload
 *  - Add sections to the binary
 *  - change Entry Point
 */
void infection(void *binary_pointer) {
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *) binary_pointer;
	Elf64_Shdr *section_headers_table = get_section_headers_table(elf_header);
	Elf64_Phdr * last_pt_load = find_last_PT_LOAD(elf_header);
	Elf64_Shdr * last_section = find_last_section(elf_header, section_headers_table, last_pt_load);
	printf("last PT_LOAD segment : %p\n", (void*)last_pt_load->p_offset);
	printf("last section of PT_LOAD segment : %p\n", last_section->sh_offset);
	printf("last section of PT_LOAD type : %d\n", last_section->sh_type);

	add_section(elf_header, section_headers_table, last_section);
//	adjust_segment_size(elf_header);
//	change_entry_point(elf_header);
//	adjust_e_shstrndx(elf_header);
//	adjust_sections_offset(elf_header, section_headers_table);
//	adjust_e_shoff(elf_header);
//	check_section_headers_table_integrity(elf_header, section_headers_table);
//	Elf64_Shdr *text_section = get_text_section(elf_header, section_headers_table);
//	check_text_section_integrity(elf_header, text_section);
//	encrypt_text_section(elf_header, text_section);
}

void add_section(Elf64_Ehdr *header, Elf64_Shdr *table, Elf64_Shdr *section) {


}

/*
 * find the last section of segment PT_LOAD
 * */
Elf64_Shdr *find_last_section(Elf64_Ehdr *elf_header, Elf64_Shdr *section_headers_table, Elf64_Phdr *last_pt_load) {
	if (elf_header == NULL || section_headers_table == NULL || last_pt_load == NULL) {
		fprintf(stderr, "Error: Unexpected NULL pointer in find_last_section\n");
		exit(EXIT_FAILURE);
	}
	Elf64_Shdr *last_section = NULL;
	unsigned long offset_start = last_pt_load->p_offset;
	unsigned long offset_end = last_pt_load->p_offset + last_pt_load->p_filesz;
	for (int i = 0; i < elf_header->e_shnum; ++i) {
		Elf64_Shdr *current_section = &section_headers_table[i];
		if (current_section->sh_offset >= offset_start && current_section->sh_offset <= offset_end) {
			if (last_section == NULL || last_section->sh_offset < current_section->sh_offset) {
				last_section = current_section;
			}
		}
	}
	if (last_section == NULL) {
		fprintf(stderr, "Error: could not find last section\n");
		exit(EXIT_FAILURE);
	}
	return last_section;
}

Elf64_Phdr * find_last_PT_LOAD(Elf64_Ehdr *elf_header) {
	if (elf_header == NULL) {
		fprintf(stderr, "Error: Unexpected NULL pointer in find_last_PT_LAOD\n");
		exit(EXIT_FAILURE);
	}
	Elf64_Phdr *program_header_table = get_program_headers_table(elf_header);
	Elf64_Phdr *last_PT_LOAD = NULL;
	for (int i = 0; i < elf_header->e_phnum; ++i) {
		if (program_header_table[i].p_type == PT_LOAD) {
			last_PT_LOAD = &program_header_table[i];
		}
	}
	if (last_PT_LOAD == NULL) {
		fprintf(stderr, "Error: could not find PT_LOAD segment\n");
		exit(EXIT_FAILURE);
	}
	return last_PT_LOAD;
}
