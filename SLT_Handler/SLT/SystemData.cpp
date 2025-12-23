#include "StdAfx.h"
#include "SystemData.h"

CSystemData::CSystemData(void)
{
	ZeroMemory(m_szCokName, sizeof(m_szCokName));
}

CSystemData::~CSystemData(void)
{

}

int CSystemData::UpdateFileDataToSM()/*Initialize ÈÄ »ç¿ë*/
{
	strcpy_s(m_SltData.szLastPackageName,sizeof(m_SltData.szLastPackageName), m_szCokName);
	g_COK_FileMgr.LoadSltSysFile(m_SltData);

	UpdateBasicDataToSM(m_SltData.lpstDataBasic);
	UpdateCokDataToSM(m_SltData.lpstDataCOK);

    //g_COK_FileMgr.LoadCategoryInfo(m_SltData.lpstDataBasic.szCategoryName, m_SltData.lpstDataCategory);
    //g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgmType, m_SltData.lpstDataBasic.szSltPgmName, sizeof(m_SltData.lpstDataBasic.szSltPgmName));

	UpdateSltPgmDataToSM(m_SltData.lpstDataSltPgm);
	//FTP File Path
	UpdateFTPFileToSM();


	char szPathTemp[512]={0,};
	char szPathFileName[512]={0,};
	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigPathData(szPathFileName);

	int nSaveData = ConfigPathData.GetInt("HDD","INTERLOCK", "20");
	g_DMCont.m_dmEQP.SN(NDM0_HDD_SIZE_INTERLOCK, nSaveData);
	nSaveData = ConfigPathData.GetInt("HDD","ALARM", "10");
	g_DMCont.m_dmEQP.SN(NDM0_HDD_SIZE_ALARM, nSaveData);	

	//int nParaType = g_DMCont.m_dmHandlerOpt.GN(NDM11_Tester_Para);//
	//if( nParaType == eTester_64Para ){
	//	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_SocketClean , eOPTION_NOT_USE );
	//}
    
	return 0;
}

void CSystemData::Initialize(char* szCokName)
{
	strcpy_s(m_szCokName,sizeof(m_szCokName), szCokName);
}

void CSystemData::Finalize()
{
	ZeroMemory(m_szCokName, sizeof(m_szCokName));
}

void CSystemData::UpdateBasicDataToSM(ST_BASIC_INFO lpstDataBasic)
{
    // function
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoSkipUse			, lpstDataBasic.bAutoSkipMode     );
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Contact				, lpstDataBasic.bContactMode      );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DropHeight			    , lpstDataBasic.nDropHeight           );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_ContactorDropTimer	, lpstDataBasic.nDropTimer        );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_Run					, lpstDataBasic.nRunMode          );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_OffLineTest			, lpstDataBasic.nOffLineTestTm    );
	g_DMCont.m_dmHandlerOpt.SN(BDM11_MD_VisionUse           , lpstDataBasic.nVision);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_AutoSkipCount          , lpstDataBasic.nAutoSkipCount);

	// Vibrator
	g_DMCont.m_dmSetplate.SN(NDM6_VIBRATOR_ACT_TIMES, lpstDataBasic.nVibActCnt);
	g_DMCont.m_dmSetplate.SD(DDM6_VIBRATOR_ACT_DELAY_SEC, lpstDataBasic.dVibActDelayTime);
	g_DMCont.m_dmSetplate.SN(NDM6_CNT_PLACE_DVC_TRAY_FOR_VIBRATION_ACT, lpstDataBasic.nVibUdDvcActCount);

	// Yield Monitor
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_YieldCtrl        ,	lpstDataBasic.bYieldMonitor);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Event_Type       ,	lpstDataBasic.bEventType);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Yield     ,	lpstDataBasic.bRetestYield);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_Sample_Quantity     ,	lpstDataBasic.nSampleQuantity);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_Preset_Counter      ,	lpstDataBasic.nPresetCounter);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ContinueFail_FailCnt,	lpstDataBasic.nContinueFailureCnt);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_Low_Yield           ,	lpstDataBasic.dLowYield);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_VarianceYield       ,  lpstDataBasic.dVariance);
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LoadTblArrived   ,  lpstDataBasic.bLoadTblArrived);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LoadTbl_AfterMove_Arrived, lpstDataBasic.bLoadTblAfterMoveArrived);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_ImpactTbl        ,  lpstDataBasic.bImpactTbl);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ContinueFail_FailCnt_Station, lpstDataBasic.nContinueFailureCnt_Station);

    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill         ,  lpstDataBasic.bMD_Autokill);			// Auto socket off (auto kill)
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill, lpstDataBasic.bMD_Consecutive_Fail_AutoKill);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill_Station, lpstDataBasic.bMD_Consecutive_Fail_AutoKill_Station);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill_1st_Fail, lpstDataBasic.bMD_1stFailAutokill);
	
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_RuleAAB_VISION   ,  lpstDataBasic.bMD_VisionExist_AAB);		// vision test site exist check from aab(a->a)
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TestPP_DevExist_Check, lpstDataBasic.bMD_TestPP_DevExist_Check);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TestPP_After_DevExist_Check, lpstDataBasic.bMD_TestPP_After_DevExist_Check);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_DevExist_Sen_Check, lpstDataBasic.bMD_TestPP_DevExist_Check_Sensor);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_ART_TEST_HAND_PICK_PLACE, lpstDataBasic.bMD_TestPP_ArtDevice_PickPlace);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PRE_VACUUM, lpstDataBasic.bMD_TestPP_PreVacuum);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TRAY_HAND_PRE_VACUUM, lpstDataBasic.bMD_TrayPP_PreVacuum);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN, lpstDataBasic.bMD_TestPP_PNP_SiteArrived_Sen);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PNP_SITE_MOVE_ARRIVED_SEN, lpstDataBasic.bMD_TestPP_PNP_SiteMoveArrived_Sen);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TESTPP_STATION_FR_PNP, lpstDataBasic.bMD_TestppStationFR_PNP);
	
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_LOAD_HAND_ALREADY_PICK, lpstDataBasic.bMD_LoadHandAlreadyPick);
	

	g_DMCont.m_dmHandlerOpt.SN(NDM11_AutoSkipCount   , lpstDataBasic.nAutoSkipCount);

	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_BarcodeMaxRetry, lpstDataBasic.nBarcodeRetryCnt);
	
	

	g_DMCont.m_dmHandlerOpt.SB(BDM11_PRS_DEVICE_ARRIVE_ENABLE, lpstDataBasic.bPressDeviceChk); //0 : Disable 1: Enable
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRS_DEVICE_ARRIVE_OPTION, lpstDataBasic.nPressDeviceChkOpt); //0 : All Socket 1 : Only Socket Open

	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_OPTION, lpstDataBasic.nPressVibratorChkOpt); //1 : USE 0 : NOT_USE
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_RETRY_CNT, lpstDataBasic.nPressVibratorRetryCnt);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_VIBRATOR_DELAY_TIME, lpstDataBasic.dPressVibratorDelayTime);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_ALWAY_VIBRATOR_OPTION, lpstDataBasic.nPressAlwayVibratorChkOpt); //1 : USE 0 : NOT_USE
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT, lpstDataBasic.nPressAlwayVibratorRetryCnt);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME, lpstDataBasic.dPressAlwayVibratorDelayTime);
// 
// 	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_OPTION, lpstDataBasic.nLoadTableVibratorChkOpt); //1 : USE 0 : NOT_USE
// 	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT, lpstDataBasic.nLoadTableVibratorRetryCnt);
// 	g_DMCont.m_dmHandlerOpt.SD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME, lpstDataBasic.dLoadTableVibratorDelayTime);

	//int nProjectName = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
    // socket On/Off
	for( int i = 0; i < eMaxPressUnitCount; i++ ) {
		for( int j = 0; j < 2; j++ ) {
			for( int k = 0; k < 4; k++ ) {
//				g_DMCont.m_dmHandlerOpt.SN( NDM11_SocketOnOff_Site1_1 + ( i*STATION_MAX_Y_SITE*STATION_MAX_X_SITE ) + ( j * STATION_MAX_X_SITE ) + k, lpstDataBasic.nSiteOnOff[ i ][ j ][ k ] );

				if(nTestSite_Div_Y == 1)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (i <= ePressUnit_Down_4)
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + abs(k - 3) + j * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
						else
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j) * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
					}
					else {
						if (i <= ePressUnit_Down_4) {
							int ntese = NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j - 1) * 4;
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j) * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
						}
						else
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + abs(k - 3) + j * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
					}
				}
				else
				{
					if (nBoardType == eBoardDir_Forward) {
						if (i <= ePressUnit_Down_4)
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + abs(k - 3) + j * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
						else
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j - 1) * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
					}
					else {
						if (i <= ePressUnit_Down_4) {
							int ntest = NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_Y_SITE*STATION_MAX_X_SITE) + (j * STATION_MAX_X_SITE) + k;
							int ntese = NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j - 1) * 4;
							
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + k + abs(j - 1) * 4, lpstDataBasic.nSiteOnOff[i][k][j]);
						}
						else
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_PARA) + abs(k - 3) + j * 4, lpstDataBasic.nSiteOnOff[i][k][j]);

						//g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (i*STATION_MAX_Y_SITE*STATION_MAX_X_SITE) + (j * STATION_MAX_X_SITE) + k, lpstDataBasic.nSiteOnOff[i][j][k]);
					}
				}
			
			}
		}
	}

    // LOT Information
	char szCompany[16] ={0,};
	CString strCustomer =_T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company,szCompany,sizeof(szCompany));
	strCustomer = szCompany;
	strCustomer.MakeUpper();
	if(strCustomer.Compare("SCK") == 0){
		g_DMCont.m_dmHandlerOpt.SS(SDM11_OperatorID       , lpstDataBasic.szOperID     , strlen(lpstDataBasic.szOperID)    );
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ScheduleNo       , lpstDataBasic.szScheduleNo , strlen(lpstDataBasic.szScheduleNo));
		g_DMCont.m_dmHandlerOpt.SS(SDM11_LotNo            , lpstDataBasic.szLotNo      , strlen(lpstDataBasic.szLotNo)     );
		g_DMCont.m_dmHandlerOpt.SS(SDM11_CustomerName    , lpstDataBasic.szCustomerName, strlen(lpstDataBasic.szCustomerName));
		g_DMCont.m_dmHandlerOpt.SS(SDM11_DeviceName      , lpstDataBasic.szDeviceName  , strlen(lpstDataBasic.szDeviceName)  );
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ProgramName     , lpstDataBasic.szPgmName     , strlen(lpstDataBasic.szPgmName)     );
	}
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LotStartMode  , lpstDataBasic.nStartMode                                       );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, lpstDataBasic.nFTRTQC                                          );
   
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TrayFileName	 , m_szCokName				   , strlen(m_szCokName)                 );
	g_DMCont.m_dmHandlerOpt.SS(SDM11_SLT_ProgramName , lpstDataBasic.szSltPgmName  , strlen(lpstDataBasic.szSltPgmName)  );
	g_DMCont.m_dmHandlerOpt.SS(SDM11_CategoryName	 , lpstDataBasic.szCategoryName, strlen(lpstDataBasic.szCategoryName));

    // contact force
	g_DMCont.m_dmHandlerOpt.SD(DDM11_DvcContactForce_GF		, lpstDataBasic.dContactcForceGF  );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DvcContactForce_Pin	, lpstDataBasic.nContactForcePins );
	double dForce = lpstDataBasic.dContactcForceGF * (double)lpstDataBasic.nContactForcePins;
	g_DMCont.m_dmHandlerOpt.SD(DDM11_ContactForce_gf		, dForce                          );

    // device diemnsion
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DVC_Dimen_X		, lpstDataBasic.nDvcDimensionX  );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DVC_Dimen_Y		, lpstDataBasic.nDvcDimensionY  );

    // socket cleaning
//	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_SocketClean, lpstDataBasic.bMD_SocketClean);
// 	}
// 	else {
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_SocketClean, FALSE);
// 	}
	
	// ReTouch Option & Vibrator Option

	// Vibrator Option
		char szPathFileName[MAX_PATH] = { 0, };
		char szPathTemp[512] = { 0, };
		char szFilePath[MAX_PATH] = { 0, };

		::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
		sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_FILE_PATH);
		CConfigData ConfigPathData(szPathFileName);

		char szCompanyVibratorMode[64] = { 0, };
		CString strTemp = _T("");
		sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
		CConfigData ConfigVibMode(szPathFileName);
		ConfigVibMode.GetString("Mode", "Vibrator Mode", DEF_NOT_USE, szCompanyVibratorMode);


	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_OPTION, lpstDataBasic.nPressVibratorChkOpt);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_RETRY_CNT, lpstDataBasic.nPressVibratorRetryCnt);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_VIBRATOR_DELAY_TIME, lpstDataBasic.dPressVibratorDelayTime);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_OPTION, lpstDataBasic.nLoadTableVibratorChkOpt); //1 : USE 0 : NOT_USE
	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT, lpstDataBasic.nLoadTableVibratorRetryCnt);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME, lpstDataBasic.dLoadTableVibratorDelayTime);

	g_DMCont.m_dmHandlerOpt.SD(DDM11_LOADING_TABLE_WAIT_TIME, lpstDataBasic.dLoadTableWaitTime);
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_NUMBER_OF_LOADTABLE, lpstDataBasic.nNumberOfLoadTable);
// 	else 
// 	{
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_TEST_SITE_RETOUCH, FALSE);
// 		g_DMCont.m_dmHandlerOpt.SN(NDM11_TEST_SITE_RETOUCH_DIS, 0);
// 		g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_OPTION, FALSE);  //0 = Not Use
// 	}

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanLoadTblArrived , lpstDataBasic.bMD_CleanLoadTblArrivedSen );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnContactTm       , lpstDataBasic.nCleanContacTm             );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnContactCnt      , lpstDataBasic.nCleanContacCnt            );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnAlarmCnt        , lpstDataBasic.nCleanAlarmCnt             );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnPiecesCnt       , lpstDataBasic.nCleanPiecesCnt            );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnIntervalCnt     , lpstDataBasic.nCleanIntervalCnt          );
	g_DMCont.m_dmHandlerOpt.SD(DDM11_SockClnLowYield        , lpstDataBasic.dCleanLowYield             );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanDev_Event_Type , lpstDataBasic.bMD_EventType              );

    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanInitStart      , lpstDataBasic.bMD_CleanInitStart      );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanInitRetestStart, lpstDataBasic.bMD_CleanInitRetestStart);
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanIntervalCnt    , lpstDataBasic.bMD_CleanIntervalCnt    );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanLowYield       , lpstDataBasic.bMD_CleanLowYield       );

	// GRR
	g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_AutoGrrTestCount, lpstDataBasic.nAutoGrrTestCount );
	g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_AutoGrrUnitCount, lpstDataBasic.nAutoGrrUnitCount );

	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoGrrMode, lpstDataBasic.nAutoGrrMode);
	
	g_DMCont.m_dmHandlerOpt.SN( NDM11_GRR_SOT_DELAY_TIME, lpstDataBasic.nGrrSotDelay);

	g_DMCont.m_dmHandlerOpt.SB( BDM11_MD_AutoGrrDeviceSwapUse, lpstDataBasic.bGRR_Device_swap );
	g_DMCont.m_dmHandlerOpt.SB( BDM11_MD_AutoGrrAuditLoopUse, lpstDataBasic.bGRR_Audit_loop );

	for( int i = 0; i < eMaxPressUnitCount; i++ ) {
		for( int j = 0; j < STATION_MAX_Y_SITE; j++ ) {
			for( int k = 0; k < STATION_MAX_X_SITE; k++ ) {
				g_DMCont.m_dmHandlerOpt.SN( NDM11_GRR_SocketOnOff_Site1_1 + ( i*STATION_MAX_Y_SITE*STATION_MAX_X_SITE ) + ( j * STATION_MAX_X_SITE ) + k, lpstDataBasic.nGRRSiteOnOff[ i ][ j ][ k ] );
				
			}
		}
	}


// 	for (int i = 0; i < eMaxPressUnitCount; i++) {
// 		for (int j = 0; j < STATION_MAX_Y_SITE; j++) {
// 			for (int k = 0; k < STATION_MAX_X_SITE; k++) {
// 				g_DMCont.m_dmHandlerOpt.SN(NDM11_Audit_SocketOnOff_Site1_1 + (i*STATION_MAX_Y_SITE*STATION_MAX_X_SITE) + (j * STATION_MAX_X_SITE) + k, lpstDataBasic.nAuditSiteOnOff[i][j][k]);
// 			}
// 		}
// 	}

}

void CSystemData::UpdateCokDataToSM(ST_COK_INFO lpstDataCOK)
{
	g_DMCont.m_dmCTrayXfer.SN(NDM9_SPD_Transfer_X		, lpstDataCOK.aTransfer.aAxisForm[eAxis_X].nSPD);
	g_DMCont.m_dmCTrayXfer.SN(NDM9_ACC_Transfer_X		, lpstDataCOK.aTransfer.aAxisForm[eAxis_X].nACC);
	g_DMCont.m_dmCTrayXfer.SN(NDM9_DCC_Transfer_X		, lpstDataCOK.aTransfer.aAxisForm[eAxis_X].nDCC);


	int nTrayPPNumSM[MAX_TRAY_PP_CNT] = { NDM5_SPD_LD_TrayPP_X, NDM5_SPD_UL_TrayPP_X };
	for (int i = RIDX_TRAY_PP_1; i <= RIDX_TRAY_PP_2; i++)
	{
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i], lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X].nSPD);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+1, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X].nACC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+2, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X].nDCC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+3, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nSPD);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+4, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nACC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+5, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nDCC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+6, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nSPD);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+7, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nACC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+8, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nDCC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+9, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nSPD);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+10, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nACC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+11, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nDCC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+12, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nSPD);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+13, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nACC);
		g_DMCont.m_dmTrayPP.SN(nTrayPPNumSM[i]+14, lpstDataCOK.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nDCC);
	}


	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Vacuum	, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Blow		, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nBlowTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_LDTrayPP_RetryCnt	, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_CylDownWait, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nCylDownWaitTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Pick_1Stage_Offset, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Offset);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Pick_1Stage_Speed, lpstDataCOK.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Speed);

	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Vacuum, lpstDataCOK.aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Blow, lpstDataCOK.aTrayPP_Tm[eTrayPP_2_UD].nBlowTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_LDTrayPP_RetryCnt, lpstDataCOK.aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_CylDownWait, lpstDataCOK.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Linear_Offset, lpstDataCOK.aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset);


	int nTestPPNumSM[MAX_TEST_PP_CNT] = { NDM4_SPD_TestPP1_X1, NDM4_SPD_TestPP2_X1};
	for(int i=RIDX_TEST_PP_1; i<=RIDX_TEST_PP_2;i++)
	{
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]	, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nSPD);	//nSPD_TestPP1_X, nSPD_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+1, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nACC);	//nACC_TestPP1_X, nACC_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+2, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nDCC);	//nDCC_TestPP1_X, nDCC_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+3, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nSPD);	//nSPD_TestPP1_X, nSPD_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+4, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nACC);	//nACC_TestPP1_X, nACC_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+5, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nDCC);	//nDCC_TestPP1_X, nDCC_TestPP2_X, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+6, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nSPD);	//nSPD_TestPP1_Y, nSPD_TestPP2_Y, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+7, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nACC);	//nACC_TestPP1_Y, nACC_TestPP2_Y, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+8, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nDCC);	//nDCC_TestPP1_Y, nDCC_TestPP2_Y, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+9, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+10, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+11, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+12, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+13, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+14, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+15, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+16, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+17, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+18, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+19, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+20, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+21, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+22, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+23, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+24, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nSPD);	//nSPD_TestPP1_Z, nSPD_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+25, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nACC);	//nACC_TestPP1_Z, nACC_TestPP2_Z, .....
		g_DMCont.m_dmTestPP.SN(nTestPPNumSM[i]+26, lpstDataCOK.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nDCC);	//nDCC_TestPP1_Z, nDCC_TestPP2_Z, .....
	}

	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP1_Vacuum	, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_1].nVacuumTm);
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP1_Blow		, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_1].nBlowTm);	 
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP2_Vacuum	, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_2].nVacuumTm);
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP2_Blow		, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_2].nBlowTm);	 

	g_DMCont.m_dmTestPP.SN(NDM4_TestPP1_RetryCnt	, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_1].nRetryCnt);
	g_DMCont.m_dmTestPP.SN(NDM4_TestPP2_RetryCnt	, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_2].nRetryCnt);

	g_DMCont.m_dmTestPP.SD(DDM4_TestPP1_Pick_Retry_Offset, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_1].dPickRetryOffset);
	g_DMCont.m_dmTestPP.SD(DDM4_TestPP2_Pick_Retry_Offset, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_2].dPickRetryOffset);

	g_DMCont.m_dmTestPP.SD(DDM4_TestPP1_Picker_Wait_Time, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_1].dPickerWaitTm);
	g_DMCont.m_dmTestPP.SD(DDM4_TestPP2_Picker_Wait_Time, lpstDataCOK.aTestPP_Tm[RIDX_TEST_PP_2].dPickerWaitTm);

	int nLoadTblNumSM[MAX_LD_TBL_CNT] = {NDM3_SPD_LoadTbl1_Y, NDM3_SPD_LoadTbl2_Y, NDM3_SPD_LoadTbl3_Y, NDM3_SPD_LoadTbl4_Y };
	int nLoadTblNumRotate[MAX_LD_TBL_CNT] = { NDM3_SPD_LoadTbl1_Rotate, NDM3_SPD_LoadTbl2_Rotate, NDM3_SPD_LoadTbl3_Rotate, NDM3_SPD_LoadTbl4_Rotate };
	for(int i=RIDX_LD_TBL_1; i<= RIDX_LD_TBL_4; i++)
	{
		g_DMCont.m_dmShuttle.SN(nLoadTblNumSM[i]	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD);
		g_DMCont.m_dmShuttle.SN(nLoadTblNumSM[i]+1	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC);
		g_DMCont.m_dmShuttle.SN(nLoadTblNumSM[i]+2	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC);

		g_DMCont.m_dmShuttle.SN(nLoadTblNumRotate[i]	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD);
		g_DMCont.m_dmShuttle.SN(nLoadTblNumRotate[i]+1	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC);
		g_DMCont.m_dmShuttle.SN(nLoadTblNumRotate[i]+2	, lpstDataCOK.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC);
	}

	int nPressNumSM[eMaxPressUnitCount] = { NDM11_SPD_PressUnit1, NDM11_SPD_PressUnit2, NDM11_SPD_PressUnit3, NDM11_SPD_PressUnit4, NDM11_SPD_PressUnit5, NDM11_SPD_PressUnit6, NDM11_SPD_PressUnit7, NDM11_SPD_PressUnit8 };
	                                      //,NDM11_SPD_PressUnit9, NDM11_SPD_PressUnit10,NDM11_SPD_PressUnit11,NDM11_SPD_PressUnit12,NDM11_SPD_PressUnit13,NDM11_SPD_PressUnit14,NDM11_SPD_PressUnit15,NDM11_SPD_PressUnit16};
	
	int nPressNumBlow[eMaxPressUnitCount] = { NDM11_TM_PRESS1_BLOW_WAIT, NDM11_TM_PRESS2_BLOW_WAIT, NDM11_TM_PRESS3_BLOW_WAIT, NDM11_TM_PRESS4_BLOW_WAIT, NDM11_TM_PRESS5_BLOW_WAIT, NDM11_TM_PRESS6_BLOW_WAIT, NDM11_TM_PRESS7_BLOW_WAIT
											, NDM11_TM_PRESS8_BLOW_WAIT };//, NDM11_TM_PRESS9_BLOW_WAIT, NDM11_TM_PRESS10_BLOW_WAIT, NDM11_TM_PRESS11_BLOW_WAIT, NDM11_TM_PRESS12_BLOW_WAIT, NDM11_TM_PRESS13_BLOW_WAIT, NDM11_TM_PRESS14_BLOW_WAIT
// 											, NDM11_TM_PRESS15_BLOW_WAIT, NDM11_TM_PRESS16_BLOW_WAIT};

	for(int i=RIDX_PRESS_UNIT_1; i<= RIDX_PRESS_UNIT_8; i++)
	{
		g_DMCont.m_dmHandlerOpt.SN(nPressNumSM[i]   , lpstDataCOK.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD);
		g_DMCont.m_dmHandlerOpt.SN(nPressNumSM[i]+1 , lpstDataCOK.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC);	
		g_DMCont.m_dmHandlerOpt.SN(nPressNumSM[i]+2 , lpstDataCOK.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC);
		
		g_DMCont.m_dmHandlerOpt.SN(nPressNumBlow[i], lpstDataCOK.aPress_Tm[i].nBlowTm);
	}

	g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_X   , lpstDataCOK.aTrayForm.nCnt_X);
	g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_Y   , lpstDataCOK.aTrayForm.nCnt_Y);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_X, lpstDataCOK.aTrayForm.nStart_PitchX);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_Y, lpstDataCOK.aTrayForm.nStart_PitchY);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_X, lpstDataCOK.aTrayForm.nPitchX);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_Y, lpstDataCOK.aTrayForm.nPitchY);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Thickness, lpstDataCOK.aTrayForm.nThickness);	

	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_X,  lpstDataCOK.nLoadTbl_Xpitch);
	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_Y,  lpstDataCOK.nLoadTbl_Ypitch);
    g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Div_X  ,  lpstDataCOK.nLoadTbl_XCnt);
    g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Div_Y  ,  lpstDataCOK.nLoadTbl_YCnt);

	g_DMCont.m_dmTestPP.SN( NDM4_TestSite_Div_X, lpstDataCOK.nTestSite_XCnt );
	g_DMCont.m_dmTestPP.SN( NDM4_TestSite_Div_Y, lpstDataCOK.nTestSite_YCnt );
	g_DMCont.m_dmTestPP.SN( NDM4_TestSite_Pitch_X, lpstDataCOK.nTestSite_Xpitch );
	g_DMCont.m_dmTestPP.SN( NDM4_TestSite_Pitch_Y, lpstDataCOK.nTestSite_Ypitch );

	g_DMCont.m_dmTestPP.SN(NDM4_TestBuffTbl_Div_X  ,	lpstDataCOK.nTestBuffTbl_XCnt);
	g_DMCont.m_dmTestPP.SN(NDM4_TestBuffTbl_Div_Y  ,	lpstDataCOK.nTestBuffTbl_YCnt);
	g_DMCont.m_dmTestPP.SN(NDM4_TestBuffTbl_Pitch_X,	lpstDataCOK.nTestBuffTbl_Xpitch);
	g_DMCont.m_dmTestPP.SN(NDM4_TestBuffTbl_Pitch_Y,	lpstDataCOK.nTestBuffTbl_Ypitch);

	g_DMCont.m_dmTestPP.SN( NDM4_TestCleanTbl_Div_X, lpstDataCOK.nTestClean_XCnt );
	g_DMCont.m_dmTestPP.SN( NDM4_TestCleanTbl_Div_Y, lpstDataCOK.nTestClean_YCnt );
	g_DMCont.m_dmTestPP.SN( NDM4_TestCleanTbl_Pitch_X, lpstDataCOK.nTestClean_Xpitch );
	g_DMCont.m_dmTestPP.SN( NDM4_TestCleanTbl_Pitch_Y, lpstDataCOK.nTestClean_Ypitch );

	g_DMCont.m_dmTestPP.SN( NDM4_TestPP_Pitch_X, lpstDataCOK.nTestPP_Xpitch );
	g_DMCont.m_dmTestPP.SN( NDM4_TestPP_Pitch_Y, lpstDataCOK.nTestPP_Ypitch );

	g_DMCont.m_dmTrayPP.SN(NDM5_TrayPP_LdTbl_Pitch_X, lpstDataCOK.nTrayPPLtPitchX);
	g_DMCont.m_dmTrayPP.SN(NDM5_TrayPP_LdTbl_Pitch_Y, lpstDataCOK.nTrayPPLtPitchY);

	g_DMCont.m_dmStacker.SN(NDM7_STK_MAIN_UPDN_WAITTIME, lpstDataCOK.nStacker_Clamp_WaitTm);
	g_DMCont.m_dmStacker.SN(NDM7_STK_SUB_UPDN_WAITTIME, lpstDataCOK.nStacker_Sub_WaitTm);
	g_DMCont.m_dmStacker.SN(NDM7_STK_CLAMP_WAITTIME, lpstDataCOK.nStacker_Clamp_WaitTm);
	g_DMCont.m_dmStacker.SN(NDM7_STK_CONVEYOR_UNLOAD_STOP_WAIT, lpstDataCOK.nStacker_Conveyor_StopWaitTm);
	g_DMCont.m_dmStacker.SN(NDM7_SLOW_UNLOAD_STACKER_OPTION, lpstDataCOK.nSlowUnloadStackerOption);


	//2d Mark Validate Option Use
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_BarCode_Use, lpstDataCOK.b2dBarcodeUse);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_DUPLICATION_OPT_USE, lpstDataCOK.b2dDuplicateOptUse);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_LENGTH_OPT_USE, lpstDataCOK.b2dLengthOptUse);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_2DID_LENGTH, lpstDataCOK.n2dDeviceLength);
	
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_MARK_OPT_USE, lpstDataCOK.b2dMarkOptUse);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_2D_MARK_START_INDEX, lpstDataCOK.n2dMarkStartIdx);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_DEVICE_NAME, lpstDataCOK.sz2dDeviceName, sizeof(lpstDataCOK.sz2dDeviceName));
	g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_MARK_LIST, lpstDataCOK.sz2dMarkList, sizeof(lpstDataCOK.sz2dMarkList));
}

void CSystemData::UpdateSltPgmDataToSM(ST_SLT_PGM lpstData)
{
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BootTmOut	    , lpstData.bMD_BootTmOut	  );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_PreCooling      , lpstData.bMD_PreCooling     );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AcPwrOnDly      , lpstData.bMD_AcPwrOnDly     );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AcPwrOff        , lpstData.bMD_AcPwrOff       );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AtxPowerOn      , lpstData.bMD_AtxPowerOn     );    
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_FailCodeDig     , lpstData.bMD_FailCodeDig    );    
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_MinPassTm       , lpstData.bMD_MinPassTm      );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BaudRate        , lpstData.bMD_BaudRate       );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_HangReboot      , lpstData.bMD_HangReboot     );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BiosBoot        , lpstData.bMD_BiosBoot       );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BiosS3          , lpstData.bMD_BiosS3         );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_TempMonitor     , lpstData.bMD_TempMonitor    );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_ExtentionPwrAGP , lpstData.bMD_ExtentionPwrAGP);
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_PassStr         , lpstData.bMD_PassStr        );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_FailCodeHead    , lpstData.bMD_FailCodeHead   );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_StartLine       , lpstData.bMD_StartLine      );
    g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_EndLine         , lpstData.bMD_EndLine        );

	g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_SiteOff_TestTimeOut, lpstData.bMD_SiteOffTestTimeOut );
	g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_SiteOff_DefaultBin, lpstData.bMD_SiteOffDefaultBin );
	g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_Sort_as_DefaultBin, /*lpstData.bMD_Sort_as_DefaultBin*/TRUE );

	g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_SotDelayTm        , lpstData.nSotDelayTm         );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_TestTmOut         , lpstData.nTestTmOut	         );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_FailCodeDigitalNo , lpstData.nFailCodeDigitalNo  );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_PreCooling        , lpstData.nPreCoolingTm       );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AcPowerOn         , lpstData.nAcPowerOnDelayTm   );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AcPowerOff        , lpstData.nAcPowerOffDelayTm  );    
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AtxPowerOn        , lpstData.nAtxPowerOnDelayTm  );    
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_MinPassTm         , lpstData.nMinPassTm          );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BaudRate          , lpstData.nBaudRate           );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosBootCnt       , lpstData.nBiosBootCnt        );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosTmOut         , lpstData.nBiosTmOut          );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosS3Cnt         , lpstData.nBiosS3Cnt          );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_S3TmOut           , lpstData.nBiosS3TmOut        );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosS3DelayTm     , lpstData.nBiosS3DelayTm      );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_SetTemp           , lpstData.nSetTemp            );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_TempGuardBand     , lpstData.nTempGuardBand      );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AgpDelayTm        , lpstData.nAgpDelayTm         );
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_NotTestingTmOut, lpstData.nNotTestingTmOut);

    g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_PassStr	         , lpstData.szPassString    , strlen(lpstData.szPassString));
    g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_FailCodeHeader	 , lpstData.szFailCodeHeader, strlen(lpstData.szFailCodeHeader));
    g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_StartLine	     , lpstData.szStartLine     , strlen(lpstData.szStartLine));
    g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_EndLine	         , lpstData.szEndLine       , strlen(lpstData.szEndLine));
}

void CSystemData::UpdateFTPFileToSM()
{	
	char szPathFileName[MAX_PATH]={0,};
	char szOEETemp[512]={0,}, szOEEIP[MAX_PATH] = {0,}, szOEEPath[MAX_PATH] = {0,};
	::GetCurrentDirectory(sizeof(szOEETemp), szOEETemp);
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szOEETemp, SZ_FTP_FILE_NAME);

	CConfigData CConfig(szPathFileName);

	char szCompany[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	//OEE DATA
	if(strcmp(szCompany,"SCK") == 0 ){
		CConfig.GetString("OEE DATA","IP","0.0.0.0",szOEEIP);
		CConfig.GetString("OEE DATA","PATH",SZ_DIR_PATH_OEE_DATA,szOEEPath);
		int nOEEChk = CConfig.GetInt("OEE DATA","USE",0);

		g_DMCont.m_dmEQP.SS(SDM0_OEE_IP,szOEEIP,sizeof(szOEEIP));
		g_DMCont.m_dmEQP.SS(SDM0_OEE_PATH,szOEEPath,sizeof(szOEEPath));
		g_DMCont.m_dmEQP.SN(NDM0_OEE_USE,nOEEChk);
	}
	char szSubTitle[128] = {0,};
	char szMacPath[256] = {0,};
	char szServerPath[256] = {0,};
	char szFTPOption[256] = {0,};
	int  nChk = 0;

	nChk = CConfig.GetInt("FTP","Use",0);
	g_DMCont.m_dmEQP.SN(NDM0_FTP_USE, nChk);
	
	nChk = CConfig.GetInt("FTP","Type",0);
	g_DMCont.m_dmEQP.SN(NDM0_FTP_SERVER_USE, nChk);

	CConfig.GetString("FTP","IP", "", szFTPOption);
	g_DMCont.m_dmEQP.SS(SDM0_FTP_IP, (LPSTR)(LPCTSTR)szFTPOption, sizeof(szFTPOption));
	
	CConfig.GetString("FTP","ID", "", szFTPOption);
	g_DMCont.m_dmEQP.SS(SDM0_FTP_ID, (LPSTR)(LPCTSTR)szFTPOption, sizeof(szFTPOption));
	
	CConfig.GetString("FTP","PW", "", szFTPOption);
	g_DMCont.m_dmEQP.SS(SDM0_FTP_PW, (LPSTR)(LPCTSTR)szFTPOption, sizeof(szFTPOption));

	//FTP File Path
	for(int i=0; i < eMaxPressUnitCount; i++){
		ZeroMemory(szSubTitle, sizeof(szSubTitle));
		ZeroMemory(szMacPath, sizeof(szMacPath));
		ZeroMemory(szServerPath, sizeof(szServerPath));

		sprintf_s(szSubTitle, sizeof(szSubTitle), "Site%d", i+1);
		CConfig.GetString("FTP MAC PATH", szSubTitle, "", szMacPath);
		CConfig.GetString("FTP SERVER PATH", szSubTitle, "", szServerPath);
		nChk = CConfig.GetInt("FTP Use", szSubTitle, 0);

		g_DMCont.m_dmEQP.SS(SDM0_FTP_MAC_SITE1+i   , (LPSTR)(LPCTSTR)szMacPath   , sizeof(szMacPath)    );
		g_DMCont.m_dmEQP.SS(SDM0_FTP_SERVER_SITE1+i, (LPSTR)(LPCTSTR)szServerPath, sizeof(szServerPath) );
		g_DMCont.m_dmEQP.SN(NDM0_FTP_USE_SITE1+i, nChk);
	}
}


void CSystemData::Update2DMarkToSM()
{

}