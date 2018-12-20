
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_network.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module implements network related APIs.
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
#include "ril_network.h"
#include "ril.h"
#include "ril_util.h"
#include "stdlib.h"
#include "ol_error.h"
#include "ol_system.h"
#include "ol_iostream.h"
#include "ol_interface.h"
#include "ol_ril.h"
#include <string.h>
#include <stdio.h>
#include "ril_sim.h"



s32 Callback_Null(char* line, u32 len, void* userdata)
{
	ol_trace("Callback_Null----just for test \r\n");
}

/******************************************************************************
* Function:     ATResponse_CREG_Handler
*  
* Description:
*               This function is used to deal with the response of the AT+CREG command.
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
static s32 ATResponse_CREG_Handler(char* line, u32 len, void* userdata)
{
    char *head = ol_RIL_FindString_ext(line, len, "+CREG:"); //continue wait
    ol_trace("head-----%s\r\n",head);
    if(head)
    {
        s32 n = 0;
        s32 *state = (s32 *)userdata;
        Ol_sscanf(head,"%*[^ ]%d,%d,%[^\r\n]",&n,state);
		ol_trace("ATResponse_CREG_Handler---n= %d,state= %d\r\n",n,*state);
        return  RIL_ATRSP_CONTINUE;
    }

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
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

/******************************************************************************
* Function:     RIL_NW_GetGSMState
*  
* Description:
*               This function gets the GSM network register state. 
*
* Parameters:    
*               stat:
*                   [out]GPRS State.
* Return:
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_GetGSMState(s32 *stat)
{
    s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CREG?\0";

    retRes = ol_RIL_SendATCmd(strAT, strlen(strAT), ATResponse_CREG_Handler, &nStat, 1000);
    if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("RIL_NW_GetGSMState--- stat= %d\r\n",*stat);
    }
    return retRes;
}

/******************************************************************************
* Function:     ATResponse_CGREG_Handler
*  
* Description:
*               This function is used to deal with the response of the AT+CGREG command.
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
static s32 ATResponse_CGREG_Handler(char* line, u32 len, void* userdata)
{
    char *head = ol_RIL_FindString_ext(line, len, "+CGREG:"); //continue wait
    if(head)
    {
        s32 n = 0;
        s32 *state = (s32 *)userdata;
        Ol_sscanf(head,"%*[^ ]%d,%d,%[^\r\n]",&n,state);
        return  RIL_ATRSP_CONTINUE;
    }

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
       return  RIL_ATRSP_SUCCESS;
   }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
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
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.
******************************************************************************/
s32  RIL_NW_GetGPRSState(s32 *stat)
{
    s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CGREG?";

    retRes = ol_RIL_SendATCmd(strAT, strlen(strAT), ATResponse_CGREG_Handler, &nStat, 1000);
    if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
	   ol_trace("RIL_NW_GetGPRSState--- stat= %d\r\n",*stat);
    }
    return retRes;
}


/******************************************************************************
* Function:     ATResponse_CSQ_Handler
*  
* Description:
*               This function is used to deal with the response of the AT+CSQ command.
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
static s32 ATResponse_CSQ_Handler(char* line, u32 len, void* userdata)
{
    ST_CSQ_Reponse *CSQ_Reponse = (ST_CSQ_Reponse*)userdata;

    char *head = ol_RIL_FindString_ext(line, len, "+CSQ:"); //continue wait
    ol_trace("csq-head----= %s\r\n",head);
    if(head)
    {
        Ol_sscanf(head,"%*[^ ]%d,%d,%[^\r\n]",&CSQ_Reponse->rssi,&CSQ_Reponse->ber);
        return  RIL_ATRSP_CONTINUE;
    }

    head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
    if(head)
    {  
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
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
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL. 
******************************************************************************/
s32  RIL_NW_GetSignalQuality(u32* rssi, u32* ber)
{
    s32 retRes = 0;
    char strAT[] = "AT+CSQ\0";
    ST_CSQ_Reponse pCSQ_Reponse;
    memset(&pCSQ_Reponse,0, sizeof(pCSQ_Reponse));
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), ATResponse_CSQ_Handler,(void*)&pCSQ_Reponse,1000);
    if(RIL_AT_SUCCESS == retRes)
    {
       *rssi = pCSQ_Reponse.rssi; 
       *ber = pCSQ_Reponse.ber;
    }
 
    return retRes;
}



s32  RIL_NW_SetAPN(char* apn, char* userName, char* pw)
{
    s32 retRes = 0;
    char strAT[80] ;

    memset(strAT,0x00, sizeof(strAT));
    if((NULL != apn) && (NULL != userName) && (NULL != pw))
    {
        sprintf(strAT,"AT+CSTT=\"%s\",\"%s\",\"%s\"",apn,userName,pw);
    }
    else if((NULL != apn) && (NULL != userName) && (NULL == pw))
    {
        sprintf(strAT,"AT+CSTT=\"%s\",\"%s\"",apn,userName);
    }
    else if((NULL != apn) && (NULL == userName) &&(NULL == pw))
    {
        sprintf(strAT,"AT+CSTT=\"%s\"",apn);
    }
    
        
    retRes = ol_RIL_SendATCmd(strAT,strlen(strAT), Callback_Null,NULL,1000);
    return retRes;

}


//
// This function activates pdp context for AT command mode.
s32  RIL_NW_OpenPDPContext(void)
{
    s32 retRes;
    char strAT[] = "AT+CIICR\0";
    retRes = ol_RIL_SendATCmd(strAT, strlen(strAT), Callback_Null, NULL,3000);
    return retRes;
}

static s32 ATRsp_CIPSHUT_Hdlr(char* line, u32 len, void* userData)
{
    if (ol_RIL_FindLine_ext(line, len, "SHUT OK"))
    {
        return  RIL_ATRSP_SUCCESS;
    } 
    else if (ol_RIL_FindLine_ext(line, len, "ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE; //continue wait
}

s32 RIL_NW_ClosePDPContext(void)
{
    char strAT[] = "AT+CIPSHUT\0";
    return ol_RIL_SendATCmd("AT+CIPSHUT\n", strlen(strAT), Callback_Null, NULL, 1000);
}

static s32 ATRsp_COPS_Handler(char* line, u32 len, void* param)
{
    char* pStr = (char *)param;
    char* pHead = ol_RIL_FindString_ext(line, len, "+COPS:"); //continue wait
    if (pHead)
    {
        char str[100] = {0};
        char *p = NULL;
        char *q = NULL; 
        p = pHead + strlen("+COPS: ");
        q = strstr(p, "\"");
        if (p)
        {// the response is like: +COPS: 0,0,"CHINA MOBILE"
            p = q + 1;
            q = strstr(p, "\"");
            if (q != NULL)
            {
                memcpy(pStr, p, q - p);
                pStr[q - p] = '\0';
            }
        }
        else
        {//  the response is like +COPS: 0
            *pStr = '\0';
        }
        return  RIL_ATRSP_SUCCESS;
    }

   pHead = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if (pHead)
   {  
       return  RIL_ATRSP_SUCCESS;
   }

    pHead = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
    if (pHead)
    {  
        return  RIL_ATRSP_FAILED;
    } 

    pHead = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if (pHead)
    {
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32 RIL_NW_GetOperator(char* operator)
{
    char strAT[] = "AT+COPS?";
    if (NULL == operator)
    {
        return RIL_AT_INVALID_PARAM;
    }
    return ol_RIL_SendATCmd(strAT, strlen(strAT), ATRsp_COPS_Handler,(void*)operator, 1000);
}


static s32 ATRsp_CENG_Handler(char* line, u32 len, void* param)
{
	CELL_NETWORK_INFO* cell_info = (CELL_NETWORK_INFO *)param;
	char* pHead = ol_RIL_FindString_ext(line, len, "+CENG:"); //continue wait
	if (pHead)
	{	
		char* p1 = NULL;
		char* p2 = NULL;

		p1 = strstr(pHead,":");
		cell_info->mcc = Ol_atoi(p1+1);

		p2 = strstr(p1+1,",");
		cell_info->mnc = Ol_atoi(p2+1);

		p1 = strstr(p2+1,",");
		cell_info->lac = Ol_atoi(p1+1);

		p2 = strstr(p1+1,",");
		cell_info->cellid = Ol_atoi(p2+1);

		p1 = strstr(p2+1,",");
		cell_info->bsic = Ol_atoi(p1+1);

		p2 = strstr(p1+1,",");
		cell_info->rxl = Ol_atoi(p2+1);
	
		{
			char buffer[128]={0};
	        ol_trace("---buffer: %d,%d,%d,%d,%d,%d\r\n",cell_info->mcc,cell_info->mnc,cell_info->lac,cell_info->cellid,cell_info->bsic,cell_info->rxl);
		}
		return  RIL_ATRSP_SUCCESS;
	}

	pHead = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
	if (pHead)
	{  
		return  RIL_ATRSP_SUCCESS;
	}

	pHead = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if (pHead)
	{  
		return  RIL_ATRSP_FAILED;
	} 

	pHead = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if (pHead)
	{
		return  RIL_ATRSP_FAILED;
	}

	return RIL_ATRSP_CONTINUE; //continue wait
}

s32 RIL_NW_GetNetInfo(CELL_NETWORK_INFO * cell_info)
{
	s32 ret = RIL_AT_SUCCESS;
    char strAT[80];

	memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+CENG=%d",3);
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_Null,NULL,1000);
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }

	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+CENG?");
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATRsp_CENG_Handler,(void*)cell_info,3000);
	if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
	return RIL_AT_SUCCESS;
}



s32 RIL_NW_CheckStatus(void)
{
	s32 ret = RIL_AT_SUCCESS;
	s32 cgreg_state,sim_state,i;

	for(i=0; i<5; i++)
	{
		RIL_NW_GetGPRSState(&cgreg_state);
		RIL_SIM_GetSimState(&sim_state);
		ol_Sleep(500000);
		if(cgreg_state == 1 && sim_state == 1)
		{
			return RIL_AT_SUCCESS;
		}	
	}

	if(cgreg_state != 1 || sim_state != 1)
	{
		return RIL_AT_FAILED;
	}
	return RIL_AT_SUCCESS;
}
//#endif  //__OCPU_RIL_SUPPORT__

