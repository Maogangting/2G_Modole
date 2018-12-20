/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_telphony.c'.
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
#ifndef __RIL_TELEPHONY_H__
#define __RIL_TELEPHONY_H__
#include "ol_type.h"

#define PHONE_NUMBER_MAX_LEN 41
typedef struct {
    s32 type;
    char phoneNumber[PHONE_NUMBER_MAX_LEN];
}ST_ComingCall;

typedef struct {
    u32  ringCnt;
    ST_ComingCall comingCall[6];
}ST_ComingCallInfo;

typedef enum {
    CALL_STATE_ERROR = -1,
    CALL_STATE_OK = 0,
    CALL_STATE_BUSY,
    CALL_STATE_NO_ANSWER,
    CALL_STATE_NO_CARRIER,
    CALL_STATE_NO_DIALTONE,
    CALL_STATE_END
}Enum_CallState;

/******************************************************************************
* Function:     RIL_Telephony_Dial
*  
* Description:
*               This function dials the specified phone number, only support voice call.
*
* Parameters:    
*               type:
*                   [In] Must be 0 , just support voice call.
*               phoneNumber:
*                   [In] Phone number, null-terminated string.
*               result:
*                   [Out] Result for dial, one value of Enum_CallState.
*
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32 RIL_Telephony_Dial(u8 type, char* phoneNumber, s32* result);

/******************************************************************************
* Function:     RIL_Telephony_Dial
*  
* Description:
*               This function answers a coming call.
*
* Parameters:    
*               result:
*                   [Out] Delete flag , which is one value of 'Enum_SMSDeleteFlag'.
*
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32 RIL_Telephony_Answer(s32 *result);

/******************************************************************************
* Function:     RIL_Telephony_Dial
*  
* Description:
*               This function answers a call.
*
* Parameters:    
*               None
*
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32 RIL_Telephony_Hangup(void);

#endif
