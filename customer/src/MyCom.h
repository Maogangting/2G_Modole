
#ifndef _H_MYCOM_H_
#define _H_MYCOM_H_

#include "ol_type.h"

enum {
	UART_START			= 0xff,
	UART_SECOND			= 0x55,
	COM_REV_BUF_SIZE	= 1024,		// 1K
	REV_FLAME_SIZE_MAX	= 64
};


typedef struct 
{
	u8	Cmd[REV_FLAME_SIZE_MAX];		// 一条完整数据
	u8	AfxRevBuf[COM_REV_BUF_SIZE];	// 串口原始数据仓库
	s16		RdIndex,WrIndex;			// 串口原始数据读写定位
	//u8	bOpen;
}CMyCom;

extern CMyCom MyCom; // uart

extern u8 ReadCmd();
extern int UartSendCmd(u8 *sentbuf);
extern int ReadMyCom(void* lpInBuffer);      // ���˿�
extern void DataToRevBuf(u8 *RevBuf, int DataNum);

#endif