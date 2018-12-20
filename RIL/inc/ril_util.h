/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in 'ril_until.c'.
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
#ifndef __RIL_UTIL_H__
#define __RIL_UTIL_H__
#include "Ol_type.h"

typedef enum {
    CHAR_0 = '0',
    CHAR_9 = '9',
    CHAR_A = 'A',
    CHAR_F = 'F',
    END_OF_STR = '\0'
}Enum_Char;
#define IS_NUMBER(alpha_char)   \
    (((alpha_char >= CHAR_0) && (alpha_char <= CHAR_9) ) ? 1 : 0)

extern s32  ol_StrPrefixMatch(const char* str, const char *prefix);
extern ol_bool ol_HexStrToInt(u8 *str, u32 *val);
extern char* ol_StrToUpper(char* str);
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
extern char* ol_RIL_FindString_ext(char *line, u32 len,char *str);

/******************************************************************************
* Function:     ol_RIL_FindLine_ext
*  
* Description:
*                This function is used to find the specified character line by line.
*                for example,if you want to find "OK", In fact, we think that you are
*                looking for <CR><LF>OK<CR><LF>,<CR>OK<CR> or <LF>OK<LF>.
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
extern char* ol_RIL_FindLine_ext(char *line, u32 len,char *str);

#endif
