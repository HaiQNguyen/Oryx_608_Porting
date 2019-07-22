/**
 * @file llmnr_common.c
 * @brief Definitions common to LLMNR client and responder
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
 * @version 1.9.2h
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL LLMNR_TRACE_LEVEL

//Dependencies
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core/net.h"
#include "llmnr/llmnr_client.h"
#include "llmnr/llmnr_responder.h"
#include "llmnr/llmnr_common.h"
#include "dns/dns_debug.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (LLMNR_CLIENT_SUPPORT == ENABLED || LLMNR_RESPONDER_SUPPORT == ENABLED)

//LLMNR IPv6 multicast group (ff02::1:3)
const Ipv6Addr LLMNR_IPV6_MULTICAST_ADDR =
   IPV6_ADDR(0xFF02, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0003);


/**
 * @brief LLMNR related initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t llmnrInit(NetInterface *interface)
{
   error_t error;

#if (IPV4_SUPPORT == ENABLED)
   //Join the LLMNR IPv4 multicast group
   error = ipv4JoinMulticastGroup(interface, LLMNR_IPV4_MULTICAST_ADDR);
   //Any error to report?
   if(error)
      return error;
#endif

#if (IPV6_SUPPORT == ENABLED)
   //Join the LLMNR IPv6 multicast group
   error = ipv6JoinMulticastGroup(interface, &LLMNR_IPV6_MULTICAST_ADDR);
   //Any error to report?
   if(error)
      return error;
#endif

   //Callback function to be called when a LLMNR message is received
   error = udpAttachRxCallback(interface, LLMNR_PORT, llmnrProcessMessage,
      NULL);
   //Any error to report?
   if(error)
      return error;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Process incoming LLMNR message
 * @param[in] interface Underlying network interface
 * @param[in] pseudoHeader UDP pseudo header
 * @param[in] udpHeader UDP header
 * @param[in] buffer Multi-part buffer containing the incoming LLMNR message
 * @param[in] offset Offset to the first byte of the LLMNR message
 * @param[in] param Callback function parameter (not used)
 **/

void llmnrProcessMessage(NetInterface *interface, const IpPseudoHeader *pseudoHeader,
   const UdpHeader *udpHeader, const NetBuffer *buffer, size_t offset, void *param)
{
   size_t length;
   LlmnrHeader *message;

   //Retrieve the length of the LLMNR message
   length = netBufferGetLength(buffer) - offset;

   //Ensure the LLMNR message is valid
   if(length < sizeof(LlmnrHeader))
      return;
   if(length > DNS_MESSAGE_MAX_SIZE)
      return;

   //Point to the LLMNR message header
   message = netBufferAt(buffer, offset);
   //Sanity check
   if(message == NULL)
      return;

   //Debug message
   TRACE_INFO("LLMNR message received (%" PRIuSIZE " bytes)...\r\n", length);
   //Dump message
   dnsDumpMessage((DnsHeader *) message, length);

   //LLMNR messages received with an opcode other than zero must be silently
   //ignored
   if(message->opcode != DNS_OPCODE_QUERY)
      return;

   //LLMNR messages received with non-zero response codes must be silently
   //ignored
   if(message->rcode != DNS_RCODE_NO_ERROR)
      return;

   //LLMNR query received?
   if(!message->qr)
   {
#if (LLMNR_RESPONDER_SUPPORT == ENABLED)
      //Process incoming LLMNR query message
      llmnrProcessQuery(interface, pseudoHeader, udpHeader, message, length);
#endif
   }
   //LLMNR response received?
   else
   {
#if (LLMNR_CLIENT_SUPPORT == ENABLED)
      //Process incoming LLMNR response message
      llmnrProcessResponse(interface, pseudoHeader, udpHeader, message, length);
#endif
   }
}

#endif
