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

/******************************************************************************
 * Message default header initialization - implementation without CCSDS
 * extended header
 */
#ifndef CFE_EDS_ENABLED_BUILD
#include "cfe_msg_hdr.h"
#else
#include <stddef.h>
#include "cfe_msg_eds_typedefs.h"
#endif /* CFE_EDS_ENABLED_BUILD */
#include "cfe_msg_defaults.h"

/*----------------------------------------------------------------
 *
 * Function: CFE_MSG_InitDefaultHdr
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
void CFE_MSG_InitDefaultHdr(CFE_MSG_Message_t *MsgPtr)
{
    CFE_MSG_SetDefaultCCSDSPri(MsgPtr);
}
