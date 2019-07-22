/**
 * @file lpcxpresso_1769.h
 * @brief LPCXpresso1769 evaluation board
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

#ifndef _LPCXPRESSO_1769_H
#define _LPCXPRESSO_1769_H

//Dependencies
#include "lpc17xx.h"

//Red LED
#define LED_R_MASK      (1 << 22)
#define LED_R_FIODIR    LPC_GPIO0->FIODIR
#define LED_R_FIOSET    LPC_GPIO0->FIOSET
#define LED_R_FIOCLR    LPC_GPIO0->FIOCLR

//Green LED
#define LED_G_MASK      (1 << 25)
#define LED_G_FIODIR    LPC_GPIO3->FIODIR
#define LED_G_FIOSET    LPC_GPIO3->FIOSET
#define LED_G_FIOCLR    LPC_GPIO3->FIOCLR

//Blue LED
#define LED_B_MASK      (1 << 26)
#define LED_B_FIODIR    LPC_GPIO3->FIODIR
#define LED_B_FIOSET    LPC_GPIO3->FIOSET
#define LED_B_FIOCLR    LPC_GPIO3->FIOCLR

//SW2
#define SW2_MASK        (1 << 10)
#define SW2_FIODIR      LPC_GPIO2->FIODIR
#define SW2_FIOPIN      LPC_GPIO2->FIOPIN

#endif
