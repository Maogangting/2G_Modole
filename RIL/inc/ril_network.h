/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_network.c'.
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
#ifndef __RIL_NETWORK_H__
#define __RIL_NETWORK_H__
#include "ol_type.h"
#include "ril_sim.h"


/****************************************************************************
 * Definition for network State
 ***************************************************************************/
typedef enum {
    NW_STAT_NOT_REGISTERED = 0,    // Not register to network
    NW_STAT_REGISTERED,            // The normal network state
    NW_STAT_SEARCHING,             // Searching network
    NW_STAT_REG_DENIED,            // The register request is denied
    NW_STAT_UNKNOWN,
    NW_STAT_REGISTERED_ROAMING     //Registered and Roaming state
}Enum_NetworkState;

typedef struct{
    int rssi;
    int ber;
}ST_CSQ_Reponse;

typedef struct
{
	u16	  mcc;             //Mobile country code
	u16   mnc;             //Mobile network code
	u16   lac;             //Location area code
	u16   cellid;          //Cell id
	u8    bsic;            //Base station identity code
	u16   rxl;             //Receive level          
}CELL_NETWORK_INFO;
/******************************************************************************
* Function:     RIL_NW_GetGSMState
*  
* Description:
*               This function gets the GSM network register state. 
*
* Parameters:    
*               stat:
*                   [out]GSM State.
* Return:  
*               One value of Enum_NetworkState: network register state code.
*               -1 : fail to get the network state.
******************************************************************************/
s32  RIL_NW_GetGSMState(s32 *stat);

/******************************************************************************
* Function:     RIL_NW_GetGPRSState
*  
* Description:
*               This function gets the GPRS network register state. 
*
* Parameters:    
*               stat:
*                   [out]GPRS State.
* Return:  
*               One value of Enum_NetworkState: network register state code.
*               -1 : fail to get the network state.
******************************************************************************/
s32  RIL_NW_GetGPRSState(s32 *stat);

/******************************************************************************
* Function:     RIL_NW_GetSignalQuality
*  
* Description:
*               This function gets the signal quality level and bit error rate. 
*
* Parameters:    
*               rssi:
*                   [out] Signal quality level, 0~31 or 99. 99 indicates module
*                         doesn't register to GSM network.
*                       
*               ber:
*                   [out] The bit error code of signal.
* Return:  
*               QL_RET_OK indicates success.
*               QL_RET_ERR_INVALID_PARAMETER indicates something wrong for input parameters. 
******************************************************************************/
s32  RIL_NW_GetSignalQuality(u32* rssi, u32* ber);



/******************************************************************************
* Function:     RIL_NW_SetAPN
*  
* Description:
*               This function sets the APN for the current context.
*
* Parameters:    
*               
*               apn:
*                   [in] pointer to the APN name.
*
*               userName:
*                   [in] pointer to the user name.
*
*               pw:
*                   [in] pointer to the password.
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_SetAPN(char* apn, char* userName, char* pw);


/******************************************************************************
* Function:     RIL_NW_OpenPDPContext
*  
* Description:
*               This function opens/activates the GPRS PDP context. 
*
* Parameters:    
*               None.
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_OpenPDPContext(void);

/******************************************************************************
* Function:     RIL_NW_ClosePDPContext
*  
* Description:
*               This function closes/deactivates the GPRS PDP context.
*
* Parameters:    
*               None.
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_ClosePDPContext(void);

/******************************************************************************
* Function:     RIL_NW_GetOperator
*  
* Description:
*               This function gets the network operator that module registered.
*
* Parameters:    
*               operator:
*                   [out] a string with max 16 characters, which indicates the 
*                   network operator that module registered. 
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_GetOperator(char* operator);

/******************************************************************************
* Function:     RIL_NW_GetNetInfo
*  
* Description:
*               This function get the network infomation.
*
* Parameters:    
*               cell_info:
*                   [out] a struct. 
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.              
******************************************************************************/
s32  RIL_NW_GetNetInfo(CELL_NETWORK_INFO * cell_info);


/******************************************************************************
* Function:     RIL_NW_CheckStatus
*  
* Description:
*               This function is Check before the Internet.
*
* Parameters:    
*               void
* Return:  
*               RIL_AT_SUCCESS,send AT successfully.
*               RIL_AT_FAILED, send AT failed.
*               RIL_AT_TIMEOUT,send AT timeout.
*               RIL_AT_BUSY,   sending AT.
*               RIL_AT_INVALID_PARAM, invalid input parameter.              
******************************************************************************/
s32  RIL_NW_CheckStatus(void);
#endif // __RIL_NETWORK_H__
