#pragma once

#define SZ_DM_NAME_11		_T("DM_HANDLER_OPT")

//============================================================BOOL <start>
#define BDM11_MD_ConinueFail	          0x000
#define BDM11_MD_YieldCtrl		          0x001
#define BDM11_MD_Contact		          0x002
#define BDM11_MD_AutoSkipUse		      0x003
#define BDM11_MD_TrayAssign               0x004
#define BDM11_MD_LotStartMode             0x005
#define BDM11_MD_VisionUse                0x006
#define BDM11_MD_AutoRetestSiteUse	      0x007
#define BDM11_MD_2D_BarCode_Use	          0x008
#define BDM11_MD_Event_Type			      0x009    // enum eRetestYieldFunc{
#define BDM11_MD_Retest_Yield		      0x00A
#define BDM11_MD_LoadTblArrived           0x00B    // 안착감지 센서
#define BDM11_MD_SocketClean              0x00C    // Socket Cleaning
#define BDM11_MD_CleanLoadTblArrived      0x00D    // Socket Cleaning 시 안착감지센서
#define BDM11_MD_AutoKill                 0x00E    // Auto Kill (Auto Socket Off)
#define BDM11_MD_ImpactTbl			      0x00F
									      
									      
#define BDM11_MD_CleanInitStart           0x010
#define BDM11_MD_CleanInitRetestStart     0x011
#define BDM11_MD_CleanSocketAlarm         0x012
#define BDM11_MD_CleanIntervalCnt         0x013
#define BDM11_MD_CleanFinish              0x014
#define BDM11_MD_CleanLowYield            0x015
#define  BDM11_MD_AutoKill_1st_Fail       0x016    // Auto Kill (Auto Socket Off)
									      
//#define BDM11_MD_Dvc_HorVerType           0x016
#define BDM11_MD_CleanDev_Event_Type      0x017
#define BDM11_MD_Consecutive_Fail_AutoKill 0x018    // Auto Kill (Auto Socket Off)
#define BDM11_MD_Consecutive_Fail_AutoKill_Station 0x019    // Auto Kill (Station All Site Fail)

									      
#define BDM11_MD_AutoGrrUse			      0x020
#define BDM11_MD_AutoGrrSupplyEnd	      0x021    // 0:Not End, 1:End (AutoGRR은 1개의 Tray로 Device투입 후에 더이상 투입되면 안된다)
#define BDM11_MD_AutoGrrDeviceSwapUse     0x022
#define BDM11_MD_AutoGrrAuditLoopUse      0x023


#define BDM11_MD_Retest_Skip_HW1          0x025
#define BDM11_MD_Retest_Skip_HW2          0x026
#define BDM11_MD_Retest_Skip_HW3          0x027
#define BDM11_MD_Retest_Skip_HW4          0x028

#define BDM11_MD_OFFLINEBIN_RANDOM		  0x029
#define BDM11_MD_OFFLINEBIN_YIELDCUT	  0x02A
#define BDM11_MD_AutoLoopUse			  0x02B
#define BDM11_MD_AutoAuditUse			  0x02C
#define BDM11_MD_AutoGrr2DIDSortUse       0x02D

#define BDM11_SocketOnOff_Site1		      0x030
#define BDM11_SocketOnOff_Site2		      0x031
#define BDM11_SocketOnOff_Site3		      0x032
#define BDM11_SocketOnOff_Site4		      0x033
#define BDM11_SocketOnOff_Site5		      0x034
#define BDM11_SocketOnOff_Site6		      0x035
#define BDM11_SocketOnOff_Site7		      0x036
#define BDM11_SocketOnOff_Site8		      0x037
#define BDM11_SocketOnOff_Site9		      0x038
#define BDM11_SocketOnOff_Site10	      0x039
#define BDM11_SocketOnOff_Site11	      0x03A
#define BDM11_SocketOnOff_Site12	      0x03B
#define BDM11_SocketOnOff_Site13	      0x03C
#define BDM11_SocketOnOff_Site14	      0x03D
#define BDM11_SocketOnOff_Site15	      0x03E
#define BDM11_SocketOnOff_Site16	      0x03F
									      
#define BDM11_SltPgm_BootTmOut	          0x040
#define BDM11_SltPgm_PreCooling           0x041 
#define BDM11_SltPgm_AcPwrOnDly           0x042 
#define BDM11_SltPgm_AcPwrOff             0x043 
#define BDM11_SltPgm_AtxPowerOn           0x044 
#define BDM11_SltPgm_PassStr              0x045 
#define BDM11_SltPgm_FailCodeHead         0x046 
#define BDM11_SltPgm_FailCodeDig          0x047 
#define BDM11_SltPgm_StartLine            0x048 
#define BDM11_SltPgm_EndLine              0x049 
#define BDM11_SltPgm_MinPassTm            0x04A 
#define BDM11_SltPgm_SendStart            0x04B 
#define BDM11_SltPgm_BaudRate             0x04C 
#define BDM11_SltPgm_HangReboot           0x04D 
#define BDM11_SltPgm_BiosBoot             0x04E 
#define BDM11_SltPgm_BiosS3               0x04F 
#define BDM11_SltPgm_TempMonitor          0x050 
#define BDM11_SltPgm_ExtentionPwrAGP      0x051
#define BDM11_SltPgm_SiteOff_TestTimeOut  0x052
#define BDM11_SltPgm_SiteOff_DefaultBin   0x053
#define BDM11_SltPgm_Sort_as_DefaultBin   0x054
									      
#define BDM11_MD_RuleAAB_VISION							0x060
#define BDM11_MD_NoDupCheck_2D							0x061
#define BDM11_MD_TestPP_DevExist_Check					0x062
#define BDM11_MD_FrontSW							    0x063
#define BDM11_MD_BatchFile_Use							0x064
#define BDM11_MD_DevExist_Sen_Check						0x065
#define BDM11_MD_ART_TEST_HAND_PICK_PLACE				(0x066)	// a->a' 에서 device pick/place할 건지 사용하는 옵션
#define BDM11_MD_TEST_HAND_PRE_VACUUM					(0x067)	// test hand z 축
#define BDM11_MD_TRAY_HAND_PRE_VACUUM					(0x068)	
#define BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN			(0x069)
#define BDM11_MD_TESTER_VERSION_CHECK_OFF				(0x06A)
#define BDM11_MD_TesterBatchFile_Use					(0x06B)
#define BDM11_MD_TestPP_After_DevExist_Check			(0x06C)
#define BDM11_MD_TEST_HAND_PNP_SITE_MOVE_ARRIVED_SEN	(0x06D)
#define BDM11_MD_TESTPP_STATION_FR_PNP					(0x06E)
#define BDM11_MD_2D_TEST_SITE_MOVE						(0x06F)


#define BDM11_MANU_CLN_REV_SITE1	   (0x070)
#define BDM11_MANU_CLN_REV_SITE2	   (0x071)
#define BDM11_MANU_CLN_REV_SITE3	   (0x072)
#define BDM11_MANU_CLN_REV_SITE4	   (0x073)
#define BDM11_MANU_CLN_REV_SITE5	   (0x074)
#define BDM11_MANU_CLN_REV_SITE6	   (0x075)
#define BDM11_MANU_CLN_REV_SITE7	   (0x076)
#define BDM11_MANU_CLN_REV_SITE8	   (0x077)
#define BDM11_MANU_CLN_REV_SITE9	   (0x078)
#define BDM11_MANU_CLN_REV_SITE10	   (0x079)
#define BDM11_MANU_CLN_REV_SITE11	   (0x07A)
#define BDM11_MANU_CLN_REV_SITE12	   (0x07B)
#define BDM11_MANU_CLN_REV_SITE13	   (0x07C)
#define BDM11_MANU_CLN_REV_SITE14	   (0x07D)
#define BDM11_MANU_CLN_REV_SITE15	   (0x07E)
#define BDM11_MANU_CLN_REV_SITE16	   (0x07F)

#define BDM11_PRS_DEVICE_ARRIVE_ENABLE (0x080) //0 : Disable 1: Enable
#define BDM11_MD_TESTER_SSH			   (0x081)	
#define BDM11_PRS_RECONTACT_ENABLE	   (0x082) //0 : Disable 1: Enable
#define BDM11_MD_AlwayBin1Sorting	   (0x083)

//Barcode 유효성 검사
#define BDM11_MD_DUPLICATION_OPT_USE	(0x084)
#define BDM11_MD_2D_LENGTH_OPT_USE		(0x085)
#define BDM11_MD_2D_MARK_OPT_USE		(0x086)

//VAT Mode
#define BDM11_VAT_MODE				   (0x090)
#define BDM11_VAT_OFFSET_MODE		   (0x091)
#define BDM11_VAT_ALARM				   (0x092)

#define BDM11_VAT_AUTO_MODE						(0X093)
#define	BDM11_VAT_MODE_Z_TEACHING				(0X094)
#define BDM11_VAT_MODE_VISION_AUTO_TEACHING		(0x095)
#define BDM11_VAT_MODE_CAM_SETTING				(0x096)
#define BDM11_VAT_X_PITCH_DOUBLE                (0x097)

//#define BDM11_BACKLASH_MODE				(0x098)
#define BDM11_VAT_VISION_PROTOCALL				(0x098)
//#define BDM11_VAT_RESTART_STATUS		(0x093)

#define BDM11_GUI_OPEN_HISTORY		   (0x100)
#define BDM11_TEST_SITE_RETOUCH		   (0x101)
#define BDM11_GUI_ALIGNMENT_OPEN_HISTORY		  (0x102)
#define BDM11_STATUS_OPEN_HISTORY		   (0x103)
#define BDM11_MD_ALL_UNIT_IONIZER            (0x104)



#define BDM11__MD_AutoGrr_ReContact (0x110)

#define BDM11_MD_LoadTbl_AfterMove_Arrived           (0x111)    // Table 이동 후, 안착감지 센서 check

#define BDM11_MD_TEST_LOAD_HAND_ALREADY_PICK		 (0x112)    // LOAD HAND Already Pick

//Picker Setting
#define BDM11_PICKER_LD_X1_Y1			(0x120)
#define BDM11_PICKER_LD_X2_Y1			(0x121)
#define BDM11_PICKER_LD_X3_Y1			(0x122)
#define BDM11_PICKER_LD_X4_Y1			(0x123)
#define BDM11_PICKER_LD_X1_Y2			(0x124)
#define BDM11_PICKER_LD_X2_Y2			(0x125)
#define BDM11_PICKER_LD_X3_Y2			(0x126)
#define BDM11_PICKER_LD_X4_Y2			(0x127)

#define BDM11_PICKER_UD_X1_Y1			(0x128)
#define BDM11_PICKER_UD_X2_Y1			(0x129)
#define BDM11_PICKER_UD_X3_Y1			(0x12A)
#define BDM11_PICKER_UD_X4_Y1			(0x12B)
#define BDM11_PICKER_UD_X1_Y2			(0x12C)
#define BDM11_PICKER_UD_X2_Y2			(0x12D)
#define BDM11_PICKER_UD_X3_Y2			(0x12E)
#define BDM11_PICKER_UD_X4_Y2			(0x12F)

//check Picker
#define BDM11_PKR_X1_Y1				(0x130)
#define BDM11_PKR_X2_Y1				(0x131)
#define BDM11_PKR_X3_Y1				(0x132)
#define BDM11_PKR_X4_Y1				(0x133)
#define BDM11_PKR_X1_Y2				(0x134)
#define BDM11_PKR_X2_Y2				(0x135)
#define BDM11_PKR_X3_Y2				(0x136)
#define BDM11_PKR_X4_Y2				(0x137)

#define BDM11_PKR_OPTION			(0x138)


#define BDM11_OS_RECENT_VER			(0x150)

#define BDM11_JAM_ONLINE_OFFLINE_VIEW	(0x160)

//============================================================BOOL <End>

//============================================================INTEGER <start>
#define NDM11_MD_Run				   0x000    // Online, offline
#define NDM11_SYS_LANGUAGE		       0x001    // 0:Eng, 1:Kor, 2:Chi, 3:Jap
#define NDM11_SYS_MD_DEVICE_CHK        0x002    // 0:Device Mode, 1:NoDeviceTrayExist, 2:NoDeviceNoTray

#define NDM11_AUTO_STATUS_ONECYCLE     (0x003)	// True : onecycle수행 , false : OneCycle 미수행
#define NDM11_AUTO_STATUS_CLEANOUT     (0x004)	// DEF_LOTEND_UNK, DEF_LOTEND_TRAYEND, DEF_LOTEND_TRAYFEED

#define NDM11_MD_TESTER_IF_PRO_AUDIT   (0x005)

#define NDM11_MULTIORIGIN_HISTORY		(0x00A)

#define NDM11_PICKER_HEIGHT_CAL_STATUS	(0x00B)

#define NDM11_LOADING_TABLE_2DID_LENGTH (0x02F)

#define NDM11_DvcContactForce_Pin	   (0x010)
#define NDM11_MD_FTRTQCContinue		   (0x011)
#define NDM11_MD_AutoRetest		       (0x012)    //enum eAUTO_RETEST_TEST_PP
#define NDM11_MD_AutoGrrTestCount	   (0x013)
#define NDM11_MD_AutoGrrUnitCount	   (0x014)
#define NDM11_AutoSkipCount            (0x015)
#define NDM11_QaSampleCount            (0x016)	// Lot :: QaSample Count
#define NDM11_MD_LOT_MODE			   (0x017)
#define NDM11_TM_OffLineTest		   (0x018)
#define NDM11_MD_AutoGrrMode		   (0x019)
#define NDM11_GRR_SOT_DELAY_TIME	   (0x01A)
#define NDM11_PARA_PER_STATION			(0x01B)

#define NDM11_PRS_DEVICE_ARRIVE_OPTION (0x020) //0 : All Socket 1 : Only Socket Open
#define NDM11_DropHeight			   (0x021)
#define NDM11_DVC_Dimen_X    		   (0x022)
#define NDM11_DVC_Dimen_Y			   (0x023)
#define NDM11_TM_ContactorDropTimer    (0x024)
#define NDM11_TEST_SITE_RETOUCH_DIS	   (0x025)
#define NDM11_PRESS_VIBRATOR_OPTION   (0x026)
#define NDM11_PRESS_VIBRATOR_RETRY_CNT   (0x027)

#define NDM11_PRESS_BLOW_POS_OPTION   (0x028)

#define NDM11_PRESS_CLEAN_OFFSET_OPTION  (0x029)

#define NDM11_LOADING_TABLE_VIBRATOR_OPTION   (0x02A)
#define NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT   (0x02B)

#define NDM11_PRESS_ALWAY_VIBRATOR_OPTION   (0x02C)
#define NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT   (0x02D)

#define NDM11_PRESS_RECONTACT_CNT   (0x02E)
#define NDM11_LOADING_TABLE_2DID_LENGTH (0x02F)


#define NDM11_TM_PRESS1_BLOW_WAIT      (0x030)
#define NDM11_TM_PRESS2_BLOW_WAIT      (0x031)
#define NDM11_TM_PRESS3_BLOW_WAIT      (0x032)
#define NDM11_TM_PRESS4_BLOW_WAIT      (0x033)
#define NDM11_TM_PRESS5_BLOW_WAIT      (0x034)
#define NDM11_TM_PRESS6_BLOW_WAIT      (0x035)
#define NDM11_TM_PRESS7_BLOW_WAIT      (0x036)
#define NDM11_TM_PRESS8_BLOW_WAIT      (0x037)
#define NDM11_TM_PRESS9_BLOW_WAIT      (0x038)
#define NDM11_TM_PRESS10_BLOW_WAIT     (0x039)
#define NDM11_TM_PRESS11_BLOW_WAIT     (0x03A)
#define NDM11_TM_PRESS12_BLOW_WAIT     (0x03B)
#define NDM11_TM_PRESS13_BLOW_WAIT     (0x03C)
#define NDM11_TM_PRESS14_BLOW_WAIT     (0x03D)
#define NDM11_TM_PRESS15_BLOW_WAIT     (0x03E)
#define NDM11_TM_PRESS16_BLOW_WAIT     (0x03F)

// Press Unit SPD/ACC/DCC
#define NDM11_SPD_PressUnit1		   (0x040)
#define NDM11_ACC_PressUnit1		   (0x041)
#define NDM11_DCC_PressUnit1		   (0x042)
#define NDM11_SPD_PressUnit2		   (0x043)
#define NDM11_ACC_PressUnit2		   (0x044)
#define NDM11_DCC_PressUnit2		   (0x045)
#define NDM11_SPD_PressUnit3		   (0x046)
#define NDM11_ACC_PressUnit3		   (0x047)
#define NDM11_DCC_PressUnit3		   (0x048)
#define NDM11_SPD_PressUnit4		   (0x049)
#define NDM11_ACC_PressUnit4		   (0x04A)
#define NDM11_DCC_PressUnit4		   (0x04B)
#define NDM11_SPD_PressUnit5		   (0x04C)
#define NDM11_ACC_PressUnit5		   (0x04D)
#define NDM11_DCC_PressUnit5		   (0x04E)
#define NDM11_SPD_PressUnit6		   (0x04F)
#define NDM11_ACC_PressUnit6		   (0x050)
#define NDM11_DCC_PressUnit6		   (0x051)
#define NDM11_SPD_PressUnit7		   (0x052)
#define NDM11_ACC_PressUnit7		   (0x053)
#define NDM11_DCC_PressUnit7		   (0x054)
#define NDM11_SPD_PressUnit8		   (0x055)
#define NDM11_ACC_PressUnit8		   (0x056)
#define NDM11_DCC_PressUnit8		   (0x057)
#define NDM11_SPD_PressUnit9		   (0x058)
#define NDM11_ACC_PressUnit9		   (0x059)
#define NDM11_DCC_PressUnit9		   (0x05A)
#define NDM11_SPD_PressUnit10		   (0x05B)
#define NDM11_ACC_PressUnit10		   (0x05C)
#define NDM11_DCC_PressUnit10		   (0x05D)
#define NDM11_SPD_PressUnit11		   (0x05E)
#define NDM11_ACC_PressUnit11		   (0x05F)
#define NDM11_DCC_PressUnit11		   (0x060)
#define NDM11_SPD_PressUnit12		   (0x061)
#define NDM11_ACC_PressUnit12		   (0x062)
#define NDM11_DCC_PressUnit12		   (0x063)
#define NDM11_SPD_PressUnit13		   (0x064)
#define NDM11_ACC_PressUnit13		   (0x065)
#define NDM11_DCC_PressUnit13		   (0x066)
#define NDM11_SPD_PressUnit14		   (0x067)
#define NDM11_ACC_PressUnit14		   (0x068)
#define NDM11_DCC_PressUnit14		   (0x069)
#define NDM11_SPD_PressUnit15		   (0x06A)
#define NDM11_ACC_PressUnit15		   (0x06B)
#define NDM11_DCC_PressUnit15		   (0x06C)
#define NDM11_SPD_PressUnit16		   (0x06D)
#define NDM11_ACC_PressUnit16		   (0x06E)
#define NDM11_DCC_PressUnit16		   (0x06F)
// Yield Monitoring
#define NDM11_Sample_Quantity		   (0x070)
#define NDM11_Preset_Counter		   (0x071)
#define NDM11_ContinueFail_FailCnt	   (0x072)
#define NDM11_ContinueFail_FailCnt_Station (0x073)
#define NDM11_1ST_FailCnt				 (0x074)
//===Socket Cleaning===//
#define NDM11_SockClnIntervalCnt       (0x080)
#define NDM11_SockClnContactTm         (0x081)
#define NDM11_SockClnContactCnt        (0x082)
#define NDM11_SockClnPiecesCnt         (0x083)
#define NDM11_SockClnAlarmCnt          (0x084)
#define NDM11_LotSocketCleanCount      (0x085)


#define NDM11_MD_AutoLoopTestCount	   (0x090)
#define NDM11_LOOP_SOT_DELAY_TIME	   (0x091)
#define NDM11_MD_AutoLoopMode		   (0x092)
#define NDM11_MD_AutoLoopUnitCount		(0x093)
#define NDM11_MD_AutoRePickPlaceCount	(0x094)
#define NDM11_MD_AutoLoopTestRecontactCount	(0x095)

#define NDM11_MD_AutoAuditTestCount	   (0x09A)
#define NDM11_Audit_SOT_DELAY_TIME	   (0x09B)
#define NDM11_MD_AutoAuditpMode		   (0x09C)
#define NDM11_MD_AutoAuditUnitCount		(0x09D)

#define NDM11_Theoretical_UPH		   (0x09E)
//VAT
#define NDM11_VAT_ALARM_IDX			   (0x09F)


// 128para
// Station 1  : 0x100 ~ 0x107 / Station 2  : 0x108 ~ 0x10F 
// Station 3  : 0x110 ~ 0x117 / Station 4  : 0x118 ~ 0x11F
// Station 5  : 0x120 ~ 0x127 / Station 6  : 0x128 ~ 0x12F 
// Station 7  : 0x130 ~ 0x137 / Station 8  : 0x138 ~ 0x13F
// Station 9  : 0x140 ~ 0x147 / Station 10 : 0x148 ~ 0x14F
// Station 11 : 0x150 ~ 0x157 / Station 12 : 0x158 ~ 0x15F
// Station 13 : 0x160 ~ 0x167 / Station 14 : 0x168 ~ 0x16F
// Station 15 : 0x170 ~ 0x177 / Station 16 : 0x178 ~ 0x17F
#define NDM11_GRR_SocketOnOff_Site1_1      (0x100)
#define NDM11_GRR_SocketOnOff_Site16_8     (0x17F)


// 128para
// Station 1  : 0x200 ~ 0x207 / Station 2  : 0x208 ~ 0x20F 
// Station 3  : 0x210 ~ 0x217 / Station 4  : 0x218 ~ 0x21F
// Station 5  : 0x220 ~ 0x227 / Station 6  : 0x228 ~ 0x22F 
// Station 7  : 0x230 ~ 0x237 / Station 8  : 0x238 ~ 0x23F
// Station 9  : 0x240 ~ 0x247 / Station 10 : 0x248 ~ 0x24F
// Station 11 : 0x250 ~ 0x257 / Station 12 : 0x258 ~ 0x25F
// Station 13 : 0x260 ~ 0x267 / Station 14 : 0x268 ~ 0x26F
// Station 15 : 0x270 ~ 0x277 / Station 16 : 0x278 ~ 0x27F
#define NDM11_SocketOnOff_Site1_1      (0x200)
#define NDM11_SocketOnOff_Site16_8     (0x27F)


// 128para
// Station 1  : 0x280 ~ 0x287 / Station 2  : 0x288 ~ 0x29F 
// Station 3  : 0x290 ~ 0x297 / Station 4  : 0x3A8 ~ 0x3AF
// Station 5  : 0x320 ~ 0x327 / Station 6  : 0x328 ~ 0x32F 
// Station 7  : 0x330 ~ 0x337 / Station 8  : 0x338 ~ 0x33F
// Station 9  : 0x340 ~ 0x347 / Station 10 : 0x348 ~ 0x34F
// Station 11 : 0x350 ~ 0x357 / Station 12 : 0x358 ~ 0x35F
// Station 13 : 0x360 ~ 0x367 / Station 14 : 0x368 ~ 0x36F
// Station 15 : 0x370 ~ 0x377 / Station 16 : 0x378 ~ 0x3A8
#define NDM11_Audit_SocketOnOff_Site1_1      (0x280)
#define NDM11_Audit_SocketOnOff_Site16_8     (0x3A8)

// SltPgm Setting
#define NDM11_SltPgm_SotDelayTm			0x39F
#define NDM11_SltPgm_TestTmOut          0x340
#define NDM11_SltPgm_FailCodeDigitalNo  0x341
#define NDM11_SltPgm_PreCooling         0x342
#define NDM11_SltPgm_AcPowerOn          0x343
#define NDM11_SltPgm_AcPowerOff         0x344
#define NDM11_SltPgm_AtxPowerOn         0x345
#define NDM11_SltPgm_MinPassTm          0x346
#define NDM11_SltPgm_BaudRate           0x347
#define NDM11_SltPgm_BiosBootCnt        0x348
#define NDM11_SltPgm_BiosTmOut          0x349
#define NDM11_SltPgm_BiosS3Cnt          0x34A
#define NDM11_SltPgm_S3TmOut            0x34B
#define NDM11_SltPgm_BiosS3DelayTm      0x34C
#define NDM11_SltPgm_SetTemp            0x34D
#define NDM11_SltPgm_TempGuardBand      0x34E
#define NDM11_SltPgm_AgpDelayTm         0x34F //Extention power
#define NDM11_SltPgm_NotTestingTmOut    0x350
#define NDM11_PRESS_CONTACT_2ST_SPEED	0x351

#define NDM11_2D_MARK_START_INDEX		0x352
#define NDM11_NUMBER_OF_LOADTABLE		0x353

//Tray Assign
#define NDM11_TRAY_ASSIGN_LOAD1_TOP       0x360
#define NDM11_TRAY_ASSIGN_LOAD2_TOP       0x361
#define NDM11_TRAY_ASSIGN_UNLOAD1_TOP     0x362
#define NDM11_TRAY_ASSIGN_UNLOAD2_TOP	  0x363
#define NDM11_TRAY_ASSIGN_UNLOAD3_TOP     0x364
#define NDM11_TRAY_ASSIGN_UNLOAD4_TOP     0x365
#define NDM11_TRAY_ASSIGN_UNLOAD5_TOP     0x366
#define NDM11_TRAY_ASSIGN_LOAD1_BOTTOM    0x367
#define NDM11_TRAY_ASSIGN_LOAD2_BOTTOM    0x368
#define NDM11_TRAY_ASSIGN_UNLOAD1_BOTTOM  0x369
#define NDM11_TRAY_ASSIGN_UNLOAD2_BOTTOM  0x36A
#define NDM11_TRAY_ASSIGN_UNLOAD3_BOTTOM  0x36B
#define NDM11_TRAY_ASSIGN_UNLOAD4_BOTTOM  0x36C
#define NDM11_TRAY_ASSIGN_UNLOAD5_BOTTOM  0x36D


#define NDM11_VAT_POS_Stacker_1			0x370
#define NDM11_VAT_POS_Stacker_2			0x371
#define NDM11_VAT_POS_Stacker_3			0x372
#define NDM11_VAT_POS_Stacker_4			0x373
#define NDM11_VAT_POS_Stacker_5			0x374
#define NDM11_VAT_POS_Stacker_6			0x375
#define NDM11_VAT_POS_Stacker_7			0x376
#define NDM11_VAT_POS_CleanBuf_1		0x377
#define NDM11_VAT_POS_CleanBuf_2		0x378
#define NDM11_VAT_POS_ARTBuf_1			0x379
#define NDM11_VAT_POS_ARTBuf_2			0x37A
#define NDM11_VAT_POS_TrayLoadTbl_1			0x37B
#define NDM11_VAT_POS_TrayLoadTbl_2			0x37C
#define NDM11_VAT_POS_Station_1			0x37D
#define NDM11_VAT_POS_Station_2			0x37E
#define NDM11_VAT_POS_Station_3			0x37F
#define NDM11_VAT_POS_Station_4			0x380
#define NDM11_VAT_POS_Station_5			0x381
#define NDM11_VAT_POS_Station_6			0x382
#define NDM11_VAT_POS_Station_8			0x383
#define NDM11_VAT_POS_Station_9			0x384
#define NDM11_VAT_POS_Station_7			0x385
#define NDM11_VAT_POS_Station_10		0x386
#define NDM11_VAT_POS_Station_11		0x387
#define NDM11_VAT_POS_Station_12		0x388
#define NDM11_VAT_POS_Station_13		0x389
#define NDM11_VAT_POS_Station_14		0x38A
#define NDM11_VAT_POS_Station_15		0x38B
#define NDM11_VAT_POS_Station_16		0x38C
#define NDM11_VAT_POS_TrayLowerCam		0x38D
#define NDM11_VAT_POS_TestLowerCam1		0x38E
#define NDM11_VAT_POS_TestLowerCam2		0x38F
#define NDM11_VAT_POS_TestLoadTbl_1			0x390
#define NDM11_VAT_POS_TestLoadTbl_2			0x391

//============================================================INTEGER <end>

//============================================================  DOUBLE <start>
#define	DDM11_Low_Yield				   0x000	// Yield Monitoring
#define DDM11_DvcContactForce_GF	   0x010    // Pin당 Force
#define DDM11_ContactForce_gf          0x011    // 계산치
// #define DDM11_SockClnForcePinGf        0x012    // pin당 Force
// #define DDM11_SockClnContacForce       0x013    // 계산치
#define DDM11_SockClnLowYield          0x014
#define DDM11_VarianceYield            0x00A
#define DDM11_OffLine_YieldCutPer	   0x00F

#define DDM11_PRESS_VIBRATOR_DELAY_TIME				(0x015)

#define DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME		(0x016)
#define DDM11_LOADING_TABLE_WAIT_TIME				(0x017)

#define DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME		(0x018)

#define DDM11_TM_PRESS1_BLOW_POS      (0x020)
#define DDM11_TM_PRESS2_BLOW_POS      (0x021)
#define DDM11_TM_PRESS3_BLOW_POS      (0x022)
#define DDM11_TM_PRESS4_BLOW_POS      (0x023)
#define DDM11_TM_PRESS5_BLOW_POS      (0x024)
#define DDM11_TM_PRESS6_BLOW_POS      (0x025)
#define DDM11_TM_PRESS7_BLOW_POS      (0x026)
#define DDM11_TM_PRESS8_BLOW_POS      (0x027)
#define DDM11_TM_PRESS9_BLOW_POS      (0x028)
#define DDM11_TM_PRESS10_BLOW_POS     (0x029)
#define DDM11_TM_PRESS11_BLOW_POS     (0x02A)
#define DDM11_TM_PRESS12_BLOW_POS     (0x02B)
#define DDM11_TM_PRESS13_BLOW_POS     (0x02C)
#define DDM11_TM_PRESS14_BLOW_POS     (0x02D)
#define DDM11_TM_PRESS15_BLOW_POS     (0x02E)
#define DDM11_TM_PRESS16_BLOW_POS     (0x02F)

#define DDM11_TM_PRESS1_CLEAN_OFFSET      (0x030)
#define DDM11_TM_PRESS2_CLEAN_OFFSET      (0x031)
#define DDM11_TM_PRESS3_CLEAN_OFFSET      (0x032)
#define DDM11_TM_PRESS4_CLEAN_OFFSET      (0x033)
#define DDM11_TM_PRESS5_CLEAN_OFFSET      (0x034)
#define DDM11_TM_PRESS6_CLEAN_OFFSET      (0x035)
#define DDM11_TM_PRESS7_CLEAN_OFFSET      (0x036)
#define DDM11_TM_PRESS8_CLEAN_OFFSET      (0x037)
#define DDM11_TM_PRESS9_CLEAN_OFFSET      (0x038)
#define DDM11_TM_PRESS10_CLEAN_OFFSET     (0x039)
#define DDM11_TM_PRESS11_CLEAN_OFFSET     (0x03A)
#define DDM11_TM_PRESS12_CLEAN_OFFSET     (0x03B)
#define DDM11_TM_PRESS13_CLEAN_OFFSET     (0x03C)
#define DDM11_TM_PRESS14_CLEAN_OFFSET     (0x03D)
#define DDM11_TM_PRESS15_CLEAN_OFFSET     (0x03E)
#define DDM11_TM_PRESS16_CLEAN_OFFSET     (0x03F)

#define DDM11_CurPressure1			   (0x040)
#define DDM11_CurPressure2			   (0x041)

#define DDM11_SockClnContactDelayTime		 (0x042)

#define DDM11_PRESS_CLEANING_2ND_Z_OFFSET	(0x043)

//VAT Picker Align
// LD
#define DDM11_PKR_LD_X0_Y0_OFFSETX			(0x050)
#define DDM11_PKR_LD_X0_Y0_OFFSETY			(0x051)
#define DDM11_PKR_LD_X1_Y0_OFFSETX			(0x052)
#define DDM11_PKR_LD_X1_Y0_OFFSETY			(0x053)
#define DDM11_PKR_LD_X2_Y0_OFFSETX			(0x054)
#define DDM11_PKR_LD_X2_Y0_OFFSETY			(0x055)
#define DDM11_PKR_LD_X3_Y0_OFFSETX			(0x056)
#define DDM11_PKR_LD_X3_Y0_OFFSETY			(0x057)
#define DDM11_PKR_LD_X0_Y1_OFFSETX			(0x058)
#define DDM11_PKR_LD_X0_Y1_OFFSETY			(0x059)
#define DDM11_PKR_LD_X1_Y1_OFFSETX			(0x05A)
#define DDM11_PKR_LD_X1_Y1_OFFSETY			(0x05B)
#define DDM11_PKR_LD_X2_Y1_OFFSETX			(0x05C)
#define DDM11_PKR_LD_X2_Y1_OFFSETY			(0x05D)
#define DDM11_PKR_LD_X3_Y1_OFFSETX			(0x05E)
#define DDM11_PKR_LD_X3_Y1_OFFSETY			(0x05F)

// UD
#define DDM11_PKR_UD_X0_Y0_OFFSETX			(0x060)
#define DDM11_PKR_UD_X0_Y0_OFFSETY			(0x061)
#define DDM11_PKR_UD_X1_Y0_OFFSETX			(0x062)
#define DDM11_PKR_UD_X1_Y0_OFFSETY			(0x063)
#define DDM11_PKR_UD_X2_Y0_OFFSETX			(0x064)
#define DDM11_PKR_UD_X2_Y0_OFFSETY			(0x065)
#define DDM11_PKR_UD_X3_Y0_OFFSETX			(0x066)
#define DDM11_PKR_UD_X3_Y0_OFFSETY			(0x067)
#define DDM11_PKR_UD_X0_Y1_OFFSETX			(0x068)
#define DDM11_PKR_UD_X0_Y1_OFFSETY			(0x069)
#define DDM11_PKR_UD_X1_Y1_OFFSETX			(0x06A)
#define DDM11_PKR_UD_X1_Y1_OFFSETY			(0x06B)
#define DDM11_PKR_UD_X2_Y1_OFFSETX			(0x06C)
#define DDM11_PKR_UD_X2_Y1_OFFSETY			(0x06D)
#define DDM11_PKR_UD_X3_Y1_OFFSETX			(0x06E)
#define DDM11_PKR_UD_X3_Y1_OFFSETY			(0x06F)

#define DDM11_OEE							(0x070)
#define DDM11_CYLINDER_DELAY_TIME			(0x071)

#define DDM11_TESTPP_DELAY_TIME				(0x073)
#define	DDM11_PRESS_RECONTACT_OFFSET		(0x074)
#define	DDM11_PRESS_RECONTACT_DELAY_TIME	(0x075)
#define DDM11_PRESS_CONTACT_POS_OFFSET		(0x076)
#define DDM11_PRESS_SOCKET_CLEAN_BLOW_POS	(0x077)
#define DDM11_PRESS_SOCKET_CLEAN_BLOW_TIME	(0x078)
//============================================================  DOUBLE <end>

//============================================================STRING <start>
#define	SDM11_CustomerName			   0x000
#define	SDM11_DeviceName			   0x001
#define	SDM11_ProgramName			   0x002
#define	SDM11_TrayFileName			   0x003
#define	SDM11_SLT_ProgramName		   0x004
#define	SDM11_CategoryName			   0x005
#define	SDM11_LotNo					   0x006
#define	SDM11_ScheduleNo			   0x007

#define	SDM11_OperatorID			   0x009
#define	SDM11_REC_LotNo				   0x00A
#define	SDM11_REC_ScheduleNo		   0x00B
#define SDM11_REC_FTRTQC			   0x00C
#define	SDM11_REC_OperatorID		   0x00D
//#define SDM11_SltPgmType			   0x00E
						   
									   
#define	SDM11_PowerOnTm				   0x010
#define	SDM11_RunTm					   0x011
#define	SDM11_NrmStopTm				   0x012
#define	SDM11_Lv1AlarmStopTm		   0x013
#define	SDM11_Lv2AlarmStopTm		   0x014
#define	SDM11_Lv3AlarmStopTm		   0x015
#define	SDM11_OtherStopTm			   0x016

// SltPgm
#define	SDM11_SltPgm_PassStr           0x020    // char size 128
#define	SDM11_SltPgm_FailCodeHeader    0x021    // char size 128
#define	SDM11_SltPgm_StartLine         0x022    // char size 32
#define	SDM11_SltPgm_EndLine           0x023    // char size 32

//File Path 
#define SDM11_SummaryFile_Path         0x030
#define SDM11_BatchFile_Path           0x031
#define SDM11_LotInfoFile_Path		   0x032
#define SDM11_2DID_Sort_FilePath       0x033
#define SDM11_Tester_BatchFile_Path    0x034
#define SDM11_NetWorkSummaryFile_Path   0x035
#define SDM11_2did_Csv_File_Path		0x036
#define SDM11_Hit_Summary_File_Path		0x037
//Lot Info - Amkor 
#define SDM11_Company                 0x040
#define SDM11_Operation                0x041
#define SDM11_Temp                     0x042
#define SDM11_RetestCode               0x043
#define SDM11_QTY                      0x044
#define SDM11_ART                      0x045
#define SDM11_Soak_Time                0x046
#define SDM11_FUSE                     0x047
#define SDM11_HANDLER                  0x048
#define SDM11_LOAD_BD_NO               0x049
#define SDM11_SOCKER_ID                0x04A
#define SDM11_C_KIT_ID                 0x04B
#define SDM11_FOUNDRY_LOTID            0x04C
#define SDM11_Assembly_startdate       0x04D
#define SDM11_Assembly_enddate         0x04E
#define SDM11_ASSY_vendor_lotid        0x04F
#define SDM11_TEST_vendor_lotid        0x050
#define SDM11_BOT_SUB_Vendor           0x051
#define SDM11_Nickname                 0x052
#define SDM11_TP_REV				   0x053
#define SDM11_UDP_HOST_NAME			   0x054
#define SDM11_DEFAULT_QA_QTY           0x060

#define SDM11_Socket_Diagnosis_Site1_1      (0x200)
#define SDM11_Socket_Diagnosis_Site16_8     (0x27F)

#define SDM11_2D_DEVICE_NAME				(0x280)
#define SDM11_2D_MARK_LIST					(0x281)
#define SDM11_2D_MARK_IGNORE_REL_TEST		(0x282)

//Lot Info - USI
//#define SDM11_Theoretical_UPH		   0x060 	
//#define SDM11_OEE					   0x061	
//============================================================STRING <end>