
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
#include "ril_location.h"
#include "ril.h"
#include "ril_util.h"
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



ST_LocInfo st_location_info;



s32 Callback_ATGTPOSResponse(char*  line, s32 len, void* userdata)
{
	char *head = ol_RIL_FindString_ext(line, len, "+GTPOS:"); //continue wait
	ol_trace("----Callback_ATGTPOSResponse----: %s\r\n",line);
	if(head)
	{
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		s32 n = 0,m=0;

		memset(&st_location_info,0,sizeof(ST_LocInfo));
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
		strTmp = userdata;
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			p1 = strstr(strTmp,":");
			p2 = strstr(p1,",");
			if(p1 && p2)
				memcpy(st_location_info.latitude,p1+1,p2-p1);
			p1 = strstr(p2+1,",");
			if(p1 && p2)
				memcpy(st_location_info.longitude,p2+1,p1-p2);

			ol_trace("st_location_info.latitude= %s,st_location_info.longitude= %s\r\n",st_location_info.latitude,st_location_info.longitude);
        }
		return	RIL_ATRSP_SUCCESS;
	}
	head = ol_RIL_FindLine_ext(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
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

	head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
	if(head)
	{
		return	RIL_ATRSP_FAILED;
	}	
	return RIL_ATRSP_FAILED;
	
}

s32 RIL_GetLocation(void)
{
	s32 ret = RIL_AT_FAILED;
	char strAT[40]; 
    char buf[80]={0};
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+GTPOS");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATGTPOSResponse,buf,10000);
    return ret;
}

s32 RIL_GetLOcationByWifi(char * macCOMMAsignal) //format ---> "5c63bfd259d2,-75"
{
	s32 ret = RIL_AT_FAILED;
	char strAT[40];
	char buf[80]={0};

	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+GTPOS=1");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATGTPOSResponse,buf,10000);
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+GTPOS=3,\"%s\"",macCOMMAsignal);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),Callback_ATGTPOSResponse,buf,10000);
    return ret;
}




