/*******************************************************************************
 * SPDX-License-Identifier: MPL-2.0
 * SPDX-FileType: SOURCE
 * SPDX-FileCopyrightText: (c) 2022, OpenGateware authors and contributors
 *******************************************************************************
 *
 * ORCA (Open ROM Conversion Assistant)
 * Copyright (c) 2022, OpenGateware authors and contributors (see AUTHORS file)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 ******************************************************************************/

/*!*****************************************************************************
 * @file build.cpp
 * @brief
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arc.h"
#include "mra.h"
#include "rom.h"
#include "utils.h"
#include "version.h"
#include "json.h"

int trace = 0;
int verbose = 0;
char *rom_basename = NULL;

void print_version() {
	printf(PROJECT_TITLE " v%s [%s] (%s)\n", PROJECT_VER, BUILD_REF, BUILD_DATE);
}

void print_usage() {
	printf(PROJECT_NAME " v%s [%s] (%s)\n", PROJECT_VER, BUILD_REF, BUILD_DATE);
	printf("Usage:\n");
	printf("  orca [-vlzoOaAJ] <recipe.xml>\n");
	printf("\nConvert ROM recipes to a single file for usage with arcade cores.\nOptionally creates the associated metadata files.\n\n");
	printf("Options:\n");
	printf("  -h\t\tthis help.\n");
	printf("  -v\t\twhen it is the only parameter, display version information and exit. Otherwise, set Verbose on (default: off).\n");
	printf("  -l\t\tlist recipe content instead of creating the ROM file.\n");
	printf("  -z directory\tadd directory to include zip files. Directories added with -z have priority over the current dir.\n");
	printf("  -o filename\tset the output ROM file name. Overrides the internal generation of the filename.\n");
	printf("  -O directory\tset the output directory. By default, ROM and metadata files are created in the current directory.\n");
	printf("  -a filename\tset the output metadata file name. Overrides the internal generation of the filename.\n");
	printf("  -A\t\tcreate ARC file. This is done in addition to creating the ROM file.\n");
	printf("  -J\t\tcreate JSON file. This is done in addition to creating the ROM file.\n");
	printf("  -s\t\tskip ROM creation. This is useful if only the metadata file is required.\n");
}

int main(int argc, char **argv) {
	char *rom_filename = NULL;
	char *arc_filename = NULL;
	char *output_dir = NULL;
	char *mra_filename;
	char *mra_basename;
	t_string_list *dirs = string_list_new(NULL);
	int i, res;
	int dump_mra = 0;
	int dump_rom = -1;
	int create_arc = 0;
	int create_json = 0;

	if(trace > 0) {
		for(i = 0; i < argc; i++) {
			printf("argv[%d]: %s\n", i, argv[i]);
		}
	}

	// Parse command line
	// Looks bad but mkfs does it so why not me ?
	if(argc == 2 && !strcmp(argv[1], "-v")) {
		print_version();
		exit(0);
	}

	int opt;
	// put ':' in the starting of the
	// string so that program can
	// distinguish between '?' and ':'
	while((opt = getopt(argc, argv, ":vlhAJo:a:O:z:s")) != -1) {
		switch(opt) {
			case 'v': verbose = -1; break;
			case 'l': dump_mra = -1; break;
			case 'A': create_arc = -1; break;
			case 'J': create_json = -1; break;
			case 'z': string_list_add(dirs, replace_backslash(optarg)); break;
			case 'O': output_dir = replace_backslash(strndup(optarg, 1024)); break;
			case 'o': rom_filename = replace_backslash(strndup(optarg, 1024)); break;
			case 'a': arc_filename = replace_backslash(strndup(optarg, 1024)); break;
			case 's': dump_rom = 0; break;
			case 'h': print_usage(); exit(0);
			case ':': printf("option needs a value\n");
			case '?': printf("unknown option: %c\n", optopt);
			default: print_usage(); exit(-1);
		}
	}

	if(optind == argc) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	if(argc - optind > 1) {
		free(rom_filename);
		free(arc_filename);
		rom_filename = NULL;
		arc_filename = NULL;
	}

	for(int name_idx = optind; name_idx < argc; name_idx++) {
		t_mra mra;

		mra_filename = replace_backslash(strndup(argv[name_idx], 1024));
		if(!file_exists(mra_filename)) {
			printf("error: file not found (%s)\n", mra_filename);
			exit(EXIT_FAILURE);
		}

		if(trace > 0) {
			printf("mra: %s\n", mra_filename);
		}

		char *mra_path = get_path(mra_filename);
		if(mra_path) {
			string_list_add(dirs, mra_path);
		}
		string_list_add(dirs, ".");

		if(verbose) {
			if(dirs->n_elements) {
				printf("zip include dirs: ");
				for(i = 0; i < dirs->n_elements; i++) {
					printf("%s%s/", i ? ", " : "", dirs->elements[i]);
				}
				printf("\n");
			}
		}

		if(mra_load(mra_filename, &mra)) {
			exit(EXIT_FAILURE);
		}

		mra_basename = get_basename(mra_filename, 1);
		if(rom_filename) {
			rom_basename = get_basename(rom_filename, 1);
			if(output_dir) {
				rom_filename = get_filename(output_dir, rom_basename, "rom");
			}
		} else {
			rom_basename = dos_clean_basename(mra.setname ? mra.setname : mra_basename, 0);
			rom_filename = get_filename(output_dir ? output_dir : ".", rom_basename, "rom");
		}

		if(trace > 0) {
			printf("MRA loaded...\n");
		}

		if(dump_mra) {
			if(trace > 0) {
				printf("dumping MRA content...\n");
			}
			mra_dump(&mra);
		} else {
			if(create_arc) {
				if(trace > 0) {
					printf("create_arc set...\n");
				}
				if(arc_filename) {
					if(output_dir) {
						arc_filename = get_filename(output_dir, get_basename(arc_filename, 1), "arc");
					}
					make_fat32_compatible(arc_filename, 0);
				} else {
					char *arc_mra_filename = strdup(mra.name ? mra.name : mra_basename);
					make_fat32_compatible(arc_mra_filename, 1);
					arc_filename = get_filename(output_dir ? output_dir : ".", arc_mra_filename, "arc");
					free(arc_mra_filename);
				}
				if(verbose) {
					printf("Creating ARC file %s\n", arc_filename);
				}
				res = write_arc(&mra, arc_filename);
				if(res != 0) {
					printf("Writing ARC file failed with error code: %d\n. Retry without -A if you still want to create the ROM file.\n", res);
					exit(EXIT_FAILURE);
				}
				arc_filename = NULL;

			}

			if(create_json) {
				if(trace > 0) {
					printf("create_json set...\n");
				}

				char *arc_mra_filename = strdup(mra.name ? mra.name : mra_basename);
				make_fat32_compatible(arc_mra_filename, 1);
				arc_filename = get_filename(output_dir ? output_dir : ".", arc_mra_filename, "json");
				free(arc_mra_filename);

				if(verbose) {
					printf("Creating JSON file %s\n", arc_filename);
				}
				res = write_json(&mra, arc_filename);
				if(res != 0) {
					printf("Writing JSON file failed with error code: %d\n. Retry without -J if you still want to create the ROM file.\n", res);
					exit(EXIT_FAILURE);
				}
				arc_filename = NULL;


			}
			if(dump_rom) {
				if(trace > 0) {
					printf("creating ROM...\n");
				}
				res = write_rom(&mra, dirs, rom_filename);
				if(res != 0) {
					printf("Writing ROM failed with error code: %d\n", res);
					exit(EXIT_FAILURE);
				}
			}
			free(rom_filename);
			rom_filename = NULL;
		}
	}
	if(verbose) {
		printf("done!\n");
	}
	return EXIT_SUCCESS;
}
