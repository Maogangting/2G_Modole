/*==========================================================================
 |               Mobiletek OpenCPU --  Library header files
 |
 |              Copyright (c) 2017 Mobiletek Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      This file defines interfaces which are used in "ril_audio.c".
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
#ifndef __RIL_AUD_H__
#define __RIL_AUD_H__

typedef enum 
{
     AUD_CHANNEL_NORMAL= 0,
     AUD_CHANNEL_HEADSET,
     AUD_CHANNEL_LOUD_SPEAKER
} Enum_AudChannel;



/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_PlayFile
 * DESCRIPTION
 *  This function is used to play audio file.
 * PARAMETERS
 *	sound_level	   :[in] play audio sound size.
 *  filePath       :[in]source file name with filepath
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_AUD_PlayFile(u8 sound_level,char* filePath);

/*****************************************************************************
 * FUNCTION
 *      RIL_AUD_StopPlay
 * DESCRIPTION
 *      This function is used to stop play audio file.
 * PARAMETERS
 *      void
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_AUD_StopPlay(void);

/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_StartRecord
 * DESCRIPTION
 *  This function is used to start record audio.
 * PARAMETERS
 *  fileName        :[in]source file name
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_AUD_StartRecord(char* fileName);

/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_StopRecord
 * DESCRIPTION
 *  This function is used to stop record audio.
 * PARAMETERS
 *  void
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_AUD_StopRecord(void);

/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_GetRecordFileInfo
 * DESCRIPTION
 *  This function is used to get record file information.
 * PARAMETERS
 *
 * fileinfo        :[out]   record file number,name,size. 
 *        
 * RETURNS
 *                
 *****************************************************************************/
s32 RIL_AUD_GetRecordFileInfo(u8 * fileinfo);

/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_SetVolume
 * DESCRIPTION
 *  This function is used to set the volume level according to the specified volume type.
 * PARAMETERS
 *  pvolLevel      :[in] volume level.
 * RETURNS
 *  s32    OL_RET_OK means AT execute suceess,see 'Error Code Definition'
 *****************************************************************************/
s32  RIL_AUD_SetVolume(u8 volLevel);

/*****************************************************************************
 * FUNCTION
 *  RIL_AUD_SetChannel
 * DESCRIPTION
 *  This function is used to set the audio channel.
 * PARAMETERS
 *  audChannel       :[out] audio channel,see Enum_AudChannel
 * RETURNS
 *                RIL_AT_SUCCESS, this function succeeds.
 *                Or, please see the definition of Enum_ATSndError.
 *****************************************************************************/
s32 RIL_AUD_SetChannel(Enum_AudChannel audChannel);


#endif  //__RIL_AUD_H__
