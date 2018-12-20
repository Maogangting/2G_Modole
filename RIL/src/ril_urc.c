
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_urc.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module handles URC in RIL.
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
#include <stdlib.h>
#include <string.h>
#include "ril.h "
#include "ril_util.h"
//#include "ril_sms.h"
#include "ril_telephony.h "
#include "ol_ril.h"
//#include "ql_power.h"
//#include "ql_system.h"
#include "ril_audio.h"
#include "ril_ftp.h"
#include "ril_http.h"
#include "ol_system.h"
#include "ril_urc.h"
#include "ol_iostream.h"
#include "ol_interface.h"
#include "ril_location.h"


/************************************************************************/
/* Definition for URC receive task id.                                  */
/************************************************************************/
s32 URC_RCV_TASK_ID;  



/************************************************************************/
/* Declarations for URC handler.                                        */
/************************************************************************/
static void OnURCHandler_Call(const char* strURC, void* reserved);
static void OnURCHandler_SMS(const char* strURC, void* reserved);
static void OnURCHandler_Network(const char* strURC, void* reserved);
static void OnURCHandler_SIM(const char* strURC, void* reserved);
static void OnURCHandler_FTP(const char* strURC, void* reserved);
static void OnURCHandler_AudPlayInd(const char* strURC, void* reserved);

// DTMF URC callback
extern void OnURCHandler_DTMF( const char* strURC, void* reserved );


extern void OnURCHandler_GPSCMD(const char* strURC, void* reserved);

extern void OnURCHandler_MQTTCMD(const char* strURC, void* reserved);
extern void OnURCHandler_ALIMQTT( const char* strURC,  void* reserved);
extern void OnURCHandler_ALIAUTH( const char* strURC,  void* reserved);

/************************************************************************/
/* Customer ATC URC callback                                          */
/************************************************************************/
OL_ST_MSG ol_st_msg;

ST_LocInfo urc_location;



Urc_Network_T urc_net_t;
Urc_Ftp_T urc_ftp_t;
Urc_Zaudrec_T urc_zaudrec_t;
Urc_Sms_T urc_sms_t;
Urc_Mqtt_T_1 urc_mqtt_t;
Urc_ALIMqtt_T urc_alimqtt_t;
Urc_ALIAuth_T urc_aliauth_t;



/******************************************************************************
* Definitions for URCs and the handler.
* -------------------------------------
* -------------------------------------
* In OpenCPU RIL, URC contains two types: system URC and AT URC.
*   - System URCs indicate the various status of module.
*   - AT URC serves some specific AT command. 
*     For example, some AT command responses as below:
*         AT+QABC     (send at command)
*
*         OK          (response1)
*         +QABC:xxx   (response2) --> this is the final result which is reported by URC.
*     When calling Ql_RIL_SendATCmd() to send such AT command, the return value of 
*     Ql_RIL_SendATCmd indicates the response1, and the response2 may be reported
*     via the callback function. Especially for some AT commands that the time span
*     between response1 and response2 is very long, such as AT+QHTTPDL, AT+QFTPGET.
******************************************************************************/
/****************************************************/
/* Definitions for system URCs and the handler      */
/****************************************************/
const static ST_URC_HDLENTRY m_SysURCHdlEntry[] = {

    //Telephony unsolicited response
    {"+CRING: VOICE",                         OnURCHandler_Call},
    {"RING",                                  OnURCHandler_Call},
    {"BUSY",                                  OnURCHandler_Call},
    {"NO ANSWER",                             OnURCHandler_Call},
    {"NO CARRIER",                            OnURCHandler_Call},
    {"NO DIALTONE",                       	  OnURCHandler_Call},
    {"+CLIP:",                                OnURCHandler_Call},

    //SMS unsolicited response
    {"+CMTI:",                                OnURCHandler_SMS},

    //Network status unsolicited response
    {"+CREG:",                                OnURCHandler_Network},
    {"+CGREG:",                               OnURCHandler_Network},

    //SIM card unsolicited response
    {"+CPIN:",                                OnURCHandler_SIM},                       

	// Location indication
};

/****************************************************/
/* Definitions for AT URCs and the handler          */
/****************************************************/
const static ST_URC_HDLENTRY m_AtURCHdlEntry[] = {
    //FTP unsolicited response
    {"+FTPGET:",                             OnURCHandler_FTP},
    {"+FTPPUT:",                             OnURCHandler_FTP},

    //Audio (file or resource) playing indication  ��ʱδ����
    {"+ZAUDREC:",                            OnURCHandler_AudPlayInd},

    // DTMF unsolicited response
    {"+DTMF:",                              OnURCHandler_DTMF},
	//GNSS unsolicited response
	{"+GTPOS:",							  	  OnURCHandler_GPSCMD},
	//MQTT unsolicited response
	{"+MSUB:",								  OnURCHandler_MQTTCMD},
    {"+CONTEXT:",    						  OnURCHandler_MQTTCMD},
    {"+IMQTTRCVPUB",						OnURCHandler_ALIMQTT},
    {"+IMQTTAUTH",						OnURCHandler_ALIAUTH},
};
static void OnURCHandler_ALIAUTH( const char* strURC, void* reserved )
	{ 
		u8 * p1 = NULL;
		u8 * p2 = NULL;
		u32 index = 0;
		if(ol_StrPrefixMatch(strURC, "+IMQTTAUTH:OK"))
		{ol_trace("authAAAOK\r\n");
		urc_aliauth_t.status=OL_TRUE;
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIMUTH_MESSAGE_IND, (u32)(&urc_aliauth_t));
		return ;
		}
		else
		{ol_trace("authAAAERROR\r\n");
		urc_aliauth_t.status=OL_FALSE;
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIMUTH_MESSAGE_IND, (u32)(&urc_aliauth_t));
		return ;
		}
	}
static void OnURCHandler_ALIMQTT( const char* strURC, void* reserved )
	{  
		char* p1 = NULL;
		char* p2 = NULL;
		char* p3 = NULL;
		char* p4 = NULL;
		char* p5 = NULL;
		char* p6 = NULL;	
		s32 ret_num = 0;
	
		if(ol_StrPrefixMatch(strURC, "+IMQTTRCVPUB:"))
		{
			s32 state = 0;
			s32 ret_code = 0;
			s32 size = 0;
			memset(&urc_alimqtt_t,0,sizeof(Urc_ALIMqtt_T));
			p1 = strstr(strURC,":");
			p2 = strstr(p1,",");
			memcpy(urc_alimqtt_t.packet_id,p1+1,p2-p1-1);
			p3 = strstr(p2,"/");
			p4 = strstr(p3,",");
			memcpy(urc_alimqtt_t.topic,p3,p4-p3-1);
			p5 = strstr(p4+1,",");
			p6 = strstr(p5,"\r\n");
			memcpy(urc_alimqtt_t.message,p5+2,p6-p5-3);
			//Ol_sscanf(p1+1,"%d,%s,%d,%[^\r\n]",&urc_http_t.req_state,&urc_http_t.ret_code,&urc_http_t.size);
			ol_trace("OnURCHandler_MQTT----%s,%s,%s\r\n",urc_alimqtt_t.packet_id,urc_alimqtt_t.topic,urc_alimqtt_t.message);
			ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIMQTT_MESSAGE_IND, (u32)(&urc_alimqtt_t));
			return ;
		}
		return ;
	}

static void OnURCHandler_MQTTCMD( const char* strURC, void* reserved )
{

	u8 * p1 = NULL;
	u8 * p2 = NULL;
	u32 index = 0;

	if(ol_StrPrefixMatch(strURC,"+MSUB:"))
	{
		if(ol_StrPrefixMatch(strURC,"+MSUB: "))
		{
			p1 = strstr(strURC,":");
			index = Ol_atoi(p1+1);
			ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_MQTT_NUM_IND, index);
			return ;
		}else 
		{	
			Ol_memset(&urc_mqtt_t,0,sizeof(Urc_Mqtt_T_1));
			
			p1 = strstr(strURC,":");
			p2 = strstr(p1+1,",");
			
			memcpy(urc_mqtt_t.topic,p1+1,p2-p1-1);
			urc_mqtt_t.count = Ol_atoi(p2+1);			
		}
	}

	if(ol_StrPrefixMatch(strURC,"+CONTEXT: "))
	{
		memcpy(urc_mqtt_t.message,strURC+10,urc_mqtt_t.count);	
		
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_MQTT_MESSAGE_IND, (u32)(&urc_mqtt_t));
	}

	return ;

}

static void OnURCHandler_DTMF( const char* strURC, void* reserved )
{  
	char* p1 = NULL;
	s32 ret_num = 0;

	if(ol_StrPrefixMatch(strURC, "+DTMF:"))
	{
		
		p1 = strstr(strURC,":");
		ret_num = Ol_atoi(p1+1);
		ol_trace("OnURCHandler_DTMF--ret_num= %d\r\n",ret_num);
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_DTMF_STATE_IND, ret_num);
	
		return ;
	}
	return ;
}

static void OnURCHandler_GPSCMD( const char* strURC, void* reserved )
{
	if(ol_StrPrefixMatch(strURC, "+GTPOS:"))
	{
		char strTmp[80]={0};
		char* p1 = NULL;
		char* p2 = NULL;
		s32 i = 0,state = 0;
		p1 = strstr(strURC,":");
		for(i =0 ;i < 4;i++)
		{
			if(p1[i] == '-')
			{
				state = '0' - p1[i+1]; //Enum_Gtpos_State
				ol_trace("state = %d\r\n",state);
				return ;
			}
		}

		memset(&urc_location,0,sizeof(ST_LocInfo));
		
		p1 = strstr(strURC,":");
		p2 = strstr(p1,",");
		memcpy(urc_location.latitude,p1+1,p2-p1-1);
		p1 = strstr(p2+1,",");
		memcpy(urc_location.longitude,p2+1,p1-p2-1);
		
		ol_trace("111 urc_location.latitude= %s,urc_location.longitude= %s\r\n",urc_location.latitude,urc_location.longitude);	
		
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_GTPOS_STATE_IND, (u32)(&urc_location));
		return ;
	}	
	return ;
}


static void OnURCHandler_SIM(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
    char strTmp[40];
    s32 len,cpinStat;
    extern s32 RIL_SIM_GetSimStateByName(char* simStat, u32 len);

    memset(strTmp, 0x0, sizeof(strTmp));
    len = sprintf(strTmp, "+CPIN: ");
 	if (ol_StrPrefixMatch(strURC, strTmp))
    {
		Ol_sscanf(strURC,"%*[^ ]%s%[^\r\n]",strTmp);      	 
		cpinStat = (u32)RIL_SIM_GetSimStateByName(strTmp, strlen(strTmp));
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_SIM_CARD_STATE_IND, cpinStat);     
		return ;
	}
	return ;
}
static void OnURCHandler_Network(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
    char strTmp[10];

	memset(&urc_net_t,0,sizeof(Urc_Network_T));
    if (ol_StrPrefixMatch(strURC, "+CREG: "))
    {
  
       Ol_sscanf(strURC,"%*[^ ]%d,%d,%[^\r\n]",&urc_net_t.index,&urc_net_t.state);
       ol_trace("+CREG--n = %d,state= %d\r\n",urc_net_t.index,urc_net_t.state);
       ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_GPRS_NW_STATE_IND, (u32)(&urc_net_t));
    }
    else if (ol_StrPrefixMatch(strURC, "+CGREG: "))
    {
       Ol_sscanf(strURC,"%*[^ ]%d,%d,%[^\r\n]",&urc_net_t.index,&urc_net_t.state);
       ol_trace("+CGREG--n = %d,state= %d\r\n",urc_net_t.index,urc_net_t.state);
       ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_GPRS_NW_STATE_IND, (u32)(&urc_net_t));
    }
	return ;
}


#if 0
ST_ComingCallInfo  ol_g_comingCall;

static void OnURCHandler_Call(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
    char strTmp[10];
    if (ol_StrPrefixMatch(strURC, "RING") || 
        ol_StrPrefixMatch(strURC, "+CLIP:") ||
        ol_StrPrefixMatch(strURC, "+CRING: VOICE"))
    {// Coming call
       
        u16 len;

        p1 = strstr(strURC, "+CLIP:");
        if (!p1)
        {
            return;
        }

        ol_g_comingCall.ringCnt++;
        if ((ol_g_comingCall.ringCnt / 6) > 0)
        {
            ol_g_comingCall.ringCnt %= 6;
        }

        // Retrieve phone number
        p1 += strlen("+CLIP:");
        p2 = strstr(p1 + 1, ",");
        len = p2 - (p1 + 2) - 1;
        memcpy(ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt].phoneNumber, p1 + 2, len);
        ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt].phoneNumber[len] = '\0';

		ol_trace("phoneNumber= %s\r\n",ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt].phoneNumber);
        // Retrieve number type
        p1 = p2;
        p2 = strstr(p1 + 1, ",");
        memset(strTmp, 0x0, sizeof(strTmp));
        memcpy(strTmp, p1 + 1, p2 - p1 -1);
        ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt].type = Ol_atoi(strTmp);
		ol_trace("type= %d\r\n",ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt].type);

//		ol_Sleep(3000);		
//		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_COMING_CALL_IND, (u32)(&(ol_g_comingCall.comingCall[ol_g_comingCall.ringCnt])));	
	}
    else if (ol_StrPrefixMatch(strURC, "BUSY")         ||
             ol_StrPrefixMatch(strURC, "NO ANSWER")    ||
             ol_StrPrefixMatch(strURC, "NO CARRIER")   ||
             ol_StrPrefixMatch(strURC, "NO DIALTONE"))
    {
        u32 callStat;

        if (ol_StrPrefixMatch(strURC, "BUSY"))
        {
            callStat = CALL_STATE_BUSY;
        }
        else if (ol_StrPrefixMatch(strURC, "NO ANSWER"))
        {
            callStat = CALL_STATE_NO_ANSWER;
        }
        else if (ol_StrPrefixMatch(strURC, "NO CARRIER"))
        {
            callStat = CALL_STATE_NO_CARRIER;
        }
        else if (ol_StrPrefixMatch(strURC, "NO DIALTONE"))
        {
            callStat = CALL_STATE_NO_DIALTONE;
        }else{
            return;
        }
		ol_trace("OnURCHandler_Call callstat--- %d\r\n",callStat);
      ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_CALL_STATE_IND, callStat);
    }
}

#endif

static void OnURCHandler_Call(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
    char strTmp[10];
    if (ol_StrPrefixMatch(strURC, "RING") || 
        ol_StrPrefixMatch(strURC, "+CLIP:") ||
        ol_StrPrefixMatch(strURC, "+CRING: VOICE"))
    {// Coming call
        extern ST_ComingCallInfo  g_comingCall;
        u16 len;

        p1 = strstr(strURC, "+CLIP:");
        if (!p1)
        {
            return;
        }

        g_comingCall.ringCnt++;
        if ((g_comingCall.ringCnt / 6) > 0)
        {
            g_comingCall.ringCnt %= 6;
        }

        // Retrieve phone number
        p1 += strlen("+CLIP:");
        p2 = strstr(p1 + 1, ",");
        len = p2 - (p1 + 2) - 1;
        memcpy(g_comingCall.comingCall[g_comingCall.ringCnt].phoneNumber, p1 + 2, len);
        g_comingCall.comingCall[g_comingCall.ringCnt].phoneNumber[len] = '\0';

		ol_trace("phoneNumber= %s\r\n",g_comingCall.comingCall[g_comingCall.ringCnt].phoneNumber);
        // Retrieve number type
        p1 = p2;
        p2 = strstr(p1 + 1, ",");
        memset(strTmp, 0x0, sizeof(strTmp));
        memcpy(strTmp, p1 + 1, p2 - p1 -1);
        g_comingCall.comingCall[g_comingCall.ringCnt].type = Ol_atoi(strTmp);
		ol_trace("type= %d\r\n",g_comingCall.comingCall[g_comingCall.ringCnt].type);

		ol_Sleep(3000);		
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_COMING_CALL_IND, (u32)(&(g_comingCall.comingCall[g_comingCall.ringCnt])));	
		return ;
	}
    else if (ol_StrPrefixMatch(strURC, "BUSY")         ||
             ol_StrPrefixMatch(strURC, "NO ANSWER")    ||
             ol_StrPrefixMatch(strURC, "NO CARRIER")   ||
             ol_StrPrefixMatch(strURC, "NO DIALTONE"))
    {
        u32 callStat;

        if (ol_StrPrefixMatch(strURC, "BUSY"))
        {
            callStat = CALL_STATE_BUSY;
        }
        else if (ol_StrPrefixMatch(strURC, "NO ANSWER"))
        {
            callStat = CALL_STATE_NO_ANSWER;
        }
        else if (ol_StrPrefixMatch(strURC, "NO CARRIER"))
        {
            callStat = CALL_STATE_NO_CARRIER;
        }
        else if (ol_StrPrefixMatch(strURC, "NO DIALTONE"))
        {
            callStat = CALL_STATE_NO_DIALTONE;
        }else{
            return;
        }
		ol_trace("OnURCHandler_Call callstat--- %d\r\n",callStat);
      ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_CALL_STATE_IND, callStat);
    }
	return ;
}



static void OnURCHandler_SMS(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;

    //TODO: Something wrong with long SMS
    if (ol_StrPrefixMatch(strURC, "+CMTI:")) 
    {
     //   u32 smsIndex;
    //    char mem[SMS_MEM_CHAR_LEN];

		
		memset(&urc_sms_t,0x0,sizeof(Urc_Sms_T));
        // Get 'mem'
        p1 = strstr(strURC, ":");
        p1 += 3;
        p2 = strstr(p1, ",");
        if (p1 && p2)
        {              
            strncpy(urc_sms_t.mem, p1, (p2 - p1 - 1));
        }
	
        // Get index
        urc_sms_t.index = Ol_atoi(p2+1);
	
       ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_NEW_SMS_IND, (u32)(&urc_sms_t));
        
    }
    else if (ol_StrPrefixMatch(strURC, "+CMT:"))
    {
    }
	return ;
}


static void OnURCHandler_FTP(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
	s32 n=0,m=0;
    char strTmp[20];

	memset(&urc_ftp_t,0,sizeof(Urc_Ftp_T));
	
    p1 = strstr(strURC, "+FTPGET:");
    p1 += strlen("+FTPGET:");
    p2 = strstr(p1, ",");
	if(p1 && p2)
	{
		urc_ftp_t.index = Ol_atoi(p1);
		urc_ftp_t.ftp_ret = Ol_atoi(p2+1);
		ol_trace("ftpget index= %d,ftp_ret= %d\r\n",urc_ftp_t.index,urc_ftp_t.ftp_ret);
	   
        ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_FTP_STATE_IND, (u32)(&urc_ftp_t));
		return;
	}

    p1 = NULL;
    p2 = NULL;
    p1 = strstr(strURC, "+FTPPUT:");
    p1 += strlen("+FTPPUT:");
    p2 = strstr(p1, ",");
    if (p1 && p2)
    {
        urc_ftp_t.index = Ol_atoi(p1);
		if(NULL == strstr(p2+1,","))
		{
			urc_ftp_t.ftp_ret = Ol_atoi(p2+1);
		}
		else 
		{
		
			p1 = strstr(p2+1,",");
			urc_ftp_t.ftp_ret = Ol_atoi(p1+1);
		}
		 ol_trace("ftpput index= %d,ftp_ret= %d\r\n",urc_ftp_t.index,urc_ftp_t.ftp_ret);
		 ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_FTP_STATE_IND, (u32)(&urc_ftp_t));
      	return;
    }
	return ;
}



static void OnURCHandler_Undefined(const char* strURC, void* reserved)
{
	ol_trace("not found urc msg \r\n");
   // ol_OS_SendMessage(URC_RCV_TASK_ID, 5, URC_END, 0);
}


static void OnURCHandler_AudPlayInd(const char* strURC, void* reserved)
{
    char* p1 = NULL;
    char* p2 = NULL;
    s32 oper = free_operation;
	s32 state = idle_state;
	s32 n=0,m=0;
    char strTmp[20];

	memset(&urc_zaudrec_t,0,sizeof(Urc_Zaudrec_T));
    p1 = strstr(strURC, "+ZAUDREC:");
    p1 += strlen("+ZAUDREC:");
    p2 = strstr(p1, ",");
	if(p1 && (NULL == strstr(p2+1,",")))
	{
		urc_zaudrec_t.oper = Ol_atoi(p1);   //Enum_Record_Operation
		urc_zaudrec_t.state = Ol_atoi(p2+1); // Enum_Record_State
		ol_trace("zaudrec oper= %d,state= %d\r\n",urc_zaudrec_t.oper,urc_zaudrec_t.state);
	   
        ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_AUDIO_STATE_IND, (u32)(&urc_zaudrec_t));
		return;
	} 
  return ;
}

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
void OnURCHandler(const char* strURC, void* reserved)
{
    s32 i;
    
    if (NULL == strURC)
    {
        return;
    }

    // For system URCs   ���
    for (i = 0; i < NUM_ELEMS(m_SysURCHdlEntry); i++)
    {
        if (strstr(strURC, m_SysURCHdlEntry[i].keyword))
        {
            m_SysURCHdlEntry[i].handler(strURC, reserved);
            return;
        }
    }

    // For AT URCs   ���
    for (i = 0; i < NUM_ELEMS(m_AtURCHdlEntry); i++)
    {
        if (strstr(strURC, m_AtURCHdlEntry[i].keyword))
        {
            m_AtURCHdlEntry[i].handler(strURC, reserved);
            return;
        }
    }

    // For undefined URCs
    OnURCHandler_Undefined(strURC, reserved);
}

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
s32 Ol_RIL_IsURCStr(const char* strRsp)
{
    s32 i;
	URC_RCV_TASK_ID = ol_OS_GetActiveTaskId();   //��ȡmodid
    for (i = 0; i < NUM_ELEMS(m_SysURCHdlEntry); i++)  //ϵͳ�ϱ���Ϣ
    {
        if (strstr(strRsp, m_SysURCHdlEntry[i].keyword)) 
        {
            return 1;
        }
    }
    for (i = 0; i < NUM_ELEMS(m_AtURCHdlEntry); i++) //AT���� �ϱ���Ϣ
    {
        if (strstr(strRsp, m_AtURCHdlEntry[i].keyword)) 
        {
            return 1;
        }
    }
    return 0;
}


