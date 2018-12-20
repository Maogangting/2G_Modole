/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_dtmf.c'.
 |      NOTE: The interfaces are based on portable codes. Therefore they are not related to platform.
 |
 |--------------------------------------------------------------------------
 |
 |  Designed by     :   K.chen
 |  Coded    by     :   K.chen
 |  Tested   by     :   K.chen
 |--------------------------------------------------------------------------
 | Revision History
 | ----------------
 |  2017/1/1       K.chen        Create this file 
 |  ------------------------------------------------------------------------
 \=========================================================================*/
#ifndef __RIL_SYSTEM_H__
#define __RIL_FTP_H__
#include "ol_uart.h"
#include "ol_type.h"

typedef struct {
    s32 index;
	s32 ftp_ret;
}FTP_LOAD_T;

#define   OL_FTP_GET_SIGNAL    0X2999
#define   ol_ftpget_case        1
typedef struct
{
	u32 get_length;
	u8  get_buffer[1460];
}FtpGet_Struct;




#define FTP_NET_ERROR                             61                  //网络错误
#define FTP_DNS_ERROR                             62                  //DNS错误
#define FTP_CONNECT_ERROR                    63                  //连接错误
#define FTP_TIMEROUT                                64                  //超时
#define FTP_SERVER_ERROR                       65                  //服务器错误
#define FTP_OPERATION_NOT_ALLOW        66                 //操作禁止
#define FTP_REPLAY_ERROR                       70                 //应答错误
#define FTP_USER_ERROR                           71                 //用户错误
#define FTP_PASSWORD_ERROR                 72                 //口令错误
#define FTP_TYPE_ERROR                            73                 //类型错误
#define FTP_REST_ERROR                            74                 //保持错误
#define FTP_PASSIVE_ERROR                      75                 //被动错误
#define FTP_ACTIVE_ERROR                        76                 //主动错误
#define FTP_OPERATE_ERROR                      77                 //操作错误
#define FTP_UPLOAD_ERROR                        78                //上传错误
#define FTP_DOWNLOAD_ERROR                  79                //下载错误
#define FTP_FILE_ACTION_ERROR		      80
#define FTP_MANUAL_QUIT                   86                  //手动关闭连接
#define FTP_ERROR_FLAG                            100

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPOPEN
 * DESCRIPTION
 *  This function is used to set the configuration information of FTP.
 * PARAMETERS
 *  hostName        :[in]  server address.
 *
 *
 port            :[in]  server port number.
 *
 *  userName        :[in]  login name.
 *
 *  password        :[in]  login password.
 *
 *  mode            :[in]  0    Active FTP mode.
 *                         1    Passive FTP mode.
 *
 *  type            :[in]  "I"   For FTP Binary sessions.
 *                         "A"   For FTP ASCII sessions.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPOPEN(u8* hostName, u32 port,u8* userName,u8* password, ol_bool mode,u8* type);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_ACTIVATE
 * DESCRIPTION
 *  This function is used to activate the FTP connection.
 * PARAMETERS
 *
 * apn        :[in]  network access point name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_ACTIVATE(u8* apn);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPQUIT
 * DESCRIPTION
 *  This function is used to isconnect the network connection.
 * PARAMETERS
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPQUIT(void);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPPUT
 * DESCRIPTION
 *  This function is used to create a network connection and the FTP server.
 * PARAMETERS
 *  fileName        :[in]  file name.
 *
 *
 putopt          :[in]  "APPE"   For appending file.
 *						   "STOU"   For storing unique file.
 *						   "STOR"   For storing file.
 *
 *
 *  filepath        :[in]  file path.
 *
 *  timeOut         :[in]  time out.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPPUT(u8* fileName,u8* putopt, u8* filepath,u32 timeOut);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTPPUTFILE
 * DESCRIPTION
 *  This function is used to upload a file to the FTP server.
 * PARAMETERS
 *
 *  size            :[in]  upload file size.
 *
 *
 file_data       :[in]  the content of the upload file.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTPPUTFILE(s32 size,u8* file_data);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPGET
 * DESCRIPTION
 *  This function is used to create a network connection and the FTP server.
 * PARAMETERS
 *
 *  fileName        :[in]  download file name.
 *
 *
 filepath        :[in]  the path of the download file.
 *
 *  filerest        :[in]  the offset of the download file.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPGET(u8* fileName,u8* filepath, u32 filerest);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPGETDATA
 * DESCRIPTION
 *  This function is used to download file from the FTP server.
 * PARAMETERS
 *
 *  size           :[in]  download file size.
 *
 *  enable         :[in]  if TRUE Will receive a signal OL_FTP_GET_SIGNAL.
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPGETDATA(s32 size,ol_bool enable);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPGETDATA
 * DESCRIPTION
 *  This function is used to set getpath.
 * PARAMETERS
 *
 *  pathName         :[in]  path name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPGETPATH(u8* pathName);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPPUTPATH
 * DESCRIPTION
 *  This function is used to set putpath.
 * PARAMETERS
 *
 *  pathName         :[in]  path name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPPUTPATH(u8* pathName);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPSIZE
 * DESCRIPTION
 *  This function is used to get file info.
 * PARAMETERS
 *
 *  fileName         :[in]  file name.
 *
 *  filepath         :[in]  filepath
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPSIZE(u8* fileName,u8* filepath);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPSIZE
 * DESCRIPTION
 *  This function is used to delete file from FTP server.
 * PARAMETERS
 *
 *  fileName         :[in]  file name.
 *
 *  filepath         :[in]  filepath
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPDELETE(u8* fileName,u8* filepath);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPMKDIR
 * DESCRIPTION
 *  This function is used create a directory on the server.
 * PARAMETERS
 *
 *  pathName         :[in]  path name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPMKDIR(u8* pathName);

/*****************************************************************************
 * FUNCTION
 *  RIL_FTP_FTPRMDIR
 * DESCRIPTION
 *  This function is used delete a directory on the server.
 * PARAMETERS
 *
 *  pathName         :[in]  path name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_FTP_FTPRMDIR(u8* pathName);



#endif


