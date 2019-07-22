/**
 * @file chipkit_pro_mx7.h
 * @brief Digilent chipKIT Pro MX7
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

#ifndef _CHIPKIT_PRO_MX7_H
#define _CHIPKIT_PRO_MX7_H

//Dependencies
#include <p32xxxx.h>

//LEDs
#define LED1_MASK    (1 << 12)
#define LED2_MASK    (1 << 13)
#define LED3_MASK    (1 << 14)
#define LED4_MASK    (1 << 15)

//Push buttons
#define BTN1_MASK    (1 << 6)
#define BTN2_MASK    (1 << 7)
#define BTN3_MASK    (1 << 0)

//Ethernet PHY reset
#define PHY_RST_MASK (1 << 6)

#endif
