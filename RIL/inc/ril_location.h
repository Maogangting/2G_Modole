/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_location.c'.
 |      NOTE: The interfaces are based on portable codes. Therefore they are not related to platform.
 |
 |--------------------------------------------------------------------------
 |
 |  Designed by     :   K.chen
 |  Coded    by     :   K.chen
 |  Tested   by     :   K.chen
 |--------------------------------------------------------------------------
 | Revision History
 | ----------------
 |  2017/1/1       K.chen        Create this file 
 |  ------------------------------------------------------------------------
 \=========================================================================*/
#ifndef __RIL_LOCATION_H__
#define __RIL_LOCATION_H__

#include "ol_type.h"




// Location info
typedef struct{
    char longitude[30];
    char latitude[30];
    //u16 reserved;
}ST_LocInfo;


/*****************************************************************************
 * FUNCTION
 *  RIL_GetLocation
 * DESCRIPTION
 *  This function is used to get the base station location information.
 * PARAMETERS
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_GetLocation(void);

/*****************************************************************************
 * FUNCTION
 *  RIL_GetLOcationByWifi
 * DESCRIPTION
 *  This function is used to get the base station location information.
 * PARAMETERS
 *
 *  macCOMMAsignal      [in]: MAC address and signal.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_GetLOcationByWifi(char * macCOMMAsignal);


#endif  //__RIL_LOCATION_H__

