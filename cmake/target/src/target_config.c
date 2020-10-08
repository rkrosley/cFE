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
 * \file target_config.c
 *
 *  Created on: Dec 3, 2013
 *  Created by: joseph.p.hickey@nasa.gov
 *
 * Defines constant configuration structures and pointers that link together
 * the CFE core, PSP, OSAL.  The content of these configuration structures
 * can be used to avoid directly using #include to reference a function
 * implemented in another library, which can greatly simplify include paths
 * and create a more modular build.
 *
 */

#include "target_config.h"
#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"
#include "cfe_es.h"
#include "cfe_time.h"
#include "private/cfe_es_resetdata_typedef.h"
#include "cfecfs_version_info.h"
#include "cfecfs_build_info.h"


#ifndef CFE_CPU_NAME_VALUE
#define CFE_CPU_NAME_VALUE          "unknown"
#endif

#ifndef CFE_CPU_ID_VALUE
#define CFE_CPU_ID_VALUE            0
#endif

#ifndef CFE_SPACECRAFT_ID_VALUE
#define CFE_SPACECRAFT_ID_VALUE     0x42
#endif

#ifndef CFE_DEFAULT_MODULE_EXTENSION
#define CFE_DEFAULT_MODULE_EXTENSION    ""
#endif

#ifndef CFE_DEFAULT_CORE_FILENAME
#define CFE_DEFAULT_CORE_FILENAME       ""
#endif

Target_CfeConfigData GLOBAL_CFE_CONFIGDATA =
{
      /*
       * Entry points to CFE code called by the PSP
       */
      .System1HzISR = CFE_TIME_Local1HzISR,
      .SystemMain = CFE_ES_Main,
      .SystemNotify = CFE_ES_ProcessAsyncEvent,

      /*
       * Default values for Startup file.
       * This is a suggested value, but the PSP may provide a different file
       */
      .NonvolStartupFile = CFE_PLATFORM_ES_NONVOL_STARTUP_FILE,

      /*
       * Sizes of other memory segments
       */
      .CdsSize = CFE_PLATFORM_ES_CDS_SIZE,
      .ResetAreaSize = sizeof(CFE_ES_ResetData_t),
      .UserReservedSize = CFE_PLATFORM_ES_USER_RESERVED_SIZE,

      .RamDiskSectorSize = CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE,
      .RamDiskTotalSectors = CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS
};

/*
 * PSP Static Module load section
 *
 * This is in here (the target-specific file) rather than the PSP config data structure
 * because it can be different for different targets even those that share the same basic PSP.
 *
 * For instance, if two boards use the same basic HW hardware but one of them has a special
 * timer interrupt for synchronization, only this second target would include a PSP module
 * to bind to that special interrupt.
 *
 * In order to reference the driver from a dynamically generated set,
 * two inclusions of the same file are done using different definitions
 * of the LOAD_PSP_MODULE() macro.  The first pass creates "extern" references
 * to the API objects, the second pass generates a table of pointers to them.
 */

/*
 * Definition of LOAD_*_MODULE for the first pass, creates an extern declaration
 * for the API object which must be named appropriately.
 */
#define LOAD_PSP_MODULE(name)   extern CFE_StaticModuleApi_t CFE_PSP_##name##_API;
#include "psp_module_list.inc"
#undef LOAD_PSP_MODULE

/*
 * Definition of LOAD_PSP_MODULE for the second pass, creates an entry with
 * the name and a pointer to the API object.
 *
 * This is done here in the target file and NOT in the PSP because it is
 * configured specifically to each target.  The contents of the "psp_module_list.inc"
 * file are generated by the build scripts and are different for each target.
 */
#define LOAD_PSP_MODULE(name)   { .Name = #name, .Api = &CFE_PSP_##name##_API },
static CFE_StaticModuleLoadEntry_t GLOBAL_PSP_MODULELIST[] =
{
#include "psp_module_list.inc"
    { .Name = NULL }
};
#undef LOAD_PSP_MODULE


/**
 * Instantiation of global system-wide configuration struct
 * This contains build info plus pointers to the PSP and CFE
 * configuration structures.  Everything will be linked together
 * in the final executable.
 */
Target_ConfigData GLOBAL_CONFIGDATA =
{
        .MissionVersion = MISSION_VERSION,
        .CfeVersion = CFE_VERSION,
        .OsalVersion = OSAL_VERSION,
        .Config = MISSION_CONFIG,
        .Date = MISSION_BUILDDATE,
        .User = MISSION_BUILDUSER "@" MISSION_BUILDHOST,
        .Default_CpuName = CFE_CPU_NAME_VALUE,
        .Default_CpuId = CFE_CPU_ID_VALUE,
        .Default_SpacecraftId = CFE_SPACECRAFT_ID_VALUE,
        .Default_ModuleExtension = CFE_DEFAULT_MODULE_EXTENSION,
        .Default_CoreFilename = CFE_DEFAULT_CORE_FILENAME,
        .CfeConfig = &GLOBAL_CFE_CONFIGDATA,
        .PspConfig = &GLOBAL_PSP_CONFIGDATA,
        .PspModuleList = GLOBAL_PSP_MODULELIST,
};



/*
 * Instantiate a list of symbols that should be statically linked into the
 * final executable.
 *
 * This table is in turn used by OSAL if the OS_STATIC_LOADER feature is
 * enabled, such that OS_SymbolLookup may return values from this table
 * in lieu of an OS/library-provided dynamic lookup function.
 */
#define STATIC_CFS_SYMBOL(n,m)    extern void n (void);
#include "cfs_static_symbol_list.inc"
#undef  STATIC_CFS_SYMBOL
#define STATIC_CFS_SYMBOL(n,m)    { .Name = #n, .Address = n, .Module = #m },
OS_static_symbol_record_t OS_STATIC_SYMBOL_TABLE[] =
{
#include "cfs_static_symbol_list.inc"
        { NULL, NULL } /* End of list marker */
};
#undef  STATIC_CFS_SYMBOL
