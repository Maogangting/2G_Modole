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
 |  Designed by     :   y.liao
 |  Coded    by     :   y.liao
 |  Tested   by     :  	y.liao
 |--------------------------------------------------------------------------
 | Revision History
 | ----------------
 |  2017/12/12       y.liao       Create this file 
 |  ------------------------------------------------------------------------
 \=========================================================================*/
#ifndef __RIL_GPRS_H__
#define __RIL_GPRS_H__
#include "ol_type.h"
s32  RIL_ReadSynchronizationTimeInfo(s32 *stat);
s32  RIL_ReadDownLoadEpoInfo(s32 *stat);
s32  RIL_ReadGpsAttachedInfo(s32 *stat);
s32  RIL_OpenColdStart(char*status);
s32  RIL_SetBandRate(char status);
s32  RIL_OpenGpsFunction(char status);
s32  RIL_SetImeiNumber(char op, char type, char* str);
s32  RIL_SetReadDownLoadFileEpo(char status);
s32  RIL_SetSynchronizationTime(char status);
s32  RIL_SetPdp(char cid, char* PDP_type, char* apn);
s32  RIL_GPRS_CheckStatus(void);
s32  RIL_AGPS_Configuration_CheckStatus(void);





#endif  //__RIL_LOCATION_H__

