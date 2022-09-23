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
#include <string.h>

#include "globals.h"
#include "arc.h"
#include "json.h"

int write_json(t_mra *mra, char *filename) {
	FILE *out;
	char buffer[MAX_JSON_LENGTH + 1];
	int i, n;
	unsigned int mod = 0;
	char mode[8] = "";


	out = fopen(filename, "wb");
	if(out == NULL) {
		fprintf(stderr, "Couldn't open %s for writing!\n", filename);
		return -1;
	}

	i = mra_get_rom_by_index(mra, 1, 0);
	if(i != -1 && mra->roms[i].n_parts == 1 && !mra->roms[i].parts[0].is_group) {
		for(size_t k = mra->roms[i].parts[0].p.data_length; k-- > 0;) {
			char temp[2 + 1] = "";
			sprintf(temp, "%02X", mra->roms[i].parts[0].p.data[k]);
			strcat(mode, temp);
		}
		sscanf(mode, "%x", &mod);
	}

	const char *json = "{\n"
					   "\t\"instance\": {\n"
					   "\t\t\"magic\": \"APF_VER_1\",\n"
					   "\t\t\"variant_select\": {\n"
					   "\t\t\t\"id\": 777,\n"
					   "\t\t\t\"select\": false\n"
					   "\t\t},\n"
					   "\t\t\"data_slots\": [\n"
					   "\t\t\t{\n"
					   "\t\t\t\t\"id\": 1,\n"
					   "\t\t\t\t\"filename\": \"%s.rom\"\n"
					   "\t\t\t}\n"
					   "\t\t],\n"
					   "\t\t\"memory_writes\": [\n"
					   "\t\t\t{\n"
					   "\t\t\t\t\"address\": \"0xfd000000\",\n"
					   "\t\t\t\t\"data\": \"0x%08x\"\n"
					   "\t\t\t},\n"
					   "\t\t\t{\n"
					   "\t\t\t\t\"address\": \"0xfe000000\",\n"
					   "\t\t\t\t\"data\": \"0x%02x\"\n"
					   "\t\t\t}\n"
					   "\t\t]\n"
					   "\t}\n"
					   "}";

	n = snprintf(buffer, MAX_JSON_LENGTH, json, rom_basename, mra->switches.defaults << mra->switches.base, (mod != -1 ? mod : 0));
	if(n >= MAX_JSON_LENGTH) {
		printf("%s:%d: warning: line was truncated while writing in JSON file!\n", __FILE__, __LINE__);
	}
	fwrite(buffer, 1, n, out);

	fclose(out);
	return 0;
}

void write_interact(t_mra *mra) {
	// if(mra->switches.page_id && mra->switches.page_name) {
	//	n = snprintf(buffer, MAX_JSON_LINE_LENGTH, "CONF=\"P%d,%s\"\n", mra->switches.page_id, mra->switches.page_name);
	//	fwrite(buffer, 1, n, out);
	// }
	//	for(i = 0; i < mra->switches.n_dips; i++) {
	//		t_dip *dip = mra->switches.dips + i;
	//		if(!strstr(str_tolower(dip->name), "unused")) {
	//			if(dip->ids) {
	//				n = snprintf(buffer, MAX_JSON_LINE_LENGTH, "CONF=\"%s,%s,%s\"\n", format_bits(mra, dip), dip->name, dip->ids);
	//			} else {
	//				n = snprintf(buffer, MAX_JSON_LINE_LENGTH, "CONF=\"%s,%s\"\n", format_bits(mra, dip), dip->name);
	//			}
	//			if(n >= MAX_JSON_LINE_LENGTH) {
	//				printf("%s:%d: warning (%s): line was truncated while writing in JSON file!\n", __FILE__, __LINE__, mra->setname);
	//				continue;
	//			}
	//			fwrite(buffer, 1, n, out);
	//		} else {
	//			printf("warning (%s): \"%s\" dip setting skipped (unused)\n", mra->setname, dip->name);
	//		}
	//	}
}
