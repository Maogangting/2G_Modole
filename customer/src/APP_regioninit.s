;/*****************************************************************************
;*  Copyright Statement:
;*  --------------------
;*  This software is protected by Copyright and the information contained
;*  herein is confidential. The software may not be copied and the information
;*  contained herein may not be used or disclosed except with the written
;*  permission of MediaTek Inc. (C) 2005
;*
;*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
;*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
;*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
;*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
;*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
;*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
;*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
;*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
;*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
;*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
;*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
;*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
;*
;*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
;*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
;*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
;*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
;*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
;*
;*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
;*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
;*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
;*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
;*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
;*
;*****************************************************************************/
;
;/*******************************************************************************
; *
; * Filename:
; * ---------
; *	regioninit.s
; *
; * Project:
; * --------
; *   Bootloader
; *
; * Description:
; * ------------
; *   This Module defines the initialization of RW/ZI/RO data for each area..
; *
; * Author:
; * -------
; * -------
; *
; *============================================================================
; *             HISTORY
; * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
; *------------------------------------------------------------------------------
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; * removed!
; *------------------------------------------------------------------------------
; * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
; *============================================================================
; ****************************************************************************/

;**************************************************************
;*     File: regioninit.s                                     *
;*  Purpose: Application Startup Code                         *
;**************************************************************
;
; This file contains the macro and supporting subroutines to
; copy RO code and RW data from ROM to RAM and zero-initialize
; the ZI data areas in RAM.

   IF  (:LNOT: :DEF: __USB_DOWNLOAD__)
      GBLL         __USB_DOWNLOAD__
__USB_DOWNLOAD__   SETL    {FALSE}
   ENDIF

   IF  (:LNOT: :DEF: __FOTA_ENABLE__)
      GBLL         __FOTA_ENABLE__
__FOTA_ENABLE__   SETL    {FALSE}
   ENDIF


;  /* Added by Anthony Chin 10/28/2001. */
 CODE32
 
        AREA RegionInit, CODE, READONLY

        EXPORT OL_InitRegions
   IF  (:DEF: __MINI_BOOTLOADER__)
        EXPORT INT_InitEMIInitCode
   ENDIF
   
   IF  (:DEF: __EXT_BOOTLOADER__)
        EXPORT InitRegionsExt
        EXPORT InitRegionPre
   ENDIF        
   
;/*************************************************************************/
;/*                                                                       */
;/* FUNCTION                                                              */
;/*   INT_InitEMIInitCode                                                 */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*   Initialize the EMI setting related code regions                     */
;/*                                                                       */
;/* INPUTS                                                                */
;/*   None                                                                */
;/*                                                                       */
;/* OUTPUTS                                                               */
;/*   None                                                                */
;/*************************************************************************/
; VOID INT_InitEMIInitCode(VOID)
; {

OL_InitRegions

        IMPORT |Load$$APP_RAM$$Base|, WEAK
        IMPORT |Image$$APP_RAM$$Base|, WEAK
        IMPORT |Image$$APP_RAM$$Length|, WEAK
        IMPORT |Image$$APP_RAM$$ZI$$Length|, WEAK
        IMPORT |Image$$APP_RAM$$ZI$$Base|, WEAK

	 STMFD  SP!, {R0-R7,lr}
        LDR     R0, =|Load$$APP_RAM$$Base|
        LDR     R1, =|Image$$APP_RAM$$Base|
        MOV     R2, R1
        LDR     R4, =|Image$$APP_RAM$$Length|
        ADD     R2, R2, R4
        BL      InitRegionPre_copy
        LDR     R2, =|Image$$APP_RAM$$ZI$$Length|
        LDR     R0, =|Image$$APP_RAM$$ZI$$Base|
        MOV     R1, R0
        ADD     R1, R1,R2
        BL      zi_init

        LDMFD   SP!, {R0-R7,pc}
 ;}
       
InitRegionPre_copy        
        CMP     R1, R2
        LDRLO   R4, [R0], #4
        STRLO   R4, [R1], #4
        BLO     InitRegionPre_copy
        MOV     pc, lr          ; return from subroutine InitRegionPre_copy

        
zi_init
        MOV     R2, #0
        CMP     R0, R1          ; loop whilst r0 < r1
        STRLO   R2, [R0], #4
        BLO     zi_init 
        MOV     pc, lr          ; return from subroutine zi_init  
        
        END

