/**********************************************************************
 *                Copyright MBLTEK(shanghai)co. Ltd.                  *
 *---------------------------------------------------------------------
 * FileName      :   app_main.c
 * version       :   1.1
 * Description   :   
 * Authors       :   mgt
 * Notes         :
 *---------------------------------------------------------------------
 *  					HISTORY OF CHANGES
 * 2018.11.27 ：整合218最新SDK代码，包括：
 * 1,导入206成熟定制文件 ril.h ril_urc.h ril_urc.c & main部分文件
 * 2,ril_urc.c文件中，删除#include "ril_sms.h"头文件
 * 3，添加ril_alimqtt.h & ril_alimqtt.c,并在同目录下的makefile文件中报备新增编译的ril_alimqtt.c文件
 * 4, 原UART2不能使用，改用ol_uart0，并且该口默认为at调试口，当作数据口需要进行额外配置，参考demo/uart
 * 
 * 问题：1, 烧录器中所有项目全部烧录过一次后，开机必死机，需要206的启动代码跑过后，再跑218代码就OK。疑惑？
 * 		2, 一段时间后，阿里后台会自动断掉问题。
 *
 *--------------------------------------------------------------------
 ********************************************************************/
#include <stdio.h>
#include <string.h>
#include "ol_type.h"
#include "ol_interface.h"
#include "ol_iostream.h"
//#include "ol_gps.h"
#include "ol_soc.h"
#include "ol_system.h"
#include "ol_ril.h"
#include "ol_timer.h"
#include "ol_type.h"
#include "ol_uart.h"
#include "ol_gpio.h"
#include "ol_wtd.h"
#include "ol_adc.h"
#include "ol_fs.h"
#include "ol_power.h"
#include "ril.h"
#include "ril_ftp.h"
#include "ril_telephony.h"
#include "ril_util.h"
#include "ril_sim.h"
#include "ril_network.h"
#include "ril_telephony.h"
#include "ril_urc.h"
#include "ril_location.h"
#include "ril_alimqtt.h"
#include "ol_gps.h"
//#include "esp_json_parser.h"
#include "cJSON.h"
#include "Mycom.h"	// 串口

//#define __DEBUG__

/********************************************************************
 * SiLan App
 ********************************************************************/
#define PRODUCT_KEY             "a1JoWeN3xAo"
#define DEVICE_NAME             "temp1"
#define DEVICE_SECRET          	"B2gHBQQZ5vaQjrhx27XSLPDpCPsZS8LT"

#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA              "/"PRODUCT_KEY"/"DEVICE_NAME"/data"
#define TOPIC_DATA_SERVER       "/"PRODUCT_KEY"/"DEVICE_NAME"/data_server"

#define MEM_BUF_SIZE		512

typedef enum {
    IOTX_MQTT_QOS0 = 0,
    IOTX_MQTT_QOS1,
    IOTX_MQTT_QOS2
} iotx_mqtt_qos_t;

typedef struct {
	char  status[2];
	char  Longitude[15];
	char  Latitude[15];
}GPS_STATE;
GPS_STATE GpsState = {"V", "", ""};

typedef struct virtual_pump {
    u8 status;
    u8 pressure;
    u8 qs_pressure;
    u8 err_code;
    u16 rtime;
    u16 power;
    u8 lowerlimit;
    u8 upperlimit;
    u8 superlimit;
    u8 step;
} pump_info_t;

enum {
	AUTH_INIT = 0,
	AUTH_OK = 1,
	AUTH_ERROR = 2
};

static pump_info_t pump_info = {0,0,0,0,0,0,0,0,0,0};
static u32 mutex_pump_info = 0;
u32 mutex_uart_buf = 0;
//static char status_received = 0;
static u8 pktID = 1;
static char systime[20] = "20181111111111";
static char fg_initOK = 0;
static char dst[20][20];
static char pub_hex[MEM_BUF_SIZE * 2];
char fg_mqtt_state = 0;

#define SEND_UART_TO_5832(cmd, value)	\
	do {				\
		data[0] = 0xff;	\
		data[1] = 0x55;	\
		data[2] = 0x02;	\
		data[3] = cmd;	\
		data[4] = value;\
		check_sum = 0;	\
		for(i = 2; i <= 4; i++) {	\
			check_sum = check_sum - data[i];	\
		}				\
		data[5] = check_sum;	\
		ol_UART_Write(OL_Port_0,data,6);	\
		ol_Sleep(100000);	\
		request();	\
	}while(0);

extern s32 URC_RCV_TASK_ID;
extern void OL_InitRegions(void);

void app_main(void *data);
void app_initial(void *data); //app inital 
void app_ext2_main(void *data);
void app_ext2_initial(void *data);
void app_ext3_main(void *data);
void app_ext3_initial(void *data);
void app_ext4_main(void *data);
void app_ext4_initial(void *data);

#pragma arm section rodata = "APP_CFG"
APP_ENTRY_FLAG 
#pragma arm section rodata

#pragma arm section rodata="APPENTRY"
	const EatEntry_st AppEntry = 
	{
		app_main,
		app_initial,    //ext1_func donit remove it!!!
		(app_user_func)OL_NULL,//soc_task, donit remove it!!!
		app_ext2_main,//app_user2,
		app_ext3_main,//app_user3,
		app_ext4_main,//app_user4,
		(app_user_func)OL_NULL,//app_user5, reserve
		(app_user_func)OL_NULL,//app_user6, reserve
		(app_user_func)OL_NULL,//app_user7, reserve
		(app_user_func)OL_NULL,//app_user8, reserve
		app_ext2_initial,
		app_ext3_initial,
		app_ext4_initial,
		OL_NULL,
		OL_NULL,
		OL_NULL
	};
#pragma arm section rodata

s32 callback_Null(char* line, s32 len, void* userdata)
{
	ol_trace("mgt-just for test \r\n");
}
s32 Callback_Null11(char*  ptr, s32 len, void* param)
{
	return 0;
}

static void request()
{
    u8 i = 0, check_sum = 0;
    u8 data[6];

    data[0] = 0xff;
    data[1] = 0x55;
    data[2] = 0x01;
    data[3] = 0x05;

    for(i = 2; i <= 3; i++) {
        check_sum = check_sum - data[i];
    }
    data[4] = check_sum;
	ol_UART_ClrTxBuffer(OL_Port_0);
    ol_UART_Write(OL_Port_0,data,5);
}

static void message_arrive(Urc_ALIMqtt_T *msg)
{
    u8 i, check_sum;
    //char CMD[20], devID[20];
    u8 Switch, pressure, qs_pressure, status, err_code, lowerlimit, upperlimit, superlimit, step;
    u16 rtime, power;
    cJSON *pJson = NULL;
	cJSON *pSub = NULL;
	char *CMD = NULL;
    u8 data[6];

    //ol_print("----------------- Message Arrived---------------------- \r\n");
    //ol_print("topic: %s   msg: %s \r\n", (char *)msg->topic, (char *)msg->message);
	
	pJson = cJSON_Parse(msg->message);
	////ol_print("systime : %s\r\n", cJSON_GetObjectItem(pJson, "systime")->valuestring);
	////ol_print("devId : %s\r\n", cJSON_GetObjectItem(pJson, "devId")->valuestring);
	////ol_print("pktid : %d\r\n", cJSON_GetObjectItem(pJson, "pktid")->valueint);

    pSub = cJSON_GetObjectItem(pJson, "param");
    CMD = cJSON_GetObjectItem(pSub, "cmd")->valuestring;
	ol_trace("mgt-mqtt receive CMD: %s \r\n", CMD);

    if( strcmp(CMD, "SWITCH") == 0){
		Switch = cJSON_GetObjectItem(pSub, "data")->valueint;
        if(Switch != pump_info.status){
			SEND_UART_TO_5832(0x01, Switch)
        }
    }else if( strcmp(CMD, "SET_PRESSURE") == 0){
		pressure = cJSON_GetObjectItem(pSub, "data")->valueint;
        if(pressure != pump_info.pressure){
			SEND_UART_TO_5832(0x02, pressure)
        }
    } else if (strcmp(CMD, "SET_QS_PRESSURE") == 0) {
		qs_pressure = cJSON_GetObjectItem(pSub, "data")->valueint;
        if(qs_pressure != pump_info.qs_pressure){
			SEND_UART_TO_5832(0x03, qs_pressure)
        }
    } else if (strcmp(CMD, "REQUEST") == 0) {
		ol_trace("mgt-#REQUEST#\r\n");
		request();
    }

	cJSON_Delete(pJson);
}

void ol_urc_print_handler(OL_ST_MSG *msg)
{
	switch(msg->param1)
	{
		case URC_ALIMUTH_MESSAGE_IND: {
				Urc_ALIAuth_T * ol_urc_auth = (Urc_ALIAuth_T *)(msg->param2);
				if(ol_urc_auth->status == OL_TRUE) {
					ol_trace("mgt-mqtt_auth success\r\n");
					ol_Sleep(500000);	// 延时一秒再connect，后续由内核升级，此处延时可省。
					RIL_AL_MQTT_Connect();
				}else {
					ol_trace("mgt-mqtt_auth fail, send reconnect msg\r\n");
					ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));
				}
				break;
			}
		case URC_ALIRECONNECT_MESAGE_IND:
			ol_trace("mgt-URC_ALIRECONNECT_MESAGE_IND\r\n");
			RIL_HTTP_ACTIVATE("cmnet");
			RIL_AL_MQTT_DeVerb(PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET);
			break;
		case URC_ALIMQTT_MESSAGE_IND: {
			Urc_ALIMqtt_T * ol_urc_mqtt = (Urc_ALIMqtt_T *)(msg->param2);
			ol_trace("mgt-+IMQTTRCVPUB:%s,%s,%s\r\n",ol_urc_mqtt->packet_id,ol_urc_mqtt->topic,ol_urc_mqtt->message);
			message_arrive(ol_urc_mqtt);
			break;
		}
		default: break;
	}
}

void uart_callback( Enum_SerialPort port, Enum_UARTEventType event, ol_bool pinLevel,void *customizePara)
{}

void uart_init(void)
{
	ST_UARTDCB  st_uartdcb;

	ol_UART_Register(OL_Port_0,uart_callback,NULL);
	st_uartdcb.baudrate = 9600;
	st_uartdcb.dataBits = 8;
	st_uartdcb.flowCtrl = 1;
	st_uartdcb.parity = 0;
	st_uartdcb.stopBits = 1;
	ol_UART_OpenEx(OL_Port_0, &st_uartdcb);

	MyCom.RdIndex = 0;
	MyCom.WrIndex = 0;
}

u16 urc_callback(u8* line,u16 param)
{
	if(Ol_RIL_IsURCStr(line))
	{
		OnURCHandler(line,NULL);
	}
	return 0;
}

char* PkgJson(char *CMD)
{
	char* pJsonString = NULL;
	cJSON * pJsonRoot = NULL;
	cJSON * pSubJson  = NULL;
	cJSON * pSub2Json = NULL;

	pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot) {
		goto JSON_END;
	}
	pSubJson = cJSON_CreateObject();
	if (NULL == pSubJson) {
		goto JSON_END;
	}
	pSub2Json = cJSON_CreateObject();
	if (NULL == pSub2Json) {
		goto JSON_END;
	}
	cJSON_AddNumberToObject(pSub2Json, "status", pump_info.status);
	cJSON_AddNumberToObject(pSub2Json, "pressure", pump_info.pressure);
	cJSON_AddNumberToObject(pSub2Json, "qs_pressure", pump_info.qs_pressure);
	cJSON_AddNumberToObject(pSub2Json, "err_code", pump_info.err_code);
	cJSON_AddNumberToObject(pSub2Json, "rtime", pump_info.rtime);
	cJSON_AddNumberToObject(pSub2Json, "power", pump_info.power);
	cJSON_AddNumberToObject(pSub2Json, "lowerlimit", pump_info.lowerlimit);
	cJSON_AddNumberToObject(pSub2Json, "upperlimit", pump_info.upperlimit);
	cJSON_AddNumberToObject(pSub2Json, "superlimit", pump_info.superlimit);
	cJSON_AddNumberToObject(pSub2Json, "step", pump_info.step);
	cJSON_AddItemToObject(pSubJson, "data", pSub2Json);
	cJSON_AddStringToObject(pSubJson, "cmd", CMD);
	cJSON_AddItemToObject(pJsonRoot, "param", pSubJson);
	cJSON_AddStringToObject(pJsonRoot, "devId", DEVICE_NAME);
	cJSON_AddNumberToObject(pJsonRoot, "pktid", pktID);
	cJSON_AddStringToObject(pJsonRoot, "systime", systime);

	pJsonString = cJSON_Print(pJsonRoot);
	if (NULL == pJsonString) {
		ol_trace("mgt-cJSON_Print error!  \r\n");
	}

JSON_END:
	cJSON_Delete(pJsonRoot);
	return pJsonString;
}

char* string_to_hex(char *str)
{
	int i = 0;
	const char ascTable[17] = {"0123456789ABCDEF"};

	if (str == NULL) return NULL;
	while (str[i] != 0)  {
		pub_hex[2*i] = ascTable[str[i] >> 4];
		pub_hex[2*i + 1] = ascTable[str[i] & 0x0f];
		i++;
	}
	pub_hex[2*i] = 0;
	return pub_hex;
}

// GPS
int split(char dst[][20], char* str, const char* spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);
    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    return n;
}
s32 ol_gps_raw_data_cb(unsigned char* buffer, u32 len, u32 flag)
{
	char status[2] = "V";
	u8 n = 0;
	//ol_trace("mgt-+GPS DATA: %s\r\n",buffer);
	//ol_print("GPS : %s\r\n", buffer);
	if (strstr(buffer, "$GNRMC") != NULL) {
		n = split(dst, buffer, ",");
		if (0 == strcmp(dst[2], "A")) {
			strcpy(GpsState.status, dst[2]);
			GpsState.Longitude[0] = '\0';
			strcat(GpsState.Longitude, dst[6]);
			strcat(GpsState.Longitude, dst[5]);
			GpsState.Latitude[0] = '\0';
			strcat(GpsState.Latitude, dst[4]);
			strcat(GpsState.Latitude, dst[3]);
			ol_trace("mgt-GPS >>>>> (%s, %s)\r\n", GpsState.Longitude, GpsState.Latitude);
		}else {
			ol_trace("mgt-GPS >>>>> not ok!\r\n");
		}
	}
}

void ReceiveFrom5832Uart(OL_ST_MSG *msg) {
	s32 	len = 0;
	u8 		data[100] 	= {0};	// num > REV_FLAME_SIZE_MAX
	
	// receive data
	len = ol_UART_Read(OL_Port_0, data, REV_FLAME_SIZE_MAX);
	if (len > 0) {
		ol_trace("mgt-{len=[%d]}\r\n", len);
		DataToRevBuf(data, len);
	}
	// clr Rx buffer,防止再不触发 OL_UART_READY_TO_READ_IND 消息！
	if (len == REV_FLAME_SIZE_MAX) {
		ol_UART_ClrRxBuffer(OL_Port_0);
	}
}

// _____________________________________________ MAIN  TASK ___________________________________________
void app_main(void *data)
{
#ifndef __DEBUG__
	OL_ST_MSG msg;
	Mqtt_Info *minfo;
	u32 status = 0;

	ol_trace("mgt-app main  \r\n");
	ol_Sleep(7000000);
	ol_RIL_SendATCmd("ATE0" , strlen("ATE0") , callback_Null , NULL, 1000);
	ol_ril_set_urc_callback(urc_callback);
	minfo->time_out=2;
	minfo->clean=1;			// mgt? 原士兰此处为 0
	minfo->keepalive=60;
	minfo->version ="3.1.1";
	// SET PARAM
	RIL_AL_MQTT_SetParam("TIMEOUT",minfo);
	RIL_AL_MQTT_SetParam("CLEAN",minfo);
	RIL_AL_MQTT_SetParam("KEEPALIVE",minfo);
	RIL_AL_MQTT_SetParam("VERSION",minfo);
	// INTO NET
	RIL_HTTP_ACTIVATE("cmnet");
	// MQTT AUTH
	RIL_AL_MQTT_DeVerb(PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET);
	// wait NET OK ...
	while(1)
	{
		oL_get_event((OL_ST_MSG *)&msg);
		ol_trace("mgt-ol_user_main->OL_get_event:%d\r\n",msg.message);
		switch(msg.message)
		{
			case MSG_ID_URC_INDICATION:	
				ol_urc_print_handler(&msg);
				break;
			default: break;
		}
		if (1 == fg_mqtt_state) break;
		ol_Sleep(100000);
	}

	ol_trace("mgt- NET NET NET OK OK OK\r\n");
	// user init ...
	mutex_pump_info = ol_sys_create_mutex("pump_info");
	mutex_uart_buf = ol_sys_create_mutex("uart_buf");
	uart_init();
	RIL_AL_MQTT_Sub(TOPIC_DATA, IOTX_MQTT_QOS1);
	ol_trace("mgt-finish Sub\r\n");
	// open gps
	ol_app_gps_get_status(0, &status);
	if(!status) {
		ol_app_gps_power_on(0, ol_gps_raw_data_cb);
	}
	// be able to real work!
	fg_initOK = 1;
	ol_trace("mgt-begin message loop ...\r\n");

	while(1)
	{
		oL_get_event((OL_ST_MSG *)&msg);
		ol_trace("mgt-ol_user_main->OL_get_event:%d\r\n",msg.message);
		switch(msg.message)
		{
			case OL_UART_READY_TO_READ_IND:
				ReceiveFrom5832Uart(&msg);	// 收到串口数据
				break;
			case MSG_ID_URC_INDICATION:	
				ol_urc_print_handler(&msg);	// 收到远程遥控数据
				break;
			default: break;
		}
		ol_Sleep(100000);
	}
#endif
	return;
}

// exe uart cmd
void app_ext2_main(void *data)
{
#ifndef __DEBUG__
	char	*JsonString = NULL;

	while (fg_initOK == 0)	ol_Sleep(100000);
	ol_trace("mgt-app_ext2_main start ...");
	while(1) {
		if (ReadCmd()) {
			ol_sys_take_mutex(mutex_pump_info);

			pump_info.status = MyCom.Cmd[5];
			pump_info.pressure = MyCom.Cmd[6];
			pump_info.qs_pressure = MyCom.Cmd[7];
			pump_info.err_code = MyCom.Cmd[9];
			pump_info.rtime = MyCom.Cmd[10];
			pump_info.rtime<<=8;
			pump_info.rtime = pump_info.rtime + MyCom.Cmd[11];
			pump_info.power = MyCom.Cmd[12];
			pump_info.power<<=8;
			pump_info.power = pump_info.power + MyCom.Cmd[13];
			pump_info.lowerlimit = MyCom.Cmd[14];
			pump_info.upperlimit = MyCom.Cmd[15];
			pump_info.superlimit = MyCom.Cmd[16];
			pump_info.step = MyCom.Cmd[17];
			JsonString = PkgJson("RESPONSE");

			ol_sys_give_mutex(mutex_pump_info);

			ol_trace("\r\n mgt---------------%s", JsonString);
			//request(); // for debug
			RIL_AL_MQTT_Pub(TOPIC_UPDATE, 1, string_to_hex(JsonString));
			ol_trace("mgt-***pub***\r\n");
			if (JsonString != NULL) ol_mem_free(JsonString);
		}
		while(0 == fg_mqtt_state) ol_Sleep(1000000);	// 断网阻塞在这里
		ol_Sleep(50000);
	}
#endif
 }

// heart beating ----- server data /60s
void app_ext3_main(void *data)
{
#ifndef __DEBUG__
	char *JsonString = NULL;

	while (fg_initOK == 0)	ol_Sleep(100000);
	ol_trace("mgt-app_ext3_main start ...");
	request();
	ol_Sleep(500000);
	
    while(1) {
		ol_sys_take_mutex(mutex_pump_info);

		JsonString = PkgJson("HEARTBEAT");

		ol_sys_give_mutex(mutex_pump_info);

		RIL_AL_MQTT_Pub(TOPIC_DATA_SERVER, 1, string_to_hex(JsonString));
		ol_trace("mgt-***pub60s***\r\n");
		if (JsonString != NULL) ol_mem_free(JsonString);
		
		while(0 == fg_mqtt_state) ol_Sleep(1000000);	// 断网阻塞在这里
        ol_Sleep(60000000);	// 一分钟一次
    }
#endif
}

// net checking
void app_ext4_main(void *data)
{
#ifndef __DEBUG__
	OL_ST_MSG msg;
	static u8 offline_cnt = 0;

	while (fg_initOK == 0)	ol_Sleep(100000);
	ol_trace("mgt-app_ext4_main start ...");
	while(1)
	{
		if (Mqtt_State_Check()) {
			if (0 == fg_mqtt_state) {
				ol_trace("mgt-[fg_mqtt_state == 0]");
				if (++offline_cnt >= 6) {	// 延时一分钟判断
					offline_cnt = 0;
					ol_trace("mgt- NET OFFLINE!");
					ol_Sleep(2000000);	// 等待其他任务的网络任务停止
					ol_OS_SendMessage(URC_RCV_TASK_ID, MSG_ID_URC_INDICATION, URC_ALIRECONNECT_MESAGE_IND, (u32)(0));
					while(0 == fg_mqtt_state) ol_Sleep(100000);
					ol_trace("mgt- NET ONLINE again! -- [state:%d]", fg_mqtt_state);
				}
			}else {
				offline_cnt = 0;
			}
		}
		ol_trace("mgt-reconnect mqtt state : %d", fg_mqtt_state);
		ol_Sleep(10000000);
	}
#endif
}

//     init 
#pragma arm section code="INIT_APP"
void app_initial(void *data)
{	
    //don't remove it!!////
	OL_InitRegions();  //it very importent!!!
    //////////////////////
}
#pragma arm section code 
void app_ext2_initial(void *data)
{}
void app_ext3_initial(void *data)
{}
void app_ext4_initial(void *data)
{}
void* mgt_malloc(u16 len){return ol_mem_alloc(len);}
void* mgt_free(void *pt){return ol_mem_free(pt);}


