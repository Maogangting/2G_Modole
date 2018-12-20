/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_http.c 
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
#include "ril.h"
#include "ril_util.h"
#include "ol_uart.h"
#include "ol_error.h"
#include "ol_iostream.h"
#include "ol_interface.h"
#include "ril_http.h"


s32 Callback_null(char*  ptr, s32 len, void* param)
{

	return 0;
}


s32 ATResponse_HTTP_Handler(char*  line, s32 len, void* param)
{
	char *head = ol_RIL_FindString_ext(line, len, "+HTTPACTION:"); //continue wait
	
    if(head)
    {
        Enum_HTTP_Action state;
        s32 ret_code = 0;
		s32 size = 0;
		char * p1 = NULL;
		p1 = strstr(head,":");
        Ol_sscanf(p1+1,"%d,%d,%d,%[^\r\n]",&state,&ret_code,&size);
		ol_trace("ATResponse_HTTP_Handler---state= %d,ret_code= %d,size= %d\r\n",state,ret_code,size);
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

	return RIL_ATRSP_CONTINUE;
}



s32 RIL_HTTP_ACTIVATE(u8 * apn)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	if(NULL != apn)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+SAPBR=3,1,\"%s\",\"%s\"","APN",apn);
    	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,3000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
		
		memset(strAT,0,sizeof(strAT));
		sprintf(strAT, "AT+SAPBR=1,1");
    	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,3000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	return ret;
}


s32 RIL_HTTP_INITIALIZE(void)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	
	sprintf(strAT, "AT+HTTPINIT");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,2000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}

s32 RIL_HTTP_CLOSE(void)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	
	sprintf(strAT, "AT+HTTPTERM");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}



s32 RIL_HTTP_SetServer_URL(char * strURL)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	
	sprintf(strAT, "AT+HTTPPARA=\"%s\",1","CID");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,2000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}

	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",\"%s\"","URL",strURL);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,4000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}


s32 RIL_HTTP_SetServer_PROXY(char * strIP)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",\"%s\"","PROIP",strIP);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}


s32 RIL_HTTP_SetServer_PORT(u32 port)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",%d","PROPORT",port);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}


s32 RIL_HTTP_SetServer_REDIR(u32 flag)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",%d","REDIR",flag);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}

s32 RIL_HTTP_SetServer_BREAK(u32 scope)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",%d","BREAK",scope);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}


s32 RIL_HTTP_SetServer_BREAKEND(u32 scope)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",%d","BREAKEND",scope);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}


s32 RIL_HTTP_SetServer_TIMEOUT(u32 time)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+HTTPPARA=\"%s\",%d","TIMEOUT",time);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	return ret;
}






s32 RIL_HTTP_Request(Enum_HTTP_Action action_req)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	
	sprintf(strAT, "AT+HTTPACTION=%d",action_req);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_HTTP_Handler,NULL,15000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	return ret;
}




s32 RIL_HTTP_ReadResponse(u32 start_address,u32 byte_size)
{
	s32 ret = RIL_AT_SUCCESS;	
	char strAT[80];

	if(0 != byte_size)
	{
		memset(strAT,0,sizeof(strAT));
		sprintf(strAT, "AT+HTTPREAD=%d,%d",start_address,byte_size);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,5000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}else{
		memset(strAT,0,sizeof(strAT));
		sprintf(strAT, "AT+HTTPREAD");
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,5000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	return ret;
}



s32 RIL_HTTP_DownloadFile(u32 byte_size,u32 timeout)
{
	s32 ret = RIL_AT_SUCCESS;
		
	char strAT[80];

	if(byte_size < 100 || (timeout < 1000 || timeout > 12000))
		return RIL_AT_FAILED;
	
	memset(strAT,0,sizeof(strAT));
	
	sprintf(strAT, "AT+HTTPDATA=%d,%d",byte_size,timeout);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,7000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	return ret;
}



