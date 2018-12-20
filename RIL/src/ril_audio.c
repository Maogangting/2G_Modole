
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_audio.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The APIs are used to afford the audio related operations,based on RIL.
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
#include <string.h>
#include <stdlib.h>
#include "ol_type.h"
#include "ol_error.h"
//#include "ol_common.h"
//#include "ql_system.h"
#include "ol_system.h"
#include "ol_uart.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_audio.h"
#include "ol_iostream.h"
#include "ol_interface.h"



#define IS_SUPPORT_AUD_CHANNEL(audChannel)    \
(   \
    (   \
            (AUD_CHANNEL_NORMAL == (audChannel))    \
         || (AUD_CHANNEL_HEADSET == (audChannel))   \
         || (AUD_CHANNEL_LOUD_SPEAKER == (audChannel))    \
    ) ? TRUE : FALSE    \
)




static s32 ATResponse_AUD_handler(char* line, u32 len, void* userdata);

s32 RIL_AUD_SetVolume(u8 volLevel)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[60];
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+CLVL=%d",volLevel);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,NULL,1000);
	if(ret != RIL_AT_SUCCESS)
	{
		return ret;
	}
	return ret;
}
s32 RIL_AUD_SetChannel(Enum_AudChannel audChannel)
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[20];
 
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+ESAM=%d",(char)audChannel);
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,NULL,0);
    return ret;
    
}

/* sound_level <0-100>*/

s32 RIL_AUD_PlayFile(u8 sound_level,char* filePath)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[60];
	u8 buffer[100]={0};
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+CLVL=%d",sound_level);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,NULL,1000);
	if(ret != RIL_AT_SUCCESS)
	{
		return ret;
	}
	
	memset(strAT, 0, sizeof(strAT));
	sprintf(strAT, "AT+ZAUDREC=%d,\"%s\"",2,filePath);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
	return ret;
}

s32 RIL_AUD_StopPlay(void)
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[20];
    u8 buffer[100]={0};
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+ZAUDREC=%d",3);

    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
    return ret;
}



/*  
start record audio, "a.amr" or "a.wav",if you input "a",be equivalent to "a.wav"
 */
s32 RIL_AUD_StartRecord(char* fileName)
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[40];
    u8 buffer[100]={0};
    
	if(NULL != fileName)
    {
    	memset(strAT, 0, sizeof(strAT));
    	sprintf(strAT, "AT+ZAUDREC=0,\"%s\"",fileName);
    	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
	}else
	{
		memset(strAT, 0, sizeof(strAT));
    	sprintf(strAT, "AT+ZAUDREC=0");    //"REC.wav"
    	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
	}
    return ret;
}

s32 RIL_AUD_StopRecord(void)
{
    s32 ret = RIL_AT_SUCCESS;
    char strAT[20];
    u8 buffer[100]={0};
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+ZAUDREC=1");
    ret = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
    return ret;
}

s32 RIL_AUD_GetRecordFileInfo(u8 * fileinfo)
{
    s32 at_result = RIL_AT_SUCCESS;
    char strAT[20];
    u8 buffer[100]={0};
    
    memset(strAT, 0, sizeof(strAT));
    sprintf(strAT, "AT+ZAUDREC?");
    at_result = ol_RIL_SendATCmd(strAT,strlen(strAT),ATResponse_AUD_handler,buffer,1000);
    if(at_result == RIL_AT_SUCCESS)
    {
        memcpy(fileinfo,buffer,strlen(buffer));
		ol_trace("RIL_AUD_GetRecordFileInfo---%s\r\n",buffer);
    }
    return at_result;
}


static s32 ATResponse_AUD_handler(char* line, u32 len, void* userdata)
{
    char *head = NULL;

    if(head =ol_RIL_FindString_ext(line, len, "+ZAUDREC:"))
    {
		char * strTmp = NULL;
        char* p1 = NULL;
        char* p2 = NULL;
		strTmp = userdata;
        p1 = strstr(head, "+");
        p2 = strstr(p1,"\r\n");
        if (p1 && p2)
        {
            memcpy(strTmp, p1, p2 - p1);
			ol_trace("ATResponse_AUD_handler---strtmp= %s\r\n",strTmp);
        }
		return	RIL_ATRSP_SUCCESS;
	}
    
    head = ol_RIL_FindLine_ext(line, len, "OK");
    if(head)
    {  
    	ol_trace("ATResponse_AUD_handler --- OK\r\n");
        return  RIL_ATRSP_SUCCESS;  
    }
    head = ol_RIL_FindLine_ext(line, len, "ERROR");
    if(head)
    {  
    	ol_trace("ATResponse_AUD_handler---- ERROR\r\n");
        return  RIL_ATRSP_FAILED;
    }
    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        return  RIL_ATRSP_FAILED;
    }
    
    return RIL_ATRSP_CONTINUE; //continue wait
}



