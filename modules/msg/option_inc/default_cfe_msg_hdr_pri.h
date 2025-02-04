/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/**
 * @file
 *
 * Define cFS standard full header
 *  - Avoid direct access for portability, use APIs
 *  - Used to construct message structures
 */

#ifndef DEFAULT_CFE_MSG_HDR_PRI_H
#define DEFAULT_CFE_MSG_HDR_PRI_H

/*
 * Include Files
 */

#include "common_types.h"
#include "ccsds_hdr.h"
#include "cfe_msg_sechdr.h"
#include "cfe_msg_api_typedefs.h"

/*
 * Type Definitions
 */

/**********************************************************************
 * Structure definitions for full header
 *
 * These are based on historically supported definitions and not
 * an open source standard.
 */
#ifndef CFE_EDS_ENABLED_BUILD
/**
 * \brief Full CCSDS header
 */
typedef struct
{
    CCSDS_PrimaryHeader_t Pri; /**< \brief CCSDS Primary Header */
} CCSDS_SpacePacket_t;
#endif

/**
 * \brief cFS generic base message
 *
 * This provides the definition of CFE_MSG_Message_t
 */
union CFE_MSG_Message
{
    CCSDS_SpacePacket_t CCSDS;                             /**< \brief CCSDS Header (Pri or Pri + Ext) */
    uint8               Byte[sizeof(CCSDS_SpacePacket_t)]; /**< \brief Byte level access */
};
#ifndef CFE_EDS_ENABLED_BUILD
/**
 * \brief cFS command header
 *
 * This provides the definition of CFE_MSG_CommandHeader_t
 */
struct CFE_MSG_CommandHeader
{
    CFE_MSG_Message_t                Msg; /**< \brief Base message */
    CFE_MSG_CommandSecondaryHeader_t Sec; /**< \brief Secondary header */
};

/**
 * \brief cFS telemetry header
 *
 * This provides the definition of CFE_MSG_TelemetryHeader_t
 */
struct CFE_MSG_TelemetryHeader
{
    CFE_MSG_Message_t                  Msg;      /**< \brief Base message */
    CFE_MSG_TelemetrySecondaryHeader_t Sec;      /**< \brief Secondary header */
    uint8                              Spare[4]; /**< \brief Pad to avoid compiler padding if payload
                                                             requires 64 bit alignment */
};
#endif /* CFE_EDS_ENABLED_BUILD */
#endif /* DEFAULT_CFE_MSG_HDR_PRI_H */
