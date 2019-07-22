/**
 * @file pic32mz_ef_curiosity.h
 * @brief PIC32MZ EF Curiosity
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2019 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.9.2
 **/

#ifndef _PIC32MZ_EF_CURIOSITY_H
#define _PIC32MZ_EF_CURIOSITY_H

//Dependencies
#include <p32xxxx.h>

//LEDs
#define LED1_MASK   (1 << 3)
#define LED2_MASK   (1 << 4)
#define LED3_MASK   (1 << 6)
#define LED4_R_MASK (1 << 5)
#define LED4_G_MASK (1 << 1)
#define LED4_B_MASK (1 << 0)

//Push button
#define SW1_MASK (1 << 12)

#endif
