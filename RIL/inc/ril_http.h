/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_http.c'.
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

#ifndef __RIL_HTTP_H__
#define __RIL_HTTP_H__

typedef enum
{
    HTTP_ACTION_GET_REQ,
    HTTP_ACTION_POST_REQ,
    HTTP_ACTION_HEAD_REQ,
}Enum_HTTP_Action;


#define 	OL_HTTP_STATUS_OK 							200
#define 	OL_HTTP_STATUS_CREATED 						201
#define 	OL_HTTP_STATUS_ACCEPTED 					202
#define 	OL_HTTP_STATUS_PARTIAL 						203
#define 	OL_HTTP_STATUS_NO_CONTENT 					204
#define 	OL_HTTP_STATUS_AMBIGUOUS 					300
#define 	OL_HTTP_STATUS_MOVED 						301
#define 	OL_HTTP_STATUS_REDIRECT 					302
#define 	OL_HTTP_STATUS_REDIRECT_METHOD 				303
#define 	OL_HTTP_STATUS_NOT_MODIFIED 				304
#define 	OL_HTTP_STATUS_USE_PROXY 					305
#define 	OL_HTTP_STATUS_TEMP_REDIRECT 				307
#define 	OL_HTTP_STATUS_BAD_REQUEST 					400
#define 	OL_HTTP_STATUS_DENIED 						401
#define 	OL_HTTP_STATUS_PAYMENT_REQ 					402
#define 	OL_HTTP_STATUS_FORBIDDEN 					403
#define 	OL_HTTP_STATUS_NOT_FOUND 					404
#define 	OL_HTTP_STATUS_BAD_METHOD 					405
#define 	OL_HTTP_STATUS_NONE_ACCEPTABLE 				406
#define	 	OL_HTTP_STATUS_PROXY_AUTH_REQ 				407
#define 	OL_HTTP_STATUS_REQUEST_TIMEOUT 				408
#define 	OL_HTTP_STATUS_CONFLICT 					409
#define 	OL_HTTP_STATUS_GONE 						410
#define 	OL_HTTP_STATUS_AUTH_REFUSED 				411
#define 	OL_HTTP_STATUS_SERVER_ERROR 				500
#define 	OL_HTTP_STATUS_NOT_SUPPORTED 				501
#define 	OL_HTTP_STATUS_BAD_GATEWAY 					502
#define 	OL_HTTP_STATUS_SERVICE_UNAVAIL			    503
#define 	OL_HTTP_STATUS_GATEWAY_TIMEOUT 				504
#define 	OL_HTTP_STATUS_NOT_HTTP_PDU					600
#define 	OL_HTTP_NETWORK_ERROR						601
#define 	OL_HTTP_NO_MEMORY							602
#define 	OL_HTTP_DNS_ERROR							603
#define 	OL_HTTP_STACK_ERROR							604

#define 	OL_HTTP_CME_FILE_NOT_FIND_ERROR 			188

#define 	OL_HTTP_MEMORY_FAIL  						20
#define 	OL_HTTP_PDP_AUTH_FAIL  						149
#define 	OL_HTTP_DNS_RESOLVE_FAIL 					160


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_ACTIVATE
 * DESCRIPTION
 *  This function is used to activate the HTTP connection.
 * PARAMETERS
 *
 * apn        :[in]  network access point name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_ACTIVATE(u8 * apn);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_INITIALIZE
 * DESCRIPTION
 *  This function is used to Initialize HTTP Service.
 * PARAMETERS
 *
 * void
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_INITIALIZE(void);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_CLOSE
 * DESCRIPTION
 *  This function is used to Terminate HTTP Service.
 * PARAMETERS
 *
 * void
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_CLOSE(void);



/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_URL
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * strURL        :[in]  network site name.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_URL(char* strURL);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_PROXY
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * strDNS        :[in]  proxy ip.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_PROXY(char* strIP);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_PORT
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * port         :[in]  port.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_PORT(u32 port);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_REDIR
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * flag         :[in]  This flag controls the redirection mechanism ,
 *						when it is acting as HTTP client (numeric). 
 *						If the server sends a redirect code
 *						(range 30x), the client will automatically send a
 * 						new HTTP request when the flag is set to (1).
 *						Default value is 0 (no redirection)..
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_REDIR(u32 flag);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_BREAK
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * scope         :[in]  Parameter for HTTP method "GET", used for resuming broken transfer.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_BREAK(u32 scope);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_BREAKEND
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * scope         :[in]  Parameter for HTTP method "GET", used for resuming broken transfer. which is used together
 *                       with "BREAK".
 *                      If the value of "BREAKEND" is bigger than "BREAK", the transfer scope is from "BREAK" to
 *                       "BREAKEND".
 *						If the value of "BREAKEND" is smaller than "BREAK", the transfer scope is from "BREAK" to
 *                       the end of the file.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_BREAKEND(u32 scope);



/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_SetServer_TIMEOUT
 * DESCRIPTION
 *  This function is used to Set HTTP Parameters Value.
 * PARAMETERS
 *
 * time         :[in]  if both "BREAKEND" and "BREAK" are 0, the resume broken transfer function is disabled.
 *                     Default value is 120 seconds.HTTP Parameter value. Type and supported content depend on related <HTTPParamTag>.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_SetServer_TIMEOUT(u32 time);



/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_Request
 * DESCRIPTION
 *  This function is used to set HTTP method action.
 * PARAMETERS
 *
 * action_req      :[in]  HTTP method specification.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_Request(Enum_HTTP_Action action_req);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_ReadResponse
 * DESCRIPTION
 *  This function is used to read the HTTP server response.
 * PARAMETERS
 *
 * start_address     :[in]  initial position.
 *
 * byte_size         :[in]  the size of reading.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_ReadResponse(u32 start_address,u32 byte_size);


/*****************************************************************************
 * FUNCTION
 *  RIL_HTTP_DownloadFile
 * DESCRIPTION
 *  This function is used to download HTTP data.
 * PARAMETERS
 *
 * byte_size          :[in]  the size of download.
 *
 * timeout            :[in]  timeout.
 *
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_HTTP_DownloadFile(u32 byte_size,u32 timeout);

#endif  //__RIL_HTTP_H__

