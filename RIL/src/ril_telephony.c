
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_sim.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module implements sim card related APIs.
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
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h "
#include <stdlib.h>
#include <string.h>
#include "ol_ril.h"
#include "ol_iostream.h"
#include "ol_system.h"


ST_ComingCallInfo  g_comingCall;



static s32 Telephony_Dial_AT_handler(char* line, u32 len, void* userdata)
{
    char* head = ol_RIL_FindLine_ext(line, len, "NO DIALTONE"); 
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_NO_DIALTONE;
        return  RIL_ATRSP_SUCCESS;
    }
    
    head = ol_RIL_FindLine_ext(line, len, "BUSY"); 
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_BUSY;     
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "NO CARRIER"); 
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_NO_CARRIER;          
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "OK");
    if(head)
    {  
        (*(s32* )userdata) = CALL_STATE_OK;
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");
    if(head)
    {  
        (*(s32* )userdata) = CALL_STATE_ERROR;
        return  RIL_ATRSP_FAILED;
    } 

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_ERROR;
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32 RIL_Telephony_Dial(u8 type, char* phoneNumber, s32 *result)
{
    char strAT[40];
    if (NULL == phoneNumber)
    {
        return -1;
    }
    memset(strAT, 0x0, sizeof(strAT));
  	sprintf(strAT, "ATD%s;", phoneNumber);
    return ol_RIL_SendATCmd(strAT, Ol_strlen(strAT), Telephony_Dial_AT_handler, (void* )result, 2000);
}

static s32 Telephony_Answer_AT_handler(char* line, u32 len, void* userdata)
{
    char* head = ol_RIL_FindLine_ext(line, len, "OK"); 
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_OK;
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "NO CARRIER"); 
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_NO_CARRIER;
        return  RIL_ATRSP_SUCCESS;
    }

    head = ol_RIL_FindLine_ext(line, len, "ERROR");
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_ERROR;
        return  RIL_ATRSP_FAILED;
    }

    head = ol_RIL_FindString_ext(line, len, "+CME ERROR:");//fail
    if(head)
    {
        (*(s32* )userdata) = CALL_STATE_ERROR;
        return  RIL_ATRSP_FAILED;
    }

    return RIL_ATRSP_CONTINUE; //continue wait
}

s32 RIL_Telephony_Answer(s32 *result)
{
    return ol_RIL_SendATCmd("ATA", Ol_strlen("ATA"), Telephony_Answer_AT_handler, result, 1000);
}

s32 RIL_Telephony_Hangup(void)
{
    return ol_RIL_SendATCmd("ATH", Ol_strlen("ATH"), NULL, NULL, 500);
}

