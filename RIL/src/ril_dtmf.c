
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_dtmf.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module defines the information, and APIs related to DTMF.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "ril_dtmf.h"
#include "ril.h"
#include "ril_util.h"
//#include "ql_common.h"
#include "ol_uart.h"
#include "ol_error.h"
#include "ol_iostream.h"
#include "ol_interface.h"

#define RIL_DTMF_DEBUG_ENABLE 0
#if RIL_DTMF_DEBUG_ENABLE > 0
#define RIL_DTMF_DEBUG_PORT  UART_PORT2
static char DBG_Buffer[100];
#define RIL_DTMF_DEBUG(BUF,...)  QL_TRACE_LOG(RIL_DTMF_DEBUG_PORT,BUF,100,__VA_ARGS__)
#else
#define RIL_DTMF_DEBUG(BUF,...) 
#endif



static s32 ATResponse_DTMF_Handler(char* line, u32 len, void* userdata)
{
    ol_trace("just for dtmf test \r\n");
	return 0;
}
/****************************************************
* DTMF detection          
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
s32 RIL_ToneDet_Open(void )
{
	s32 ret = RIL_AT_FAILED;
	char strAT[200]; 
    
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+DDET=1");
	ret = ol_RIL_SendATCmd( strAT, strlen(strAT), ATResponse_DTMF_Handler, NULL, 300 ) ;          
    return ret;
}

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
s32 RIL_ToneDet_Close( void )
{
	s32 ret = RIL_AT_FAILED;
	char strAT[200]; 
   
	memset( strAT, 0, sizeof(strAT) );
	sprintf( strAT, "AT+DDET=0" );
	ret = ol_RIL_SendATCmd( strAT, strlen(strAT), ATResponse_DTMF_Handler, NULL, 300 ) ;
                    
    return ret;
}



/****************************************************
* DTMF send          
****************************************************/
/*****************************************************************
* Function:     RIL_VTS_Send
* 
* Description:
*               This function is used to play DTMF tone during the call.
*
* Parameters:
*      
*                <dtmfStr>:
*                       [IN] A single ASCII character in the set .0-9, #, *, A-D.

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
s32 RIL_VTS_Send(u8 dtmfstr)
{
	s32 ret = RIL_AT_FAILED;
	char strAT[200]; 
	
	memset( strAT, 0, sizeof(strAT) );
	sprintf( strAT, "AT+VTS=%d",dtmfstr);
	ret = ol_RIL_SendATCmd( strAT, strlen(strAT), ATResponse_DTMF_Handler, NULL, 0 ) ;
                  
    return ret;
}

