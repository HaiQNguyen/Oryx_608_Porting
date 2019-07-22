/**
 * @file efm32_giant_gecko_11_sk.h
 * @brief EFM32 Giant Gecko 11 Starter Kit
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

#ifndef _EFM32_GIANT_GECKO_11_SK_H
#define _EFM32_GIANT_GECKO_11_SK_H

//Dependencies
#include "em_device.h"

//LED0
#define LED0_R_GPIO_PORT gpioPortH
#define LED0_R_GPIO_PIN  10
#define LED0_G_GPIO_PORT gpioPortH
#define LED0_G_GPIO_PIN  11
#define LED0_B_GPIO_PORT gpioPortH
#define LED0_B_GPIO_PIN  12

//LED1
#define LED1_R_GPIO_PORT gpioPortH
#define LED1_R_GPIO_PIN  13
#define LED1_G_GPIO_PORT gpioPortH
#define LED1_G_GPIO_PIN  14
#define LED1_B_GPIO_PORT gpioPortH
#define LED1_B_GPIO_PIN  15

//BTN0 button
#define BTN0_GPIO_PORT   gpioPortC
#define BTN0_GPIO_PIN    8

//BTN1 button
#define BTN1_GPIO_PORT   gpioPortC
#define BTN1_GPIO_PIN    9

#endif
