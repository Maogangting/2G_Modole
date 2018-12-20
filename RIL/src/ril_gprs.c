/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_location.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module defines the information, and APIs related to RIL.
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
#include "ril_gprs.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_network.h"
#include "ril_sim.h"
#include "ol_uart.h"
#include "ol_error.h"
#include "ol_interface.h"
#include "ol_iostream.h"
#include "ol_system.h"

#define RIL_LOC_DEBUG_ENABLE 0
#if RIL_LOC_DEBUG_ENABLE > 0
#define RIL_LOC_DEBUG_PORT  UART_PORT2
static char DBG_Buffer[100];
#define RIL_LOC_DEBUG(BUF,...)  QL_TRACE_LOG(RIL_LOC_DEBUG_PORT,BUF,100,__VA_ARGS__)
#else
#define RIL_LOC_DEBUG(BUF,...) 
#endif

s32 Callback_ATGPSResponse(char*  line, s32 len, void* userdata)
{
	char* head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
    if(head)
    {  
       ol_trace("Callback_ATGTPOSResponse   OK \r\n");
	   return  RIL_ATRSP_SUCCESS;
    }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		ol_trace("Callback_ATGTPOSResponse   ERROR \r\n");
		return	RIL_ATRSP_FAILED;
	} 
	return RIL_ATRSP_FAILED;
	
}

static s32 ATResponse_MGPSTS_Handler(char* line, u32 len, void* userdata)
{
    char *head = ol_RIL_FindString_ext(line, len, "+MGPSTS:"); //continue wait
    ol_trace("mgpsts-----%s\r\n",head);
    if(head)
    {
        s32 n = 0;
        s32 *state = (s32 *)userdata;
        Ol_sscanf(head,"%*[^ ]%d,%[^\r\n]",state);
		ol_trace("ATResponse_MGPSTS_Handler---state= %d\r\n",*state);
        return  RIL_ATRSP_CONTINUE;
    }
   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>  <LF>OK<LF>
   if(head)
   {
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>  <LF>ERROR<LF>
    if(head)
    {  
        return  RIL_ATRSP_FAILED;
    } 

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32  RIL_ReadSynchronizationTimeInfo(s32 *stat)
{
	s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+MGPSTS?\0";
	retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), ATResponse_MGPSTS_Handler,&nStat,1000);
	if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("RIL_ReadSynchronizationTimeInfo--- stat= %d\r\n",*stat);
    }
	return retRes;

}
static s32 ATResponse_MGPSEPO_Handler(char* line, u32 len, void* userdata)
{
    char *head = ol_RIL_FindString_ext(line, len, "+MGPSEPO:"); //continue wait
    ol_trace("mgpsts-----%s\r\n",head);
    if(head)
    {
        s32 n = 0;
        s32 *state = (s32 *)userdata;
        Ol_sscanf(head,"%*[^ ]%d,%[^\r\n]",state);
		ol_trace("ATResponse_MGPSEPO_Handler---state= %d\r\n",*state);
        return  RIL_ATRSP_CONTINUE;
    }

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>  <LF>OK<LF>
   if(head)
   {
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>  <LF>ERROR<LF>
    if(head)
    {  
        return  RIL_ATRSP_FAILED;
    } 

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32  RIL_ReadDownLoadEpoInfo(s32 *stat)
{
	s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+MGPSEPO?\0";
	retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), ATResponse_MGPSEPO_Handler,&nStat,1000);
	if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("RIL_ReadDownLoadEpoInfo--- stat= %d\r\n",*stat);
    }
	return retRes;
}
static s32 ATResponse_CGATT_Handler(char* line, u32 len, void* userdata)
{
    char *head = ol_RIL_FindString_ext(line, len, "+CGATT:"); //continue wait
    ol_trace("mgpsts-----%s\r\n",head);
    if(head)
    {
        s32 n = 0;
        s32 *state = (s32 *)userdata;
        Ol_sscanf(head,"%*[^ ]%d,%[^\r\n]",state);
		ol_trace("ATResponse_CGATT_Handler----state= %d\r\n",*state);
        return  RIL_ATRSP_CONTINUE;
    }

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>  <LF>OK<LF>
   if(head)
   {
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>  <LF>ERROR<LF>
    if(head)
    {  
        return  RIL_ATRSP_FAILED;
    } 

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32  RIL_ReadGpsAttachedInfo(s32 *stat)
{
	s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CGATT?\0";
	retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), ATResponse_CGATT_Handler,&nStat,1000);
	if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("RIL_ReadGpsAttachedInfo--- stat= %d\r\n",*stat);
    }
	return retRes;

}

s32  RIL_OpenColdStart(char*status)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+MGPSS=\"%s\"",status);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}
s32  RIL_SetBandRate(char status)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+IPR=%d",status);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}

s32  RIL_OpenGpsFunction(char status)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+MGPSC=%d",status);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}

s32  RIL_SetReadDownLoadFileEpo(char status)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+MGPSEPO=%d",status);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}

s32  RIL_SetSynchronizationTime(char status)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+MGPSTS=%d",status);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}
s32  RIL_SetImeiNumber(char op, char type, char* str)
{
	s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+MGPSTS=%d,%d,\"%s\"",op,type,str);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;
}

s32  RIL_SetPdp(char cid, char* PDP_type, char* apn)
{
    s32 retRes = 0;
    char strAT[80] ;
    memset(strAT,0x00, sizeof(strAT));
    sprintf(strAT,"AT+EGDCONT=%d,\"%s\",\"%s\"",cid,PDP_type,apn);
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_ATGPSResponse,NULL,1000);
    return retRes;

}
s32 RIL_GPRS_CheckStatus(void)
{
	s32 ret = RIL_AT_SUCCESS;
	s32 cgreg_state,sim_state,i,attach_state;

	for(i=0; i<5; i++)
	{
		RIL_NW_GetGPRSState(&cgreg_state);
		RIL_SIM_GetSimState(&sim_state);
		RIL_ReadGpsAttachedInfo(&attach_state);
		ol_Sleep(500000);
		if(cgreg_state == 1 && sim_state == 1&& attach_state== 1)
		{
			return RIL_AT_SUCCESS;
		}	
	}
	if(cgreg_state != 1 || sim_state != 1|| attach_state !=1)
	{
		return RIL_AT_FAILED;
	}
	return RIL_AT_SUCCESS;
}
s32 RIL_AGPS_Configuration_CheckStatus(void)
{
	s32 ret = RIL_AT_SUCCESS;
	s32 downloadepo_state,SynchronizationTime_state,i;
	for(i=0; i<5; i++)
	{
		RIL_ReadDownLoadEpoInfo(&downloadepo_state);
		RIL_ReadSynchronizationTimeInfo(&SynchronizationTime_state);
		ol_Sleep(500000);
		if(downloadepo_state == 1 && SynchronizationTime_state == 1)
		{
			return RIL_AT_SUCCESS;
		}	
	}
	if(downloadepo_state != 1 || SynchronizationTime_state != 1)
	{
		return RIL_AT_FAILED;
	}
	return RIL_AT_SUCCESS;
}



