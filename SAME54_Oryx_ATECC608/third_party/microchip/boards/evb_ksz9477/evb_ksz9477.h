/**
 * @file evb_ksz9477.h
 * @brief EVB-KSZ9477 evaluation kit
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

#ifndef _EVB_KSZ9477_H
#define _EVB_KSZ9477_H

//Clocks
#define BOARD_MAINOSC 12000000
#define BOARD_MCK ((unsigned long) ((BOARD_MAINOSC / 3 / 2) * 66 ))

//DDRAM type
#define DDRAM_MT47H64M16HR 0
#define DDRAM_MT47H128M16RT 1
#define BOARD_DDRAM_TYPE DDRAM_MT47H128M16RT

#endif
