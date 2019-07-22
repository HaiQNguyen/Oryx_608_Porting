/**
 * @file same54_crypto_pukcc.h
 * @brief SAME54 PUKCC public key accelerator
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2019 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCrypto Open.
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
 * @version 1.9.2h
 **/

#ifndef _SAME54_CRYPTO_PUKCC_H
#define _SAME54_CRYPTO_PUKCC_H

//Dependencies
#include "core/crypto.h"

//Crypto memory base address
#define PUKCC_CRYPTO_RAM_BASE 0x02011000UL

//Far to near pointer conversion
#define PUKCC_FAR_TO_NEAR(p) ((uint16_t) ((uint32_t) (p)))

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Elliptic curve point
 **/

typedef struct
{
   uint8_t *x;
   uint8_t *y;
   uint8_t *z;
} PukccEcPoint;


/**
 * @brief Fmult service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *x;
   uint8_t *y;
   uint8_t *z;
   uint8_t *r;
} PukccFmultParams;


/**
 * @brief GCD service parameters
 **/

typedef struct
{
   uint8_t *x;
   uint8_t *y;
   uint8_t *a;
   uint8_t *z;
   uint8_t *w;
} PukccGcdParams;


/**
 * @brief RedMod service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *exp;
   uint8_t *r;
   uint8_t *x;
} PukccRedModParams;


/**
 * @brief ExpMod service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *exp;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccExpModParams;


/**
 * @brief CRT service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *x;
   uint8_t *exp;
   uint8_t *p;
   uint8_t *q;
   uint8_t *dp;
   uint8_t *dq;
   uint8_t *r;
} PukccCrtParams;


/**
 * @brief ZpEcPointIsOnCurve service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint point;
   uint8_t *a;
   uint8_t *b;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcPointIsOnCurveParams;


/**
 * @brief ZpEcConvProjToAffine service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *k;
   PukccEcPoint point;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcConvProjToAffineParams;


/**
 * @brief ZpEccMul service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *k;
   PukccEcPoint point;
   uint8_t *a;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEccMulParams;



/**
 * @brief ZpEcDsaGenerate service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint basePoint;
   uint8_t *order;
   uint8_t *a;
   uint8_t *privateKey;
   uint8_t *k;
   uint8_t *h;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcDsaGenerateParams;


/**
 * @brief ZpEcDsaVerify service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint basePoint;
   uint8_t *order;
   uint8_t *a;
   PukccEcPoint publicKey;
   uint8_t *h;
   uint8_t *r;
   uint8_t *s;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcDsaVerifyParams;


//PUKCC related function
error_t pukccInit(void);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif