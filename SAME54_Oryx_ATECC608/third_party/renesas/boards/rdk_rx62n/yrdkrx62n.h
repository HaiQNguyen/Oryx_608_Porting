/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : YRDKRX62N.h
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Board-specific definitions
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

#ifndef YRDKRX62N_H
#define YRDKRX62N_H

#include <iorx62n.h>

/* Local defines */
#define LED_ON              (0)
#define LED_OFF             (1)
#define SET_BIT_HIGH        (1)
#define SET_BIT_LOW         (0)
#define SET_BYTE_HIGH       (0xFF)
#define SET_BYTE_LOW        (0x00)

// I/0 port 2
#define USB0_EXICEN         PORT2.DDR.BIT.B1
#define USB_DPUPEA          PORT2.DDR.BIT.B3
#define USB_VBUSEN          PORT2.DDR.BIT.B4

// I/O port 3
#define CAN_WIFI_TX         PORT3.DDR.BIT.B2   

// I/O port 4: switches and various peripherals 
#define SW_ACTIVE FALSE
#define SW1                 PORT4.PORT.BIT.B0  
#define SW2                 PORT4.PORT.BIT.B1   
#define SW3                 PORT4.PORT.BIT.B2
#define SW1_DDR             PORT4.DR.BIT.B0
#define SW2_DDR             PORT4.DR.BIT.B1
#define SW3_DDR             PORT4.DR.BIT.B2
#define JN1_AN3             PORT4.DR.BIT.B3
#define POTENTIOMETER       PORT4.DR.BIT.B4
#define MICROPHONE          PORT4.DR.BIT.B5
#define AUDIO_IN_R          PORT4.DR.BIT.B6
#define PWMLP_IN            PORT4.DR.BIT.B7

// I/O port 5 
#define LCD_RS              PORT5.DR.BIT.B1
#define BCLK                PORT5.DR.BIT.B3
#define PWMLP_OUT           PORT5.DR.BIT.B4
#define AMP_SHDN            PORT5.DR.BIT.B5
#define LCD_RS_DDR          PORT5.DDR.BIT.B1

// I/O port A:  Extension slot (JN2) and E/N pins
#define JN2_SSLA1_B         PORTA.DR.BIT.B0
#define JN2_SSLA2_B         PORTA.DR.BIT.B1
#define JN2_SSLA3_B         PORTA.DR.BIT.B2
#define ETH_MDIO            PORTA.DR.BIT.B3
#define ETH_MDC             PORTA.DR.BIT.B4
#define ETH_LINK            PORTA.DR.BIT.B5
#define AUDIO_OUT_R         PORTA.DR.BIT.B6
#define AUDIO_OUT_L         PORTA.DR.BIT.B7

// I/O port B:  Ethernet I/O pins definitions
#define RMII_RXD1           PORTB.DR.BIT.B0
#define RMII_RXD0           PORTB.DR.BIT.B1
#define ETH_CLK             PORTB.DR.BIT.B2
#define RMII_RX_ER          PORTB.DR.BIT.B3
#define RMII_TX_EN          PORTB.DR.BIT.B4        
#define RMII_TXD0           PORTB.DR.BIT.B5        
#define RMII_TXD1           PORTB.DR.BIT.B6    
#define ETH_CRS             PORTB.DR.BIT.B7

// I/O port C:  RSPI and peripheral I/O pins definitions
#define RSPI_CHANNEL_0      0
#define RSPI_CHANNEL_1      1
#define FLASH_CS            PORTC.DR.BIT.B0        // SSL 1
#define WIFI_CS             PORTC.DR.BIT.B1        // SSL 2
#define LCD_CS              PORTC.DR.BIT.B2        // SSL 3
#define LCD_RESET           PORTC.DR.BIT.B3         
#define SDMICRO_CS          PORTC.DR.BIT.B4        // SSL 0
#define RSPI_SCLOCK         PORTC.DR.BIT.B5        
#define RSPI_MOSI           PORTC.DR.BIT.B6        
#define RSPI_MISO           PORTC.DR.BIT.B7
#define LCD_CS_DDR          PORTC.DDR.BIT.B2
#define LCD_RESET_DDR       PORTC.DDR.BIT.B3

// I/O ports D & E:  LEDs
#define LED4                PORTD.DR.BIT.B5
#define LED5                PORTE.DR.BIT.B3
#define LED6                PORTD.DR.BIT.B2
#define LED7                PORTE.DR.BIT.B0
#define LED8                PORTD.DR.BIT.B4
#define LED9                PORTE.DR.BIT.B2
#define LED10               PORTD.DR.BIT.B1
#define LED11               PORTD.DR.BIT.B7
#define LED12               PORTD.DR.BIT.B3
#define LED13               PORTE.DR.BIT.B1
#define LED14               PORTD.DR.BIT.B0
#define LED15               PORTD.DR.BIT.B6

#define LED4_DDR            PORTD.DDR.BIT.B5
#define LED5_DDR            PORTE.DDR.BIT.B3
#define LED6_DDR            PORTD.DDR.BIT.B2
#define LED7_DDR            PORTE.DDR.BIT.B0
#define LED8_DDR            PORTD.DDR.BIT.B4
#define LED9_DDR            PORTE.DDR.BIT.B2
#define LED10_DDR           PORTD.DDR.BIT.B1
#define LED11_DDR           PORTD.DDR.BIT.B7
#define LED12_DDR           PORTD.DDR.BIT.B3
#define LED13_DDR           PORTE.DDR.BIT.B1
#define LED14_DDR           PORTD.DDR.BIT.B0
#define LED15_DDR           PORTD.DDR.BIT.B6

#endif