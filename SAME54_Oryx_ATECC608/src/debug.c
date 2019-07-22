/**
 * @file debug.c
 * @brief Debugging facilities
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2019 Oryx Embedded SARL. All rights reserved.
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

//Dependencies
#include "same54.h"
#include "debug.h"


/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

void debugInit(uint32_t baudrate)
{
   //Enable SERCOM2 core clock
   GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 |
      GCLK_PCHCTRL_CHEN;

   //Enable PORT bus clock (CLK_PORT_APB)
   MCLK->APBBMASK.bit.PORT_ = 1;
   //Enable SERCOM2 bus clock (CLK_SERCOM2_APB)
   MCLK->APBBMASK.bit.SERCOM2_ = 1;

   //Configure RXD pin (PB24)
   PORT->Group[1].PINCFG[24].bit.PULLEN = 1;
   PORT->Group[1].PINCFG[24].bit.PMUXEN = 1;
   PORT->Group[1].PMUX[12].bit.PMUXE = MUX_PB24D_SERCOM2_PAD1;

   //Configure TXD pin (PB25)
   PORT->Group[1].PINCFG[25].bit.PMUXEN = 1;
   PORT->Group[1].PMUX[12].bit.PMUXO = MUX_PB25D_SERCOM2_PAD0;

   //Perform software reset
   SERCOM2->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;

   //Resetting the SERCOM (CTRLA.SWRST) requires synchronization
   while(SERCOM2->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_SWRST);

   //Configure SERCOM2
   SERCOM2->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD |
      SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(0) |
      SERCOM_USART_CTRLA_SAMPR(0) | SERCOM_USART_CTRLA_MODE(1);

   SERCOM2->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN |
      SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_CHSIZE(0);

   //Writing to the CTRLB register when the SERCOM is enabled requires
   //synchronization
   while(SERCOM2->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_CTRLB);

   //Configure baud rate
   SERCOM2->USART.BAUD.reg = 65535 - ((baudrate * 16384) / (SystemCoreClock / 64));

   //Enable SERCOM2
   SERCOM2->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;

   //Enabling and disabling the SERCOM requires synchronization
   while(SERCOM2->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}


/**
 * @brief Display the contents of an array
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @param[in] prepend String to prepend to the left of each line
 * @param[in] data Pointer to the data array
 * @param[in] length Number of bytes to display
 **/

void debugDisplayArray(FILE *stream,
   const char_t *prepend, const void *data, size_t length)
{
   uint_t i;

   for(i = 0; i < length; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
         fprintf(stream, "%s", prepend);
      //Display current data byte
      fprintf(stream, "%02" PRIX8 " ", *((uint8_t *) data + i));
      //End of current line?
      if((i % 16) == 15 || i == (length - 1))
         fprintf(stream, "\r\n");
   }
}


/**
 * @brief Write character to stream
 * @param[in] c The character to be written
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @return On success, the character written is returned. If a writing
 *   error occurs, EOF is returned
 **/

int_t fputc(int_t c, FILE *stream)
{
   //Standard output or error output?
   if(stream == stdout || stream == stderr)
   {
      //Send character
      SERCOM2->USART.DATA.reg = c;
      //Wait for the transfer to complete
      while(!(SERCOM2->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_TXC));

      //On success, the character written is returned
      return c;
   }
   //Unknown output?
   else
   {
      //If a writing error occurs, EOF is returned
      return EOF;
   }
}
