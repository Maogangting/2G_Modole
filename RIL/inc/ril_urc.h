/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_urc.c'.
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
#ifndef __RIL_URC_H__
#define __RIL_URC_H__
#include "ol_type.h"


#define  MSG_ID_URC_INDICATION  0x1999

typedef enum
{
	start_record = 0,
	stop_record,
	play_recordfile,
	stop_playing,
	delete_file,
	play_file_incall,
	stop_play_incall,
	free_operation
}Enum_Record_Operation;

typedef enum
{
	end_of_file_play = 0,
	not_enough_space,
	space_full,
	idle_state,
	recording_now,
	playing_now
}Enum_Record_State;


typedef enum
{
	Network_busy = -1,
	LBS_not_ready = -2,
	Network_error = -3,
	Network_timeout = -4,
	Network_unack = -5,
	Network_EXISTS = -6,
	WIFI_information_error = -7,
}Enum_Gtpos_State;


typedef struct
{
	s32 index;
	s32 state;
}Urc_Network_T;


typedef struct
{
	s32 index;
	u8 mem[5];
}Urc_Sms_T;

typedef struct
{
	s32 index;
	s32 ftp_ret;
}Urc_Ftp_T;

typedef struct
{
	s32 oper;
	s32 state;
}Urc_Zaudrec_T;

typedef struct{
   u8 time_out;
   u8 clean;
   u32 keepalive;
   u8 * version;
}Mqtt_Info;

typedef struct
{
	u16 packet_id[32];
	u8  topic[100];		 
	u8  message[1360];		    
}Urc_ALIMqtt_T;
typedef struct
{
	ol_bool status;		    
}Urc_ALIAuth_T;
typedef struct
{
	u16  count;
	u8  topic[20];		 
	u8  message[64];			// mgt	    
}Urc_Mqtt_T_1;
/*****************************************************************
* Function:     OnURCHandler 
* 
* Description:
*               This function is the entrance for Unsolicited Result Code (URC) Handler.
*
* Parameters:
*               strURC:      
*                   [IN] a URC string terminated by '\0'.
*
*               reserved:       
*                   reserved, can be NULL.
* Return:        
*               The function returns "ptrUrc".
*****************************************************************/
void OnURCHandler(const char* strURC, void* reserved);




/******************************************************************************
* Function:     Ol_RIL_IsURCStr
*  
* Description:
*               This function is used to check whether a string is URC information
*               you defined.
.
* Parameters:    
*               strRsp: 
*                     [in]a string for the response of the AT command.
* Return:  
*               0 : not URC information
*               1 : URC information
******************************************************************************/
s32 Ol_RIL_IsURCStr(const char* strRsp);


#endif