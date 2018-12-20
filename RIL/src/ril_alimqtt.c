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

extern char fg_mqtt_state;
extern s32 URC_RCV_TASK_ID;

static mqtt_re_connect_times = 0;

char *ConStrDeal(char *src)		// mgt 临时
{
	int i = 0;
	if (src == NULL) return NULL;
	for (; src[i] != '\0'; i++) {
		if (src[i] > '9') break;
	}
	return &src[i];
}

s32 callback_MqttDeverb(char* line, s32 len, void* userdata)
{
	ol_trace("mgt-###callback_MqttDeverb : [%s]\r\n", line);
}

s32 callback_MqttConnect(char* line, s32 len, void* userdata)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	char *line2 = ConStrDeal(line);

	ol_trace("mgt-###callback_Mqttconnect : [%s]- [%s]\r\n", line, line2);
	if (strncmp(line2,"OK", 2) == 0) {
		ol_trace("mgt-mqtt_connect:strcmp(line,'OK') == 0\r\n");
		mqtt_re_connect_times = 0;
		RIL_AL_MQTT_State();//以连接上查询Mqtt状态即可
	}else if (strncmp(line2,"ERROR", 5) == 0) {
		ol_trace("mgt-mqtt_connect:strcmp(line,'ERROR') == 0\r\n");
		mqtt_re_connect_times = 0;
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));	// 发送消息去进行重新连接	
	}else {
		//增加回调时间重新发送连接命令
		if (mqtt_re_connect_times < 4) {
			mqtt_re_connect_times ++;
			ol_trace("mgt-mqtt_connect: timeout times >> [%d]\r\n", mqtt_re_connect_times + 1);
			memset(strAT,0,sizeof(strAT));
			sprintf(strAT,"AT+IMQTTCONN");
			ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttConnect,NULL,10000);
		}else {
			ol_trace("mgt-mqtt_connect: timeout 4 times! send URC_ALIRECONNECT_MESAGE_IND\r\n");
			mqtt_re_connect_times = 0;
			ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));	// 发送消息去进行重新连接	
		}
	}
}
s32 callback_MqttState(char* line, s32 len, void* userdata)
{
	ol_trace("mgt-&&&callback_MqttState :  [%s]\r\n", line);
	if (strstr(line, "IMQTTSTATE:0") != NULL) {
		fg_mqtt_state = 0;
		ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));
	}else {
		fg_mqtt_state = 1;
	}
}
//________________________________________________________________________________________________________
s32 callback_MqttStateEx(char* line, s32 len, void* userdata)
{
	ol_trace("mgt-&&&callback_MqttState :  [%s]\r\n", line);
	if (strstr(line, "IMQTTSTATE:0") != NULL) {
		fg_mqtt_state = 0;
		//ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));
	}else {
		fg_mqtt_state = 1;
	}
}
s32 Mqtt_State_Check(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTSTATE?");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttStateEx,NULL,200);
    return ret;
}

s32 callback_MqttSub(char* line, s32 len, void* userdata)
{
	ol_trace("mgt-###callback_MqttSub : [%s]\r\n", line);
}

s32 callback_Null1(char* line, s32 len, void* userdata)
{
	ol_trace("just for test \r\n");
}


s32 RIL_AL_MQTT_DeVerb(u8 *ProductKey,u8 * DeviceName,u8 * DeviceSecret)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTAUTH=\"%s\",\"%s\",\"%s\"",ProductKey,DeviceName,DeviceSecret);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttDeverb,NULL,3000);
    return ret;
}

s32  RIL_AL_MQTT_SetParam(u8 * ParaTag,Mqtt_Info *ParaValue)
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
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,1000);
    return ret;
}

s32  RIL_AL_MQTT_Connect()
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTCONN");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttConnect,NULL,8000);
    return ret;
}


s32 RIL_AL_MQTT_Pub(u8* topic,u8 qos,u32* message)
{
	s32 ret = RIL_AT_SUCCESS;
	
	char strAT[1500];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTPUB=\"%s\",%d,\"%s\"",topic,qos,message);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,2000);
    return ret;
}



s32 RIL_AL_MQTT_Sub(u8 * topic,s8 qos)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTSUB=\"%s\",%d",topic,qos);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttSub,NULL,2000);
    return ret;
}



s32 RIL_AL_MQTT_UnSub(u8 * topic)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];

	memset(strAT,0,sizeof(strAT));

	sprintf(strAT,"AT+IMQTTUNSUB=\"%s\"",topic);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,1000);
    return ret;
}
s32 RIL_AL_MQTT_State(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTSTATE?");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_MqttState,NULL,200);
    return ret;
}
s32 RIL_AL_MQTT_Disconn(void)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[80];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTDISCONN");
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,4000);
    return ret;
}
s32 RIL_AL_MQTT_MsgPingRsp(u8* PINGRSP ,u8 mode)
{
	s32 ret = RIL_AT_SUCCESS;
	char strAT[256];
	memset(strAT,0,sizeof(strAT));
	sprintf(strAT,"AT+IMQTTDBGPARA=\"PINGRSP\",%d",mode);
	ret = ol_RIL_SendATCmd(strAT,strlen(strAT),callback_Null1,NULL,2000);
    return ret;
}
