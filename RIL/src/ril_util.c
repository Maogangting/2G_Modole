/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2013
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_util.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module implements some common API functions.
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
#include "ril_util.h "
//#include "ql_memory.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "ol_type.h"
#include "ol_interface.h"
#include "ol_iostream.h"
#include "ol_gps.h"
#include "ol_soc.h"
#include "ol_system.h"
#include "ol_ril.h"
#include "ol_timer.h"
#include "ol_type.h"
#include "ol_uart.h"
#include "ol_gpio.h"
#include "ol_wtd.h"
#include "ril.h"
#include "ril_telephony.h"
#include "ril_util.h"
#include "ril_sim.h"
#include "ril_network.h"
#include "ril_telephony.h"
#include "ril_ftp.h"
#include "ril_urc.h"

s32 ol_StrPrefixMatch(const char* str, const char *prefix)
{
    for ( ; *str != '\0' && *prefix != '\0' ; str++, prefix++) {
        if (*str != *prefix) {
            return 0;
        }
    }
    return 1;
}

char* ol_StrToUpper(char* str)
{
    char* pCh = str;
    if (!str)
    {
        return NULL;
    }
    for ( ; *pCh != '\0'; pCh++)
    {
        if (((*pCh) >= 'a') && ((*pCh) <= 'z'))
        {
            *pCh = toupper(*pCh);
        }
    }
    return str;
}

ol_bool ol_HexStrToInt(u8* str, u32* val)
{
    u16 i = 0;
    u32 temp = 0;

    //ASSERT((str != NULL) && (val != NULL));
    if (NULL == str || NULL == val)
    {
        return FALSE;
    }
    ol_StrToUpper((char*)str);

    while (str[i] != '\0')
    {
        if (IS_NUMBER(str[i]))
        {
            temp = (temp << 4) + (str[i] - CHAR_0);
        }
        else if ((str[i] >= CHAR_A) && (str[i] <= CHAR_F))
        {
            temp = (temp << 4) + ((str[i] - CHAR_A) + 10);
        }else{
            return FALSE;
        }
        i++;
    }
    *val = temp;
    return TRUE;
}

/******************************************************************************
* Function:     ol_RIL_FindString_ext
*  
* Description:
*                This function is used to match string within a specified length.
*                This function is very much like strstr.
*
* Parameters:    
*                line:  
*                    [in]The address of the string.
*                len:   
*                    [in]The length of the string.
*                str:   
*                    [in]The specified item which you want to look for in the string.
*
* Return:  
                The function returns a pointer to the located string,
                or a  null  pointer  if  the specified string is not found.
******************************************************************************/
char* ol_RIL_FindString_ext(char *line, u32 len,char *str)
{
    s32 i;
    s32 str_len;
    char *p;

    if ((NULL == line) || (NULL == str))
        return NULL;
    
    str_len = strlen(str);
    if(str_len > len)
    {
        return NULL;
    }

    p = line;
    for (i = 0;i < len - str_len + 1; i++)
    {
        if (0 == strncmp (p, str, str_len))
        {
            return p;
        }else{
            p++;
        }
    }
    return NULL;
}

/******************************************************************************
* Function:     Ol_RIL_FindLine_ext
*  
* Description:
*                This function is used to find the specified character line by line.
*                for example,if you want to find "OK", In fact, we think that you are
*                looking for <CR><LF>OK<CR><LF>,OK<CR><LF>,<CR>OK<CR> or <LF>OK<LF>.
*
*
* Parameters:    
*                line:  
*                    [in]The address of the string.
*                len:   
*                    [in]The length of the string.
*                str:   
*                    [in]The specified item which you want to look for in the string.
*
* Return:  
                The function returns a pointer to the located string,
                or a  null  pointer  if  the specified string is not found.
******************************************************************************/
char* ol_RIL_FindLine_ext(char *line, u32 len,char *str)
{
    s32 i = 0;
    s32 strlen = 0;
    char *p = NULL;
    char *pStr = NULL;
    char *pStr2 = NULL;
    char *pStr3 = NULL;

    if ((NULL == line) || (NULL == str))
        return NULL;
    
    strlen = Ol_strlen (str);
    
    pStr = (char *)ol_mem_alloc(strlen + 4 + 1);
    if (NULL == pStr)
         return NULL;
    
    if (len >= strlen + 4)//two \r\n
    {
        p = line;
        memset(pStr, 0, strlen + 5);
        sprintf(pStr,"\r\n%s\r\n",str);
        for (i = 0;i < len - (strlen + 4) + 1; i++)
        {
            if (0 == strncmp(p, pStr, strlen + 4))
            {
                ol_mem_free(pStr);
                return p;
            }else{
                p++;
            }
        }
    }

    if (len >= strlen + 2)//two \r or two\n
    {
        p = line;

        // <CR>xx<CR>
        memset(pStr, 0, strlen + 5);
        sprintf(pStr,"\r%s\r",str);

        // <LF>xx<LF>
        pStr2 = (char*)ol_mem_alloc(strlen + 5);
        memset(pStr2, 0, strlen + 5);
        sprintf(pStr2,"\n%s\n",str);

        // xx<CR><LF>
        pStr3 = (char*)ol_mem_alloc(strlen + 5);
        memset(pStr3, 0, strlen + 5);
        sprintf(pStr3,"%s\r\n",str);

        for (i = 0;i < len - (strlen + 2) + 1; i++)
        {
            if ((0 == strncmp (p, pStr, strlen + 2)) ||
                (0 == strncmp (p, pStr2, strlen + 2)) ||
                (0 == strncmp (p, pStr3, strlen + 2)))
            {
                ol_mem_free(pStr);
                ol_mem_free(pStr2);
                ol_mem_free(pStr3);
                pStr = NULL;
                pStr2 = NULL;
                pStr3 = NULL;
                return p;
            }else{
                p++;
            }
        }
        ol_mem_free(pStr2);
        ol_mem_free(pStr3);
        pStr2 = NULL;
        pStr3 = NULL;
    }
    ol_mem_free(pStr);
    pStr = NULL;
    
    return NULL;
}

u32 Ql_GenHash(char* strSrc, u32 len)
{
    u32 h, v;
    u32 i;
    for (h = 0, i = 0; i < len; i++)
    {
        h = (u32)(5527 * h + 7 * strSrc[i]);
        v = h & 0x0000ffff;
        h ^= v * v;
    }
    return h;
}
