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

#ifndef _PART_H_
#define _PART_H_

#include "utils.h"
#include "mra.h"

int write_rom(t_mra *mra, t_string_list *dirs, char *rom_filename);

#endif
