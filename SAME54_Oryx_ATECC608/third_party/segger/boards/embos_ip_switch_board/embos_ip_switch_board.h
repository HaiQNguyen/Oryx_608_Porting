/**
 * @file embos_ip_switch_board.h
 * @brief embOS/IP Switch Board
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

#ifndef _EMBOS_IP_SWITCH_BOARD_H
#define _EMBOS_IP_SWITCH_BOARD_H

//Dependencies
#include "mk66f18.h"

//LED0
#define LED0_POS    8
#define LED0_MASK   (1 << LED0_POS)
#define PORT_LED0   PORTA
#define GPIO_LED0   PTA

//LED1
#define LED1_POS    9
#define LED1_MASK   (1 << LED1_POS)
#define PORT_LED1   PORTA
#define GPIO_LED1   PTA

//LED2
#define LED2_POS    10
#define LED2_MASK   (1 << LED2_POS)
#define PORT_LED2   PORTA
#define GPIO_LED2   PTA

//LED3
#define LED3_POS    11
#define LED3_MASK   (1 << LED3_POS)
#define PORT_LED3   PORTA
#define GPIO_LED3   PTA

#endif
