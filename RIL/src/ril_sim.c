
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_sim.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module implements sim card related APIs.
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
//#include "custom_feature_def.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_sim.h"
#include "stdlib.h"
#include "ol_error.h"
#include "ol_ril.h"
#include "ol_interface.h"
#include <string.h>
#include "ol_iostream.h"
#include "ol_system.h"
//#include "ql_system.h"




s32 RIL_SIM_GetSimStateByName(char* simStat, u32 len)
{
	s32 ss = SIM_STAT_UNSPECIFIED;
	if ((strncmp(simStat, "READY", len) == 0) || (strncmp(simStat, "ready", len) == 0))
	{
		ss = SIM_STAT_READY;
	}
	else if ((strncmp(simStat, "NOT INSERTED", len) == 0) || (strncmp(simStat, "not inserted", len) == 0))
	{
		ss = SIM_STAT_NOT_INSERTED;
	}
	else if ((strncmp(simStat, "SIM PIN", len) == 0) || (strncmp(simStat, "sim pin", len) == 0))
	{
		ss = SIM_STAT_PIN_REQ;
	}
	else if ((strncmp(simStat, "SIM PUK", len) == 0) || (strncmp(simStat, "sim puk", len) == 0))
	{
		ss = SIM_STAT_PUK_REQ;
	}
	else if ((strncmp(simStat, "PH-SIM PIN", len) == 0) || (strncmp(simStat, "ph-sim pin", len) == 0))
	{
		ss = SIM_STAT_PH_PIN_REQ;
	}
	else if ((strncmp(simStat, "PH-SIM PUK", len) == 0) || (strncmp(simStat, "ph-sim puk", len) == 0))
	{
		ss = SIM_STAT_PH_PUK_REQ;
	}
	else if ((strncmp(simStat, "SIM PIN2", len) == 0) || (strncmp(simStat, "sim pin2", len) == 0))
	{
		ss = SIM_STAT_PIN2_REQ;
	}
	else if ((strncmp(simStat, "SIM PUK2", len) == 0) || (strncmp(simStat, "sim puk2", len) == 0))
	{
		ss = SIM_STAT_PUK2_REQ;
	}
	else if ((strncmp(simStat, "SIM BUSY", len) == 0) || (strncmp(simStat, "sim busy", len) == 0))
	{
		ss = SIM_STAT_BUSY;
	}
	else if ((strncmp(simStat, "NOT READY", len) == 0) || (strncmp(simStat, "not ready", len) == 0))
	{
		ss = SIM_STAT_NOT_READY;
	}

	ol_trace("RIL_SIM_GetSimStateByName---- ss= %d\r\n",ss);
	return ss;
}



/******************************************************************************
* Function:     ATResponse_CPIN_Handler
*  
* Description:
*               This function is used to deal with the response of the AT+CPIN command.
*
* Parameters:    
*                line:  
*                    [in]The address of the string.
*                len:   
*                    [in]The length of the string.
*                userdata: 
*                    [out]Used to transfer the customer's parameter.
*                       
* Return:  
*               RIL_ATRSP_SUCCESS, indicates AT executed successfully..
*               RIL_ATRSP_FAILED, indicates AT executed failed. 
*               RIL_ATRSP_CONTINUE,indicates continue to wait for the response
*               of the AT command.
* Notes:
*               1.Can't send any new AT commands in this function.
*               2.RIL handle the AT response line by line, so this function may 
*                 be called multiple times.
******************************************************************************/
static s32 ATResponse_CPIN_Handler(char* line, u32 len, void* userdata)
{
    s32 *result = (s32 *)userdata;
    char* head = ol_RIL_FindString_ext(line, len, "+CPIN:"); //continue wait
    if(head)
    {
        char str[100] = {0};
        char *p = NULL;
        char *q = NULL; 
        p = head + strlen("+CPIN: ");
        q = strstr(p,"\r\n");
        if (p)
        {
            memcpy(str, p, q - p);
        }
        
        *result = RIL_SIM_GetSimStateByName(str,strlen(str));
        return  RIL_ATRSP_SUCCESS;
    }

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>��<LF>OK<LF>
   if(head)
   {  
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>��<LF>ERROR<LF>
    if(head)
    {  
        *result = SIM_STAT_UNSPECIFIED;
        return  RIL_ATRSP_FAILED;
    } 

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
    	s32 err = 0;
        char str[100] = {0};
        char *p = NULL;
        char *q = NULL; 
        p = head + strlen("+CME ERROR: SIM ");
        q = strstr(p,"\r\n");
        if (p)
        {
            memcpy(str, p, q - p);
        }
       
        *result = RIL_SIM_GetSimStateByName(str,strlen(str));
        
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

/******************************************************************************
* Function:     RIL_SIM_GetSimState
*  
* Description:
*                This function gets the state of SIM card. 
*
* Parameters:    
*               stat:
*                   [out]SIM card State.
* Return:  
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_SIM_GetSimState(s32 *stat)
{
    s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CPIN?";

    retRes = ol_RIL_SendATCmd(strAT, strlen(strAT), ATResponse_CPIN_Handler, &nStat, 1000);
    if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("nstat= %d\r\n",nStat);
    }
    return retRes;
}

static s32 ATRsp_IMSI_Handler(char* line, u32 len, void* param)
{
    char* pHead = NULL;
    pHead = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>��<LF>OK<LF>
    if (pHead)
    {  
    	memcpy((char*)param, line, len - 2);
        return RIL_ATRSP_SUCCESS;
    }

    pHead = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>��<LF>ERROR<LF>
    if (pHead)
    {  
        return RIL_ATRSP_FAILED;
    } 

    pHead = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if (pHead)
    {
        return RIL_ATRSP_FAILED;
    }
    memcpy((char*)param, line, len - 2); // <imsi number>\r\n
    return RIL_ATRSP_CONTINUE; //continue wait
}
s32 RIL_SIM_GetIMSI(char* imsi)
{
    char strAT[] = "AT+CIMI";
    if (NULL == imsi)
    {
        return RIL_AT_INVALID_PARAM;
    }
    return ol_RIL_SendATCmd(strAT, strlen(strAT), ATRsp_IMSI_Handler,(void*)imsi, 1000);
}
static s32 ATRsp_GPRS_Handler(char* line, u32 len, void* param)
{
    char* pHead = NULL;
    pHead = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>��<LF>OK<LF>
    if (pHead)
    {
    	memcpy((char*)param, line, len - 2);
        return RIL_ATRSP_SUCCESS;
    }

    pHead = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>��<LF>ERROR<LF>
    if (pHead)
    {
        return RIL_ATRSP_FAILED;
    }

    pHead = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if (pHead)
    {
        return RIL_ATRSP_FAILED;
    }
    memcpy((char*)param, line, len - 2); // <imsi number>\r\n
    return RIL_ATRSP_CONTINUE; //continue wait
}
s32 RIL_SIM_GetGPRSSignal(char* sig)
{
    char strAT[] = "AT+CSQ";
    if (NULL == sig)
    {
        return RIL_AT_INVALID_PARAM;
    }
    return ol_RIL_SendATCmd(strAT, strlen(strAT), ATRsp_GPRS_Handler,(void*)sig, 1000);
}
static s32 ATRsp_ICCID_Handler(char* line, u32 len, void* param)
{
    char* pHead = ol_RIL_FindString_ext(line, len, "+ICCID:");
    if (pHead)
    {
        sprintf((char*)param,"%s",pHead);
        return  RIL_ATRSP_CONTINUE; // wait for OK
    }

    pHead = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>��<LF>OK<LF>
    if (pHead)
    {  
        return RIL_ATRSP_SUCCESS;
    }

    pHead = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>��<LF>ERROR<LF>
    if (pHead)
    {  
        return RIL_ATRSP_FAILED;
    } 

    pHead = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if (pHead)
    {
        return RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}
s32 RIL_SIM_GetICCID(char* ccid)
{
    char strAT[] = "AT+ICCID";
    if (NULL == ccid)
    {
        return RIL_AT_INVALID_PARAM;
    }
    return ol_RIL_SendATCmd(strAT,strlen(strAT), ATRsp_ICCID_Handler,(void*)ccid, 1000);
}



