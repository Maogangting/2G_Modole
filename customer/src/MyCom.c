#include "Mycom.h"
#include "ol_interface.h"
#include "ol_system.h"

CMyCom MyCom = { {0},{0},0,0}; // uart
extern u32 mutex_uart_buf;

#define DELAY_BYTES_MAX	50	// 串口缓冲最大延迟处理字节数

// 串口原始数据存储
void DataToRevBuf(u8 *RevBuf, int DataNum)
{
	ol_sys_take_mutex(mutex_uart_buf);
	// 串口处理延迟，当超过约3个数据指令时，清空。
	if (MyCom.RdIndex + DELAY_BYTES_MAX < MyCom.WrIndex) {
		ol_trace("mgt-uart buf deta >80 ,clear one time\r\n");
		MyCom.WrIndex = 0;
		MyCom.RdIndex = 0;
	}

	if (MyCom.WrIndex + DataNum >= COM_REV_BUF_SIZE)
	{
		memcpy(MyCom.AfxRevBuf, MyCom.AfxRevBuf + MyCom.RdIndex, MyCom.WrIndex - MyCom.RdIndex);
		MyCom.WrIndex -= MyCom.RdIndex;
		MyCom.RdIndex = 0;
	}
	if (MyCom.WrIndex + DataNum < COM_REV_BUF_SIZE) {
		memcpy(MyCom.AfxRevBuf + MyCom.WrIndex, RevBuf, DataNum);
		MyCom.WrIndex += DataNum;
	}
	ol_sys_give_mutex(mutex_uart_buf);
}

// 读取完整一条数据
u8 ReadCmd()
{
	u8 CRC = 0;
	int frame_length = 0;
	u8 ret = 0;
	int i = 0;

	ol_sys_take_mutex(mutex_uart_buf);

	while (MyCom.RdIndex + 1 < MyCom.WrIndex && !(MyCom.AfxRevBuf[MyCom.RdIndex] == UART_START && MyCom.AfxRevBuf[MyCom.RdIndex+1] == UART_SECOND)) MyCom.RdIndex++;		// ������ͷ
	if (MyCom.RdIndex + 2 < MyCom.WrIndex) {
		// 找到一条完整数据
		frame_length = MyCom.AfxRevBuf[MyCom.RdIndex + 2];
		if (MyCom.WrIndex > MyCom.RdIndex + frame_length + 3) {
			for (i = 2; i < frame_length + 3; i++)
				CRC -= MyCom.AfxRevBuf[MyCom.RdIndex + i];
			if (CRC == MyCom.AfxRevBuf[MyCom.RdIndex + frame_length + 3]) {

				memcpy(MyCom.Cmd, MyCom.AfxRevBuf + MyCom.RdIndex, frame_length + 4);
				MyCom.RdIndex += frame_length + 4;
				ret = 1;
				goto RET_CMD;
			}
			MyCom.RdIndex += frame_length;
		}
	}

RET_CMD:
	ol_sys_give_mutex(mutex_uart_buf);
	return ret;
}