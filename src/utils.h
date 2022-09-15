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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <ctype.h>

typedef struct s_string_list {
	char **elements;
	int n_elements;
} t_string_list;
// strndup() is not available on Windows
#if defined(_WIN32) || defined(_WIN64)
char *strndup(const char *s1, size_t n);
#endif

char *str_toupper(char *src);
char *str_tolower(char *src);
char *str_trimleft(char *src);
char *replace_backslash(char *path);

int parse_hex_string(char *hexstr, unsigned char **data, size_t *length);
void sprintf_md5(char *dest, unsigned char *md5);
int file_exists(char *filename);

char *get_path(char *filename);
char *get_basename(char *filename, int strip_extension);
char *get_filename(char *path, char *basename, char *extension);
char *dos_clean_basename(char *filename, int uppercase);

t_string_list *string_list_new(char *pipe_separated_list);
char *string_list_add(t_string_list *list, char *element);
char *string_list_to_string(t_string_list *list);

void make_fat32_compatible(char *filename, int stripslashes);

#endif
