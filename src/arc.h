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

#ifndef _ARC_H_
#define _ARC_H_

#include "mra.h"

#define MAX_LINE_LENGTH 256
#define MAX_CONTENT_LENGTH 25
#define MAX_CONF_OPT_LENGTH 128

int write_arc(t_mra *mra, char *filename);
char *format_bits(t_mra *mra, t_dip *dip);
int check_ids_len(t_dip *dip);

#endif
