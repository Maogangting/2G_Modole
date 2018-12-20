
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_ftp.c 
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
//#include "custom_feature_def.h"
#include "Ol_type.h"
#include "stdlib.h"
#include "ol_error.h"
//#include "ql_common.h"
#include "ol_system.h"
#include "Ol_uart.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_ftp.h"
#include "ril_network.h"
#include "ol_interface.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ol_iostream.h"


#define RIL_FTP_DEBUG_ENABLE 0
#if RIL_FTP_DEBUG_ENABLE > 0
#define RIL_FTP_DEBUG_PORT  UART_PORT1
static char DBG_Buffer[100];
#define RIL_FTP_DEBUG(BUF,...)  OL_TRACE_LOG(RIL_FTP_DEBUG_PORT,BUF,100,__VA_ARGS__)
#else
#define RIL_FTP_DEBUG(BUF,...) 
#endif


FTP_LOAD_T ftp_load_t;



s32 Callback_null(char*  ptr, s32 len, void* param)
{
	
}




s32 RIL_FTP_FTPOPEN(u8* hostName, u32 port,u8* userName,u8* password, ol_bool mode,u8* type) 
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[80];
	if(NULL != hostName)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPSERV=\"%s\"", hostName);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	if(port)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPPORT=%d", port);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	
	if(NULL != userName)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPUN=\"%s\"", userName);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);

		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	if(NULL != password)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPPW=\"%s\"", password);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPMODE=%d", mode);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}

	if(NULL != type)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPTYPE=\"%s\"", type);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	return ret;
}


s32 RIL_FTP_ACTIVATE(u8 * apn)
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
s32 Callback_ATFTPQUITResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTP:"); //continue wait
    if(head)
    {
		head = ol_RIL_FindString_ext(head,strlen(head),"+FTP:");
		if(head)
		{
			char * strTmp = NULL;
	        char* p1 = NULL;
	        char* p2 = NULL;
			s32 n = 0;
			strTmp = userdata;
	        memset(strTmp, 0x0, sizeof(strTmp));
	        p1 = strstr(head, "+");
	        p2 = strstr(p1,"\r\n");
	        if (p1 && p2)
	        {
	            memcpy(strTmp, p1, p2 - p1);
				ol_trace("strTmp= %s\r\n",strTmp);
				ol_trace("get buffer---%s\r\n",userdata);
	        }
			return	RIL_ATRSP_SUCCESS;
		}
		
		head = ol_RIL_FindString_ext(head,strlen(head),"+FTPGET:");
		if(head)
		{
			char * strTmp = NULL;
	        char* p1 = NULL;
	        char* p2 = NULL;
			s32 n = 0;
			strTmp = userdata;
	        memset(strTmp, 0x0, sizeof(strTmp));
	        p1 = strstr(head, "+");
	        p2 = strstr(p1,"\r\n");
	        if (p1 && p2)
	        {
	            memcpy(strTmp, p1, p2 - p1);
				ol_trace("strTmp= %s\r\n",strTmp);
				ol_trace("get buffer---%s\r\n",userdata);
	        }
			return	RIL_ATRSP_SUCCESS;
		}
		head = ol_RIL_FindString_ext(head,strlen(head),"+FTPPUT:");	
		if(head)
		{
			char * strTmp = NULL;
	        char* p1 = NULL;
	        char* p2 = NULL;
			s32 n = 0;
			strTmp = userdata;
	        memset(strTmp, 0x0, sizeof(strTmp));
	        p1 = strstr(head, "+");
	        p2 = strstr(p1,"\r\n");
	        if (p1 && p2)
	        {
	            memcpy(strTmp, p1, p2 - p1);
				ol_trace("strTmp= %s\r\n",strTmp);
				ol_trace("get buffer---%s\r\n",userdata);
	        }
			return	RIL_ATRSP_SUCCESS;
		}
		return RIL_ATRSP_FAILED;
	}
	 head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
  	 if(head)
  	 {  
	   ol_trace("FTPQUIT---SUCCESS\r\n");
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
	return RIL_ATRSP_FAILED;
}


s32 RIL_FTP_FTPQUIT(void) 
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[80];
    char buffer[60]={0};
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPQUIT");

    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPQUITResponse,(void *)buffer,1000);
	if (RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
  
    return ret;
}



s32 Callback_ATFTPPUTResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPPUT:"); //continue wait
	ol_trace("put-----head= %s\r\n",head);
	if(head)
	{
		char strTmp[60]={0};
        char* p1 = NULL;
        char* p2 = NULL;
		char* p3 = NULL;
		s32 n = 0,m = 0;
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			p1 = strstr(strTmp,",");
			p3 = strstr(p1+1,",");
	//	ol_trace("p1----%s,p3-----%s\r\n",p1,p3);
			if(NULL == p3)
			{
				for(n=1;n<strlen(p1);n++)
				{
					m = m*10 +(p1[n]-'0');

				}
			}else
			{
				for(n=1;n<strlen(p3);n++)
				{
					m = m*10 +(p3[n]-'0');

				}
			}
			ol_trace("m = %d,strTmp= %s\r\n",m,strTmp);			
        }
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
	   ol_trace("FTPPUT---SUCCESS\r\n");
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		 ol_trace("FTPPUT---ERROR\r\n");
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}

s32 RIL_FTPPUTFILE(s32 size,u8 * file_data)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	s32 result;
	memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPPUT=%d,%d,\"%s\"",2,size,file_data);
	//ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPPUTResponse,NULL,4000);
	if (RIL_AT_SUCCESS != ret)
	{
    	return ret;
	}
	
	return ret;
}


s32 RIL_FTP_FTPPUT(u8* fileName, u8* putopt,u8* filepath,u32 timeOut)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPPUTOPT=\"%s\"",putopt);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,timeOut);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPPUTNAME=\"%s\"",fileName);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,timeOut);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPPUTPATH=\"%s\"",filepath);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,timeOut);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPPUT=%d",1);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPPUTResponse,NULL,9000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
	return ret;
}




s32 Callback_ATFTPGETResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPGET:"); //continue wait
	if(head)
	{
		char strTmp[40]={0};
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0,m = 0;
		
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			p1 = strstr(strTmp,",");
			p2 = strstr(strTmp,":");
			for(n=1;n<strlen(p1);n++)
			{
				m = m*10 +(p1[n]-'0');
			}
			ftp_load_t.index = Ol_atoi(p2+1);
			ftp_load_t.ftp_ret = m;
			ol_trace("index--= %d,ftp_ret= %d,strTmp= %s\r\n",ftp_load_t.index,ftp_load_t.ftp_ret,strTmp);
		}
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
   	   
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}

s32 Callback_ATFTPGETDATAResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPGET:"); //continue wait

	if(head)
	{
		char strTmp[40]={0};
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0,m = 0;
		
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head,"+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			p1 = strstr(strTmp,",");
			p2 = strstr(strTmp,":");
			for(n=1;n<strlen(p1);n++)
			{
				m = m*10 +(p1[n]-'0');
			}
			ftp_load_t.index = Ol_atoi(p2+1);
			ftp_load_t.ftp_ret = m;
			ol_trace("Callback_ATFTPGETDATAResponse index--= %d,ftp_ret= %d,strTmp= %s\r\n",ftp_load_t.index,ftp_load_t.ftp_ret,strTmp);
		}
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
   	   
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}

s32 RIL_FTP_FTPGETDATA(s32 size,ol_bool enable)
{
	s32 ret = RIL_AT_SUCCESS;
    char strAT[80];
	char buffer[60]={0};
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPGET=2,%d,%d",size,enable);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPGETDATAResponse,(void *)buffer,4000);
	if (RIL_AT_SUCCESS != ret)
	{
    	return ret;
	}
	return ret;
}


s32 RIL_FTP_FTPGET(u8* fileName,u8* filepath, u32 filerest)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	char buffer[60]={0};
	if(NULL != fileName)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETNAME=\"%s\"",fileName);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	if(NULL != filepath)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETPATH=\"%s\"",filepath);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPREST=%d",filerest);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}

	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+FTPGET=%d",1);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPGETResponse,(void *)buffer,15000);
	if (RIL_AT_SUCCESS != ret)
	{
		return ret;
	}
		
	return ret;
}


s32 RIL_FTP_FTPGETPATH(u8* pathName) 
{
    s32 ret = RIL_AT_SUCCESS;
    
    char strAT[80];
    
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPGETPATH=\"%s\"",pathName);
   
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
    
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
 
    return ret;
}

s32 RIL_FTP_FTPPUTPATH(u8* pathName) 
{
    s32 ret = RIL_AT_SUCCESS;
    
    char strAT[80];
    
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPPUTPATH=\"%s\"",pathName);
   
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
    
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
 
    return ret;
}

s32 Callback_ATFTPSIZEResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPSIZE:"); //continue wait
	if(head)
	{
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0;
		strTmp = userdata;
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			ol_trace("strTmp= %s\r\n",strTmp);
			ol_trace("get buffer---%s\r\n",userdata);
        }
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
   	   
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}


 

s32 RIL_FTP_FTPSIZE(u8* fileName,u8* filepath)
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[80];
	char buffer[60]={0};
	if(NULL != fileName)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETNAME=\"%s\"",fileName);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	if(NULL != filepath)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETPATH=\"%s\"",filepath);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	
	
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPSIZE");
 
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPSIZEResponse,(void *)buffer,1000);
   
    if(RIL_AT_SUCCESS != ret)
    {  
        return ret;
    } 
    return ret;
}


s32 Callback_ATFTPDELEResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPDELE:"); //continue wait
	if(head)
	{
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0;
		strTmp = userdata;
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			ol_trace("strTmp= %s\r\n",strTmp);
			ol_trace("get buffer---%s\r\n",userdata);
        }
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
   	   
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}


s32 RIL_FTP_FTPDELETE(u8* fileName,u8* filepath) 
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[80];
	char buffer[60]={0};
	if(NULL != fileName)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETNAME=\"%s\"",fileName);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}

	if(NULL != filepath)
	{
		memset(strAT, 0, sizeof(strAT));
		sprintf(strAT, "AT+FTPGETPATH=\"%s\"",filepath);
		ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,NULL,1000);
		if (RIL_AT_SUCCESS != ret)
		{
			return ret;
		}
	}
	
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPDELE");
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPDELEResponse,(void *)buffer,2000);
 
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
    return ret;
}


s32 Callback_ATFTPMKDResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+FTPMKD:"); //continue wait
	if(head)
	{
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0;
		strTmp = userdata;
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			ol_trace("strTmp= %s\r\n",strTmp);
			ol_trace("get buffer---%s\r\n",userdata);
        }
		return	RIL_ATRSP_CONTINUE;
	}

   head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
   if(head)
   {  
   	   
	   return  RIL_ATRSP_SUCCESS;
   }

	head = ol_RIL_FindLine_ext(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return	RIL_ATRSP_FAILED;
	} 

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_FAILED;
}


s32 RIL_FTP_FTPMKDIR(u8* pathName)
{
    s32 ret = RIL_AT_SUCCESS;
 
    char strAT[80];
    char buffer[60]={0};

	memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPGETPATH=\"%s\"",pathName);
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,(void *)buffer,1000);
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
	
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPMKD");
 
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPMKDResponse,(void *)buffer,2000);
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
  
    return ret;
}


 s32 Callback_ATFTPRMDResponse(char*  line, s32 len, void* userdata)
 {
	 char *head = ol_RIL_FindString_ext(line, len, "+FTPRMD:"); //continue wait
	 if(head)
	 {
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0;
		strTmp = userdata;
        memset(strTmp, 0x0, sizeof(strTmp));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			ol_trace("strTmp= %s\r\n",strTmp);
			ol_trace("get buffer---%s\r\n",userdata);
        }
		return	RIL_ATRSP_CONTINUE;
	}
 
	head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
	if(head)
	{  
		
		return	RIL_ATRSP_SUCCESS;
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
	 return RIL_ATRSP_FAILED;
 }


 s32 RIL_FTP_FTPRMDIR(u8* pathName)
 {
    s32 ret = RIL_AT_SUCCESS;
    char strAT[80];
    char buffer[60]={0};

	memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPGETPATH=\"%s\"",pathName);
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_null,(void *)buffer,1000);
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
	
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+FTPRMD");
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATFTPRMDResponse,(void *)buffer,2000);
    if(RIL_AT_SUCCESS != ret)
    {
        return ret;
    }
    return ret;
}





