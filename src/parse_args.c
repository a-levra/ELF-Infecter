#include "woody.h"

char G_FLAGS[5] = {0, 0, 0, 0, 0};
/*
 * no flags for now ..
 * */
void check_flag_conformity(int argc, char **argv);

void check_flags(int argc, char **argv) {
	check_flag_conformity(argc, argv);
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			int j = 1;
			while (argv[i][j]) {
				switch (argv[i][j]) {
						break;
					default: printf("Error: flag %c not recognized\n", argv[i][1]);
						exit(EXIT_FAILURE);
				}
				++j;
			}
		}
	}

}

void usage() {
	printf("Usage: woody <binary>\n");
	exit(EXIT_FAILURE);
}

void check_flag_conformity(int argc, char **argv) {
	if (argc < 2) {
		usage();
		exit(EXIT_FAILURE);
	}
	//last argument must not be a flag
	if (argv[argc - 1][0] == '-') {
		usage();
		exit(EXIT_FAILURE);
	}

	//the binary should be the last argument
	if (argc > 2) {
		for (int i = 1; i < argc - 1; ++i) {
			if (argv[i][0] != '-') {
				usage();
				exit(EXIT_FAILURE);
			}
		}
	}

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '\0') {
				printf("Error: flag %s not recognized\n", argv[i]);
				exit(EXIT_FAILURE);
			}
		}
	}

}
