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

#ifndef _MRA_H_
#define _MRA_H_

#include <stdint.h>
#include "sxmlc/sxmlc.h"
#include "utils.h"
#include "globals.h"

typedef struct s_part {
	int is_group;
	union {
		struct s_p {
			char *name;
			char *zip;
			uint32_t crc32;
			uint32_t repeat;
			uint32_t offset;
			uint32_t length;
			uint8_t *pattern;
			int _map_index; // only used for "interleave maps" to "group patterns" conversion
			uint8_t *data;
			size_t data_length;
		} p;
		struct s_g {
			int is_interleaved;
			int width;
			int repeat;
			struct s_part *parts;
			int n_parts;
		} g;
	};
} t_part;

typedef struct s_patch {
	uint32_t offset;
	uint8_t *data;
	size_t data_length;
} t_patch;

typedef struct s_rom {
	int index;
	char *md5;
	t_string_list type;
	t_string_list zip;
	t_part *parts;
	int n_parts;
	t_patch *patches;
	int n_patches;
} t_rom;

typedef struct s_dip {
	char *bits;
	char *name;
	char *ids;
} t_dip;

typedef struct s_switches {
	t_dip *dips;
	int n_dips;
	long long defaults;
	int base;
	int page_id;
	char *page_name;
} t_switches;

typedef struct s_rbf {
	char *name;
	char *alt_name;
} t_rbf;

typedef struct s_mra {
	XMLDoc _xml_doc;

	char *name;
	char *mratimestamp;
	char *mameversion;
	char *setname;
	char *year;
	char *manufacturer;

	t_rbf rbf;
	t_string_list categories;
	t_switches switches;

	t_rom *roms;
	int n_roms;
} t_mra;

int mra_load(char *filename, t_mra *mra);
int mra_dump(t_mra *mra);
int mra_get_next_rom0(t_mra *mra, int start_index);
int mra_get_rom_by_index(t_mra *mra, int index, int start_pos);

#endif
