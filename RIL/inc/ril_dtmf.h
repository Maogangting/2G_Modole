/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_dtmf.c'.
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
#ifndef __RIL_DTMF_H__
#define __RIL_DTMF_H__

#include "ol_type.h"





/****************************************************
* DTMF detection function definition         
****************************************************/
/*****************************************************************
* Function:     RIL_ToneDet_Open
* 
* Description:
*               This function is used to Open DTMF detect.
*
* Return:        
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.                
*****************************************************************/
s32 RIL_ToneDet_Open(void);

/*****************************************************************
* Function:     RIL_ToneDet_Close
* 
* Description:
*               The function is used to close DTMF detect.
*
* Parameters:
*
* Return:        
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.                
*****************************************************************/
s32 RIL_ToneDet_Close( void );


/****************************************************
* DTMF send  functions definition        
****************************************************/
/*****************************************************************
* Function:     RIL_VTS_Send
* 
* Description:
*               This function is used to play DTMF tone during the call.
*
* Parameters:
*                dtmfStr:
*                     [IN] this string consists of DTMF tone strings,A single ASCII character in the set .0-9, #, *, A-D.
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.               
*****************************************************************/
s32 RIL_VTS_Send(u8 dtmfStr);


#endif  //__RIL_DTMF_H__

