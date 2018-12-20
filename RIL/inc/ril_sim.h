/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_sim.c'.
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
#ifndef __RIL_SIM_H__
#define __RIL_SIM_H__
#include "ol_type.h"


/****************************************************************************
 * Definition for SIM Card State
 ***************************************************************************/
typedef enum {
    SIM_STAT_NOT_INSERTED = 0,
    SIM_STAT_READY,
    SIM_STAT_PIN_REQ,
    SIM_STAT_PUK_REQ,
    SIM_STAT_PH_PIN_REQ,
    SIM_STAT_PH_PUK_REQ,
    SIM_STAT_PIN2_REQ,
    SIM_STAT_PUK2_REQ,
    SIM_STAT_BUSY,
    SIM_STAT_NOT_READY,
    SIM_STAT_UNSPECIFIED
 }Enum_SIMState;


/******************************************************************************
* Function:     RIL_SIM_GetSimState
*  
* Description:
*                This function gets the state of SIM card. 
*
* Related AT:
*               "AT+CPIN?".
*
* Parameters:    
*               stat:
*                   [out]SIM card State code, one value of Enum_SIMState.
* Return:  
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
******************************************************************************/
s32 RIL_SIM_GetSimState(s32* state);

/******************************************************************************
* Function:     RIL_SIM_GetIMSI
*  
* Description:
*               This function gets the state of SIM card. 
*
* Related AT:
*               "AT+CIMI".
*
* Parameters:    
*               imsi:
*                   [out]IMSI number, a string of 15-byte.
* Return:  
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
******************************************************************************/
s32 RIL_SIM_GetIMSI(char* imsi);

/******************************************************************************
* Function:     RIL_SIM_GetCCID
*  
* Description:
*               This function gets the CCID of SIM card. 
*
* Related AT:
*               "AT+CCID".
*
* Parameters:    
*               ccid:
*                   [out] CCID number, a string of 20-byte.
* Return:  
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
******************************************************************************/
s32 RIL_SIM_GetICCID(char* ccid);

s32 RIL_SIM_GetGPRSSignal(char* sig);

#endif  //__RIL_SIM_H__

