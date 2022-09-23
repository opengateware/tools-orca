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

#ifndef ORCA_JSON_H
#define ORCA_JSON_H

#define MAX_JSON_LENGTH 8192

int write_json(t_mra *mra, char *filename);

#endif // ORCA_JSON_H
