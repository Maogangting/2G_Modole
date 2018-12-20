/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_mqtt.c 
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
#include <ril_urc.h>
#include "ril_mqtt.h"

s32 callback_Null1(char* line, s32 len, void* userdata)
{
	ol_trace("just for test \r\n");
}


s32 RIL_MQTT_DeVerb(u8 *ProductKey,u8 * DeviceName,u8 * DeviceSecret)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTAUTH=\"%s\",\"%s\",\"%s\"",ProductKey,DeviceName,DeviceSecret);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,3000);
    return ret;
}

s32  RIL_MQTT_SetParam(u8 * ParaTag,Mqtt_Info *ParaValue)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	if(strcmp(ParaTag,"VERSION")==0)
	{
	sprintf(strAT,"AT+IMQTTPARA=\"%s\",\"%s\"",ParaTag,ParaValue->version);
	}
	else if(strcmp(ParaTag,"TIMEOUT")==0)
	{
	sprintf(strAT,"AT+IMQTTPARA=\"%s\",%d",ParaTag,ParaValue->time_out);	
	}
	else if(strcmp(ParaTag,"CLEAN")==0)
	{
	sprintf(strAT,"AT+IMQTTPARA=\"%s\",%d",ParaTag,ParaValue->clean);
	}
	else if(strcmp(ParaTag,"KEEPALIVE")==0)
	{
	sprintf(strAT,"AT+IMQTTPARA=\"%s\",%d",ParaTag,ParaValue->keepalive);
	}
	else
	{
		ol_trace("ParaTag:%s\r\n",ParaTag);
		ol_trace("ParaTag error\r\n");
	}
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,3000);
    return ret;
}

s32  RIL_MQTT_Connect(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTCONN");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,8000);
    return ret;
}


s32 RIL_MQTT_Pub(u8* topic,u8 qos,u8* message)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[1500];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTPUB=\"%s\",%d,\"%s\"",topic,qos,message);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,2000);
    return ret;
}



s32 RIL_MQTT_Sub(u8 * topic,s8 qos)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTSUB=\"%s\",%d",topic,qos);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,2000);
    return ret;
}



s32 RIL_MQTT_UnSub(u8 * topic)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTUNSUB=\"%s\"",topic);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,1000);
    return ret;
}
s32 RIL_MQTT_State(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTSTATE?");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,3000);
    return ret;
}
s32 RIL_MQTT_Disconn(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTDISCONN");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,8000);
    return ret;
}
s32 RIL_MQTT_MsgPingRsp(u8* PINGRSP ,u8 mode)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTDBGPARA=\"PINGRSP\",%d",mode);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,2000);
    return ret;
}
