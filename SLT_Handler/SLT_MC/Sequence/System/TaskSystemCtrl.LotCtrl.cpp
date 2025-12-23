#include "StdAfx.h"
#include "TaskSystemCtrl.h"
#include "ConfigData.h"

#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::OnLotCommand(eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam)
{
	// check condition for command

	MakeLog("CTaskSystemCtrl::OnLotCommand Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));

	switch(eLotCmd)
	{
	case eAutoLotCmd_Skip:
	case eAutoLotCmd_AlarmPause:
	case eAutoLotCmd_Pause:
		{
			g_START_SW.SetStatus(FALSE);
			g_STOP_SW.SetStatus(TRUE);

			// stop time
			if(m_tcStop.IsChecking() == FALSE){
				m_tcStop.StartTimer();
			}
		}break;
	case eAutoLotCmd_Initial:
		{
			int nErr = CheckEqpConditionForFirstStart();
			if( nErr != ERR_NO_ERROR ){
				MakeLog("CheckEqpConditionForFirstStart Err No=%d", nErr);
				//OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);

				g_START_SW.SetStatus(FALSE);
				g_STOP_SW.SetStatus(TRUE);
				//return nErr;

				return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
			}
		
			nErr = CheckEqpConditionForStart();
			if( nErr != ERR_NO_ERROR ){
				MakeLog("CheckEqpConditionForStart Err No=%d", nErr);
				//OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);

				g_START_SW.SetStatus(FALSE);
				g_STOP_SW.SetStatus(TRUE);
				//return nErr;
				return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
			}

			// Init AutoGRR DB Data
			g_DB.DELETE_ALL_GRRDATA_TBL();
			 
			g_STOP_SW.SetStatus(FALSE);
			g_START_SW.SetStatus(TRUE);
			
			// jira 313 ���� auto idle�� �Ť���
			//BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
			//if (bRetOneCycle) {
			//	g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, FALSE);
			//}

			CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Auto);
		}break;
	case eAutoLotCmd_Retry:
	case eAutoLotCmd_CleanOut:
	case eAutoLotCmd_Resume:
		{
			//----- check eqp condition for start
			MakeLog("CheckEqpConditionForStart CTaskSystemCtrl::OnLotCommand = %s", GetLotCmdString(eLotCmd));
			int nErr = CheckEqpConditionForStart();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport( nErr, AlarmData, this );
				//OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);

				g_START_SW.SetStatus(FALSE);
				g_STOP_SW.SetStatus(TRUE);				
				//return nErr;
				return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
			}
			BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
			if(bAutoGrrUse == TRUE)
			{
				nErr = ChkAllSiteOffGrr();
			}else
			{
				nErr = CheckEqpSocketSite();
			}

			if( nErr != ERR_NO_ERROR ){
                CParamList AlarmData;
                g_Error.AlarmReport(nErr, AlarmData, this);

                //OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);

				g_START_SW.SetStatus(FALSE);
				g_STOP_SW.SetStatus(TRUE);				
                //return nErr;
				return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
            }

            // Onecycle complete ���Ŀ� Site ���� ���� �ϵ��� �Ѵ�.
            {
                BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
                if(bRetOneCycle){
                    g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, FALSE);
                }
            }

			// add stop time
			if(m_tcStop.IsChecking()){

				double dStopTime =  m_tcStop.CompareTime(FALSE);
				m_dTotalStopTm += dStopTime;
				m_tcStop.SetTimerCheckEnd();

				for (int i = 0; i < eMaxPressUnitCount; i++)
				{
					BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[i].IsChecking();
					if (bChkIndex == TRUE)
					{
						double dRecodeTime = g_DMCont.m_dmContac_Dn.GD(DDM1_STOP_INDEX_STATION1 + i);
						g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + i, dStopTime+ dRecodeTime);
					}
				}
			}

			g_STOP_SW.SetStatus(FALSE);
			g_START_SW.SetStatus(TRUE);
			CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Auto);
		}break;
	default:
		{
			g_START_SW.SetStatus(FALSE);
			g_STOP_SW.SetStatus(TRUE);

			CParamList AlarmData;
			g_Error.AlarmReport(ERR_LOT_CMD_COMMAND_DOES_NOT_EXIST, AlarmData, this);
			return ERR_LOT_CMD_COMMAND_DOES_NOT_EXIST;
		}break;
	}

	

	BOOL bErrChk = FALSE;

	MakeLog("CTaskSystemCtrl::g_TaskAutoSystem Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	int nErr = g_TaskAutoSystem.OnLotCommand( eLotCmd, NULL, NULL );

	MakeLog("CTaskSystemCtrl::g_TaskAutoTestPp Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	for (int i = 0; i < eMaxTestPPCount; i++)
	{
		nErr |= g_TaskAutoTestPp[eTestPP_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
		if (nErr != ERR_NO_ERROR) {
			bErrChk = TRUE;
		}
	}


	MakeLog("CTaskSystemCtrl::g_TaskAutoTrayUD Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	nErr = g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bErrChk = TRUE;
	}

	MakeLog("CTaskSystemCtrl::g_TaskAutoTrayLD Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	nErr = g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bErrChk = TRUE;
	}

	MakeLog("CTaskSystemCtrl::g_TaskAllAutoStacker Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	nErr = g_TaskAllAutoStacker.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bErrChk = TRUE;
	}


	MakeLog("CTaskSystemCtrl::g_TaskAutoLdTable Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		nErr |= g_TaskAutoLdTable[eLoadTbl_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
		if (nErr != ERR_NO_ERROR) {
			bErrChk = TRUE;
		}
	}

	MakeLog("CTaskSystemCtrl::g_TaskAutoPress Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		nErr |= g_TaskAutoPress[i].OnLotCommand(eLotCmd, NULL, NULL);
		if (nErr != ERR_NO_ERROR) {
			bErrChk = TRUE;
		}
	}



	MakeLog("CTaskSystemCtrl::g_TaskAutoTrayFeeder Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	nErr = g_TaskAutoTrayFeeder.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bErrChk = TRUE;
	}

	MakeLog("CTaskSystemCtrl::g_TaskAutoTransfer Start [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	nErr = g_TaskAutoTransfer.OnLotCommand( eLotCmd, NULL, NULL );
	if( nErr != ERR_NO_ERROR ) {
		bErrChk = TRUE;
	}

// 	MakeLog("CTaskSystemCtrl::g_TaskAllTestSite Start [eLotCmd = %d]", eLotCmd);
// 	nErr = g_TaskAllTestSite.OnLotCommand(eLotCmd, NULL, NULL);
// 	if (nErr != ERR_NO_ERROR) {
// 		bErrChk = TRUE;
// 	}

	if( bErrChk == TRUE){
		g_START_SW.SetStatus(FALSE);
		g_STOP_SW.SetStatus(TRUE);
		OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);
		return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
	}

	MakeLog("CTaskSystemCtrl::OnLotCommand End [eLotCmd = %s]", GetLotCmdString(eLotCmd));
	return nErr;
}

int CTaskSystemCtrl::CheckOnlineTesterInfo()
{
	CString strDisconnect = _T("");
	CString strTesterSW_VersionMisMatch = _T("");
	CString strHWIDMisMatch = _T("");
	CString strFWIDMisMatch = _T("");
	CString strCmpFWID = _T("");
	CString strStarlingFWID = _T("");
	CString strStarlingMisMatch = _T("");
	CString strNetDrvConnected = _T("");
	CString strDupStationID = _T("");
	CString strFtpServerConnected = _T("");
	CString strErrorSocketID = _T("");
	CString strErrorTesterID = _T("");
	CString strTestScript_Apl = _T("");

	std::vector<CString> vTestScript_Apl;
	std::vector<CString> vTestMode;
	std::vector<CString> vTesterSW_VersionMisMatch;
	std::vector<ST_FW_VERSION> vHWIDMisMatch;
	std::vector<ST_FW_VERSION> vFWPPOMisMatch;
	std::vector<ST_FW_VERSION> vFWHIDMisMatch;
	std::vector<CString> vTestSW_Version_Apl;

	std::multiset< std::string > setTesterID;
	std::multiset< std::string > setTesterIP;
	std::multiset< std::string > setSocketID;

	char szProgramID[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szProgramID, sizeof(szProgramID));
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	CString strDriverSite = _T("");
	std::vector<int> vStationID;
	BOOL bAutoGrrUse = FALSE;//g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrUse );
	BOOL bAutoAuditUse = FALSE;
	//int nOffsite = (bAutoGrrUse == TRUE) ? NDM11_GRR_SocketOnOff_Site1 : NDM11_SocketOnOff_Site1;
	int nFtpUse = g_DMCont.m_dmEQP.GN(NDM0_FTP_USE);
	int nFtpType = g_DMCont.m_dmEQP.GN(NDM0_FTP_SERVER_USE);

	int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoGrrUse = TRUE;


	CString strTestScript = _T("");
	char szBuffer[256] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szBuffer, sizeof(szBuffer));
	strTestScript.Format("%s", szBuffer);

	CString strTPProg;
	char szTP_Prog[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szTP_Prog, MAX_PATH);
	strTPProg.Format("%s", szTP_Prog);

	CString strTestMode = _T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation, szBuffer, sizeof(szBuffer));
	strTestMode.Format("%s", szBuffer);
	int nTestMode = GetTestModeStringToint(strTestMode);


//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0
	if (nTesterOption == eTester_Single)
	{
		for (int i = 0; i < eMaxPressUnitCount; i++) {
			int nRetSocket = g_TaskPressUnit[i].GetSocketOnCnt(bAutoGrrUse);

			if (bAutoAuditUse == TRUE)
				nRetSocket = g_TaskPressUnit[i].GetSocketAuditOnCnt();

			if (nRetSocket > 0)
			{
				int nStationID = i + 1;
				BOOL bConnect = g_AppleTestIF.IsConnected(nStationID, nTesterOption);//SendAction(i, ACTION_STATUS);
				if (bConnect != TRUE) {
					CString strTmpSiteNo = _T("");
					strTmpSiteNo.Format("[Site=%d]",  i + 1);
					strDisconnect += strTmpSiteNo;
				}

				if (bConnect == TRUE)
				{
					vStationID.push_back(nStationID);

					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID, nTesterOption);
					std::vector<std::pair<std::string, std::string>> listTesterInfo;

					//Single, Dual�� ù��° �迭�� ���� ����.
					cTesterStatus.PasingInformation(cTesterStatus.m_sInfo[0], listTesterInfo); 

					// connect������, ���� information�� ������ ����ų�, connect�ǰ� �� �ڿ� ���۽��� ����� ��� debug �߻���.
					if (listTesterInfo.empty() == TRUE) {
						CString strTmpSiteNo = _T("");
						strTmpSiteNo.Format("Site=%d]",  i + 1);
						strDisconnect += strTmpSiteNo;
						continue;
					}

					// Socket ID Data �̻� Ȯ��
					CString strSocketID = _T("");

					for (int nY = 0; nY < TestSite_Div_Y; nY++) {
						for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
							strSocketID.Format("%s", cTesterStatus.m_sSocketID_Orginal[nX + (nY * STATION_MAX_X_SITE)]);
							int nTarget = (bAutoGrrUse == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (i *STATION_MAX_PARA);

							if (bAutoAuditUse == TRUE)
								nTarget = NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (i *STATION_MAX_PARA);

							if (strSocketID.Compare(REG_SOCKET_ID_ERROR) == 0 && (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)) {
								CString strTmp = _T("");
								strTmp.Format(_T("[%d Station] Socket Number =%d \n"), i + 1, (nX + (nY *STATION_MAX_X_SITE)));
								strErrorSocketID += strTmp;
							}
							else if (strSocketID.Compare(REG_SOCKET_ID_ERROR) != 0) {
								setSocketID.insert((LPCTSTR)strSocketID);
							}
						}
					}
					//Tester ID ���� Check
					CString strTesterID = _T("");

					strTesterID.Format(_T("%s"), listTesterInfo[eTesterInfo_TesterID].second.c_str());
					if (strTesterID.IsEmpty()) {
						CString strTmp = _T("");
						strTmp.Format(_T("[%d Station]\n"), i + 1);
						strErrorTesterID += strTmp;
					}

					CString strVer = _T("");
					CString AddStr = _T("");
// 					strVer.Format("%s", listTesterInfo[eTesterInfo_SW_Version].second.c_str());
// 					if (strcmp(szProgramID, strVer) != 0) {
// 						AddStr.Format("[Station=%d, Handler = %s | Tester = %s ]", i + 1, szProgramID, strVer);
// 						vTesterSW_VersionMisMatch.push_back(AddStr);
// 					}


					// �ߺ� üũ ���� set�� ����.
					setTesterID.insert(listTesterInfo[eTesterInfo_TesterID].second);
					setTesterIP.insert(listTesterInfo[eTesterInfo_IP].second);

					int nTesterTestMode = cTesterStatus.m_nTest_Mode;
					CString sTesterRevScript = cTesterStatus.m_sTestScriptInfo;
					CString sTesterSWVersion = cTesterStatus.m_sTestSWVersion;
					if (nProjectOption == DEF_PROJECT_WATT) // Carpenter 패키지는 Tester에서 Test_Script 정보를 받아서 처리 하기 때문에 체크 시점 변경
					{
						if (nTestMode != nTesterTestMode ) {
							AddStr.Format("[Station No=%d] [Handler=%s | Tester=%s]", i+1, GetTestModeintToString(nTestMode), GetTestModeintToString(nTesterTestMode));
							vTestMode.push_back(AddStr);
						}

						if (strTestScript != sTesterRevScript) {
							AddStr.Format("[Station No=%d] [Handler=%s | Tester=%s]", i + 1, strTestScript, sTesterRevScript);
							vTestScript_Apl.push_back(AddStr);
						}

						if (strTPProg != sTesterSWVersion) {
							AddStr.Format("[Station No=%d] [Handler=%s | Tester=%s]", i + 1, strTPProg, sTesterSWVersion);
							vTestSW_Version_Apl.push_back(AddStr);
						}
					}



				}
			}
		}
	}
	else if(nTesterOption == eTester_Dual)
	{
		for (int i = 0; i < MAX_TESTER_DUAL; i++) {
			int nRetSocket = g_TaskPressUnit[i/2].GetDirDualSocketOnCnt(bAutoGrrUse, i % 2);

			if (bAutoAuditUse == TRUE)
				nRetSocket = g_TaskPressUnit[i / 2].GetSocketAuditOnCnt();

			if (nRetSocket > 0)
			{
				int nStationID = i + 1;
				BOOL bConnect = g_AppleTestIF.IsConnected(nStationID);//SendAction(i, ACTION_STATUS);
				if (bConnect != TRUE) {
					CString strTmpSiteNo = _T("");
					strTmpSiteNo.Format("[Site=%d, Tester=%d-%d]",  i / 2 +1, i / 2 + 1, (i % 2) + 1);
					strDisconnect += strTmpSiteNo;
				}

				if (bConnect == TRUE)
				{
					vStationID.push_back(nStationID);

					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
					std::vector<std::pair<std::string, std::string>> listTesterInfo;

					//Single, Dual�� ù��° �迭�� ���� ����.
					cTesterStatus.PasingInformation(cTesterStatus.m_sInfo[0], listTesterInfo);

					// connect������, ���� information�� ������ ����ų�, connect�ǰ� �� �ڿ� ���۽��� ����� ��� debug �߻���.
					if (listTesterInfo.empty() == TRUE) {
						CString strTmpSiteNo = _T("");
						strTmpSiteNo.Format("[Site=%d]", i + 1);
						strDisconnect += strTmpSiteNo;
						continue;
					}

					// Socket ID Data �̻� Ȯ��
					CString strSocketID = _T("");

					for (int nY = 0; nY < 1; nY++) {
						for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
							strSocketID.Format("%s", cTesterStatus.m_sSocketID_Orginal[nX + (nY * STATION_MAX_X_SITE)]);
							int nTarget = (bAutoGrrUse == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (i/2 *STATION_MAX_PARA);

							if (bAutoAuditUse == TRUE)
								nTarget = NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (i/2 *STATION_MAX_PARA);

							if (strSocketID.Compare(REG_SOCKET_ID_ERROR) == 0 && (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)) {
								CString strTmp = _T("");
								strTmp.Format(_T("[%d Station] Socket Number =%d \n"), i /2 + 1, (nX + (nY *STATION_MAX_X_SITE) +1));
								strErrorSocketID += strTmp;
							}
							else if (strSocketID.Compare(REG_SOCKET_ID_ERROR) != 0) {
								setSocketID.insert((LPCTSTR)strSocketID);
							}
						}
					}
					//Tester ID ���� Check
					CString strTesterID = _T("");

					strTesterID.Format(_T("%s"), listTesterInfo[eTesterInfo_TesterID].second.c_str());
					if (strTesterID.IsEmpty()) {
						CString strTmp = _T("");
						strTmp.Format(_T("[%d Station]\n"), i + 1/2);
						strErrorTesterID += strTmp;
					}

					CString strVer = _T("");
					strVer.Format("%s", listTesterInfo[eTesterInfo_SW_Version].second.c_str());
					CString AddStr;
					if (strcmp(szProgramID, strVer) != 0) {
						AddStr.Format("[Station=%d, Handler = %s | Tester = %s ]", i / 2 + 1, szProgramID, strVer);
						vTesterSW_VersionMisMatch.push_back(AddStr);
					}


					// �ߺ� üũ ���� set�� ����.
					setTesterID.insert(listTesterInfo[eTesterInfo_TesterID].second);
					setTesterIP.insert(listTesterInfo[eTesterInfo_IP].second);

				}
			}
		}
	}
	else if (nTesterOption == eTester_EachSocket)
	{
		for (int i = 0; i < _ePressUnit::eMaxPressUnitCount; i++) { // station Ž��.

			int nRetSocket = g_TaskPressUnit[i].GetSocketOnCnt();

			BOOL bSocketOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
			g_TaskPressUnit[i].GetSocketStsForEachSocket(bAutoGrrUse, bSocketOnOff);


			if (bAutoAuditUse == TRUE)
			{
				nRetSocket = g_TaskPressUnit[i].GetSocketAuditOnCnt();
				memset(bSocketOnOff, 0, sizeof(bSocketOnOff));
				g_TaskPressUnit[i].GetSocketAuditOnOffForEachSocket(bSocketOnOff);
			}

			if (nRetSocket > 0)
			{
				int nStationID = i + 1;
				// ���� ���µ� ���� Ȯ�� �Ѵ�.

				// �������� socket off ������ ���� �Ѵ�.

				BOOL bConnect = g_AppleTestIF.IsConnected(nStationID, nTesterOption, bSocketOnOff);//SendAction(i, ACTION_STATUS);
				if (bConnect != TRUE) { // Fail connect
					CString strTmpSiteNo = _T("");
					strTmpSiteNo.Format("[Station=%d", i + 1);// station ����

					// ����
					// 0: 1, 1, 1
					// 1: 1, 1, 2
					// 2: 2, 2, 1
					// 3: 2, 2, 2
					strDisconnect += strTmpSiteNo;
				}
				else //if (bConnect == TRUE) success connnect
				{
					// ���� �̻� ���� Ȯ�� �� socket ���� Ȯ�� �Ѵ�.
					vStationID.push_back(nStationID);
					
					int nDutNo = 0;
					
					std::vector<std::pair<std::string, std::string>> listTesterInfo;

					int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
					int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);



					for (nDutNo = 0; nDutNo < TestSite_Div_X*TestSite_Div_Y; nDutNo++)
					{
						if (bSocketOnOff[nDutNo] != DEF_ON)
						{
							continue;
						}

						int nSocketNumber = (((i) * (TestSite_Div_X*TestSite_Div_Y)) + nDutNo) + 1;
						CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nSocketNumber);

						listTesterInfo.clear();
						cTesterStatus.PasingMsgType(cTesterStatus.m_sInfo[0], listTesterInfo, DEF_MSG_TYPE_INFORMATION);

						// ���������� socket��  HW,FW,SW ������ Ȯ�� �Ѵ�.

						// connect������, ���� information�� ������ ����ų�, connect�ǰ� �� �ڿ� ���۽��� ����� ��� debug �߻���.
						if (listTesterInfo.empty() == TRUE) {
							CString strTmpSiteNo = _T("");
							strTmpSiteNo.Format("[Station=%d][Socket=%d]", i + 1, nDutNo + 1);
							strDisconnect += strTmpSiteNo;
							continue;
						}

						//// Socket ID Data �̻� Ȯ��
						//CString strSocketID = _T("");
						//
						//for (int nY = 0; nY < 1; nY++) {
						//	for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
						//		strSocketID.Format("%s", cTesterStatus.m_sSocketID_Orginal[nX + (nY * STATION_MAX_X_SITE)]);
						//		int nTarget = (bAutoGrrUse == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (i / 2 * STATION_MAX_PARA);
						//
						//		if (bAutoAuditUse == TRUE)
						//			nTarget = NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (i / 2 * STATION_MAX_PARA);
						//
						//		if (strSocketID.Compare(REG_SOCKET_ID_ERROR) == 0 && (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)) {
						//			CString strTmp = _T("");
						//			strTmp.Format(_T("[%d station(socket)] Socket Number =%d \n"), i + 1, i + 1);
						//			strErrorSocketID += strTmp;
						//		}
						//		else if (strSocketID.Compare(REG_SOCKET_ID_ERROR) != 0) {
						//			setSocketID.insert((LPCTSTR)strSocketID);
						//		}
						//	}
						//}

						// DUT ���� ����� ��� �������� ���� �� HW, FW, SW ���� Ȯ��.
						CString strVer = _T("");
						strVer.Format("%s", listTesterInfo[eTesterInfoEach_SW_Version].second.c_str());
						CString AddStr;
						if (strcmp(szProgramID, strVer) != 0) {
							AddStr.Format("[Station=%d, Handler = %s | Tester = %s ]", i / 2 + 1, szProgramID, strVer);
							vTesterSW_VersionMisMatch.push_back(AddStr);
						}

						_ST_FW_VERSION stVersion;
					
						stVersion.sVersion = listTesterInfo[eTesterInfoEach_HW_Version].second.c_str();
						stVersion.nIndex = nSocketNumber;
						vHWIDMisMatch.push_back(stVersion);

						stVersion.clear();
						stVersion.sVersion = listTesterInfo[eTesterInfoEach_PPO_FW_Version].second.c_str();
						stVersion.nIndex = nSocketNumber;
						vFWPPOMisMatch.push_back(stVersion);

						stVersion.clear();
						stVersion.sVersion = listTesterInfo[eTesterInfoEach_HID_FW_Version].second.c_str();
						stVersion.nIndex = nSocketNumber;
						vFWHIDMisMatch.push_back(stVersion);

// 						strVer.Format("%s", listTesterInfo[eTesterInfoEach_HW_Version].second.c_str());
// 
// 						if (strcmp("", strVer) == 0) {	// HW ���� �׸� �߰� �Ѵ�.
// 							AddStr.Format("[Station=%d, Socket=%d %s]", i + 1, nDutNo + 1, strVer);
// 							vHWIDMisMatch.push_back(AddStr);
// 						}

// 						strVer.Format("%s", listTesterInfo[eTesterInfoEach_PPO_FW_Version].second.c_str());
// 
// 						if (strcmp("", strVer) == 0) {	// HW ���� �׸� �߰� �Ѵ�.
// 							AddStr.Format("[Station=%d, Socket=%d %s]", i + 1, nDutNo + 1, strVer);
// 							vFWPPOMisMatch.push_back(AddStr);
// 						}
// 
// 						strVer.Format("%s", listTesterInfo[eTesterInfoEach_HID_FW_Version].second.c_str());
// 
// 						if (strcmp("", strVer) == 0) {	// HW ���� �׸� �߰� �Ѵ�.
// 							AddStr.Format("[Station=%d, Socket=%d %s]", i + 1, nDutNo + 1, strVer);
// 							vFWHIDMisMatch.push_back(AddStr);
// 						}

						CString strTesterScript = cTesterStatus.m_sTest_Script_Apl;

						if (nProjectOption == DEF_PROJECT_CARPENTERS) // Carpenter 패키지는 Tester에서 Test_Script 정보를 받아서 처리 하기 때문에 체크 시점 변경
						{
							if (strcmp(strTestScript, strTesterScript) != 0) {
								AddStr.Format("[Socket No=%d] [APL=%s | Tester=%s]", nSocketNumber, strTestScript, strTesterScript);
								vTestScript_Apl.push_back(AddStr);		
							}
						}

					}

				}
			}
		}
	}
//#endif


	CString strDupID = _T("");

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));


	if (strErrorSocketID.IsEmpty() != TRUE)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Socket ID Error!! [Socket ID is Empty]");
		AlarmData.m_fnAddItemFormat("%s", strErrorSocketID);
		g_Error.AlarmReport(ERR_TESTER_SOCKET_ID_ERROR, AlarmData, this);
		return ERR_TESTER_SOCKET_ID_ERROR;
	}

	int nTesterVersionCheckOff = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_VERSION_CHECK_OFF);
	if (nTesterVersionCheckOff != DEF_ON) {

		for (auto iter = setSocketID.begin(); iter != setSocketID.end(); iter++) {
			int cnt = setSocketID.count(*iter);
			if (cnt > 1) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Socket ID Duplicated %s", (*iter).c_str());
				g_Error.AlarmReport(ERR_TESTER_SOCKET_ID_ERROR, AlarmData, this);
				return ERR_TESTER_SOCKET_ID_ERROR;
			}
		}


		if (strErrorTesterID.IsEmpty() != TRUE)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester ID Error!! [Tester ID is Empty]");
			AlarmData.m_fnAddItemFormat("%s", strErrorTesterID);
			g_Error.AlarmReport(ERR_TESTER_TESTER_ID_ERROR, AlarmData, this);
			return ERR_TESTER_TESTER_ID_ERROR;

		}
	}

	if (vStationID.empty() != TRUE) {
		for (std::vector<int>::iterator it = vStationID.begin(); it != vStationID.end(); it++) {
			for (std::vector<int>::iterator it2 = vStationID.begin(); it2 != vStationID.end(); it2++) {
				if (it != it2 && *it == *it2) {
					strDupID.Format("[%d]", *it2);
				}
			}
		}
		if (strDupID.IsEmpty() != TRUE) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("StationID Occur Duplication, please check station=%s", strDupID);
			g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
			return ERR_RECEIVE_TESTER;
		}
	}

	// not connected
// 	if (strDisconnect.IsEmpty() != TRUE) {
// 		// Running �߿��� OPSwitchAct_START ���� Tester ���� Ȯ�� 
// 		// Amkor �ƴϸ� Disconnect �߻��� Running �Ұ�
// 		// sdh �����ؾ��� [10/5/2021 donghyun.shin]
// 		CParamList AlarmData;
// 		AlarmData.m_fnAddItemFormat("%s", strDisconnect);
// 		g_Error.AlarmReport(ERR_TESTER_DISCONNECTED, AlarmData, this);
// 		return ERR_TESTER_DISCONNECTED;
// 	}

	// Program Mismatch
	if (nTesterVersionCheckOff == DEF_OFF) {
		if (vTesterSW_VersionMisMatch.size() != 0 && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester SW Version MisMatch");
			for (int i = 0;i < (int)vTesterSW_VersionMisMatch.size(); i++)
			{
				AlarmData.m_fnAddItemFormat("%s", vTesterSW_VersionMisMatch[i]);
			}
			g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
			return ERR_RECEIVE_TESTER;
		}

		// HWID Mismatch
		std::unordered_map<std::string, size_t> mapHWIDMisMatch;
		for (size_t i = 0; i < vHWIDMisMatch.size(); ++i) {
			if (mapHWIDMisMatch.find(vHWIDMisMatch[i].sVersion) == mapHWIDMisMatch.end()) {
				mapHWIDMisMatch[vHWIDMisMatch[i].sVersion] = vHWIDMisMatch[i].nIndex;
			}
		}

		if (mapHWIDMisMatch.size() > 1)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester HW Version MisMatch");
			for (std::unordered_map<std::string, size_t>::const_iterator it = mapHWIDMisMatch.begin();
				it != mapHWIDMisMatch.end(); ++it)
			{		
				AlarmData.m_fnAddItemFormat("[Socket No=%d][Tester=%s]", it->second, it->first.c_str());
			}
			g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
			return ERR_RECEIVE_TESTER;
		}		

		// FWHID Mismatch
		std::unordered_map<std::string, size_t> mapFWHIDMisMatch;
		for (size_t i = 0; i < vFWHIDMisMatch.size(); ++i) {
			if (mapFWHIDMisMatch.find(vFWHIDMisMatch[i].sVersion) == mapFWHIDMisMatch.end()) {
				mapFWHIDMisMatch[vFWHIDMisMatch[i].sVersion] = vFWHIDMisMatch[i].nIndex;
			}
		}

		if (mapFWHIDMisMatch.size() > 1)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester FW HID Version MisMatch");
			for (std::unordered_map<std::string, size_t>::const_iterator it = mapFWHIDMisMatch.begin();
				it != mapFWHIDMisMatch.end(); ++it)
			{
				AlarmData.m_fnAddItemFormat("[Socket No=%d][Tester=%s]", it->second, it->first.c_str());
			}
			g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
			return ERR_RECEIVE_TESTER;
		}

		// FWPPO Mismatch
		std::unordered_map<std::string, size_t> mapFWPPOMisMatch;
		for (size_t i = 0; i < vFWPPOMisMatch.size(); ++i) {
			if (mapFWPPOMisMatch.find(vFWPPOMisMatch[i].sVersion) == mapFWPPOMisMatch.end()) {
				mapFWPPOMisMatch[vFWPPOMisMatch[i].sVersion] = vFWPPOMisMatch[i].nIndex;
			}
		}

		if (mapFWPPOMisMatch.size() > 1)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester FW HID Version MisMatch");
			for (std::unordered_map<std::string, size_t>::const_iterator it = mapFWPPOMisMatch.begin();
				it != mapFWPPOMisMatch.end(); ++it)
			{
				AlarmData.m_fnAddItemFormat("[Socket No=%d][Tester=%s]", it->second, it->first.c_str());
			}
			g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
			return ERR_RECEIVE_TESTER;
		}
	}
	//Watt & Carpenter 패키지 적용 사항
	if (vTestScript_Apl.size() != 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test Script MisMatch");
		for (int i=0;i<(int)vTestScript_Apl.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vTestScript_Apl[i]);
		}
		g_Error.AlarmReport(ERR_TESTER_INFO_TESTER_SCRIPT_MISSMATCHE, AlarmData, NULL);
		return ERR_TESTER_INFO_TESTER_SCRIPT_MISSMATCHE;
	}
	//Watt 패키지 적용 사항
	if (vTestSW_Version_Apl.size() != 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test SW Version MisMatch");
		for (int i = 0;i < (int)vTestSW_Version_Apl.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vTestSW_Version_Apl[i]);
		}
		g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, NULL);
		return ERR_RECEIVE_TESTER;
	}
	//Watt 패키지 적용 사항
	if (vTestMode.size() != 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test Mode MisMatch");
		for (int i = 0;i < (int)vTestMode.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vTestMode[i]);
		}
		g_Error.AlarmReport(ERR_TESTER_MODE_MISSMATCHE, AlarmData, NULL);
		return ERR_TESTER_MODE_MISSMATCHE;
	}


	for( auto iter = setTesterID.begin(); iter != setTesterID.end(); iter++ ) {
		int cnt = setTesterID.count( *iter );
		if( cnt > 1 ) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat( "Tester ID Duplicated %s", (*iter).c_str() );
			g_Error.AlarmReport( ERR_RECEIVE_TESTER, AlarmData, this );
			return ERR_RECEIVE_TESTER;
		}
	}
	if (nTesterVersionCheckOff == DEF_OFF) {
		if (nProjectOption != DEF_PROJECT_WATT)
		{
			for (auto iter = setTesterIP.begin(); iter != setTesterIP.end(); iter++) {
				int cnt = setTesterIP.count(*iter);
				if (cnt > 1) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tester IP Duplicated %s", (*iter).c_str());
					g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
					return ERR_RECEIVE_TESTER;
				}
			}
		}
	}

	return ERR_NO_ERROR;
}

int CTaskSystemCtrl::CheckEqpConditionForFirstStart()
{
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	int nErr = 0;
	if (IsLotInfoEmpty() == TRUE) {
		nErr = ERR_LOTINFO_EMPTY;
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}

	ULARGE_INTEGER avail, total, free;
	GetDiskFreeSpaceEx(_T("D:/"), &avail, &total, &free);

	double disk_size = (int)(total.QuadPart >> 20); //MBytes�� ó�� 

	//GBytes total.QuadPart>>30 , KBytes : total.QuadPart>>10
	//disk_free_size;
	double disk_free_size = (int)(free.QuadPart >> 20);
	double dRemainSize = (disk_free_size / disk_size) * 100.0;
	int nLimitInterlock = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_INTERLOCK);
	if (nLimitInterlock >= dRemainSize) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Hard Disk remain space lower, Can not running [Set=%d per, Current=%.2f per]", nLimitInterlock, dRemainSize);
		g_Error.AlarmReport(ERR_HARD_DISK_LOW_INTERLOCK, AlarmData, NULL);
		return ERR_HARD_DISK_LOW_INTERLOCK;
	}

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );

	// nodevice notray interlock
	if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ && nDeviceMode == eNoDeviceNoTray)
	{
		int nTrayNotExist = g_TaskTransfer.GetTrayExist(DEF_NOT_EXIST);
		if (nTrayNotExist != DEF_NOT_EXIST) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("NoDevice/NoTray mode (Transfer head tray detect sensor is on!!! please check!!!)");
			g_Error.AlarmReport(ERR_TRNASFER_TRAY_EXIST, AlarmData, this);
			return ERR_TRNASFER_TRAY_EXIST;
		}

		// check tray on conveyor
		for (int idx = 0; idx < eMaxStackerCount; idx++) {
			BOOL bRearTraySensorExist = g_TaskStacker[idx].GetExistTray_RearPos_Sen(DEF_NOT_EXIST);
			BOOL bFrontTraySensorExist = g_TaskStacker[idx].GetExistTray_FrontPos_Sen(DEF_NOT_EXIST) != ERR_NO_ERROR;
			if ( bFrontTraySensorExist != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("NoDevice/NoTray mode (conveyor [%d], Tray Exist Sensor detect!)", idx + 1);
				g_Error.AlarmReport(ERR_STACKER_1_TRAY_EXIST_FRONT_POS + idx, AlarmData, this);
				return ERR_STACKER_1_TRAY_EXIST_FRONT_POS + idx;
			}
			if ( bRearTraySensorExist != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("NoDevice/NoTray mode (conveyor [%d], Tray Exist Sensor detect!)", idx + 1);
				g_Error.AlarmReport(ERR_STACKER_1_TRAY_EXIST_REAR_POS + idx, AlarmData, this);
				return ERR_STACKER_1_TRAY_EXIST_REAR_POS + idx;
			}

			if (g_TaskStacker[idx].GetEmptyTray() == false) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("NoDevice/NoTray mode (Stacker [%d], Tray Exist Sensor detect!)", idx + 1);
				g_Error.AlarmReport(ERR_STACKER_1_TRAY_EXIST_SENSOR_DETECT + idx, AlarmData, this);
				return ERR_STACKER_1_TRAY_EXIST_SENSOR_DETECT + idx;
			}
		}
	}

	// Online checking loop < network, tester, duplicate, mismatch >
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);


	//Device Name Check
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		MakeLog("CTaskSystemCtrl::Check2dMarkDeviceName Start");

		int nErr = Check2dMarkDeviceName();
		if (nErr != ERR_NO_ERROR)
			return nErr;

		MakeLog("CTaskSystemCtrl::Check2dMarkDeviceName End");
	}
	

	//Online Tester Info.
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		MakeLog("CTaskSystemCtrl::CheckOnlineTesterInfo Start");
		int nErr = CheckOnlineTesterInfo();
		if (nErr != ERR_NO_ERROR)
			return nErr;

		MakeLog("CTaskSystemCtrl::CheckOnlineTesterInfo End");
	}


	BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	BOOL bAutoUseGRR = FALSE;//g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	BOOL bAutoUseAudit = FALSE;						 //int nsiteOff = (bAutoUseGRR == TRUE) ? NDM11_GRR_SocketOnOff_Site1 : NDM11_SocketOnOff_Site1;
	BOOL bAutoUse2DIDSort= FALSE;
							 //int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseGRR = TRUE;
	//Socket On/Off Interlock
	CParamList AlarmData;

	if (bAutoUseGRR) {
		nErr = ChkSocketOnOffInterlock_GRR(AlarmData);
	}
	else if (bAutoUseAudit)
		nErr = ChkSocketOnOffInterlock_AUDIT(AlarmData);
	else {
		nErr = ChkSocketOnOffInterlock(AlarmData);
	}
	
	if (nErr != ERR_NO_ERROR)
	{
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}

	
	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nSiteOnTestPp_1 = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(bAutoUseGRR);
	int nSiteOnTestPp_2 = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(bAutoUseGRR);

	if (bVisionUseMode == TRUE && g_pVisionTestHand_1->IsConnected() == FALSE && nSiteOnTestPp_1 > 0) 
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("test hand 1 vision not connected!!!");
		g_Error.AlarmReport(ERR_VISION_NOT_CONNECT, AlarmData, this);
		return ERR_VISION_NOT_CONNECT;
	}
	if (bVisionUseMode == TRUE && g_pVisionTestHand_2->IsConnected() == FALSE && nSiteOnTestPp_2 > 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("test hand 2 vision not connected!!!");
		g_Error.AlarmReport(ERR_VISION_NOT_CONNECT, AlarmData, this);
		return ERR_VISION_NOT_CONNECT;
	}

	// checking initial transfer tray
	int nTrayNotExist = g_TaskTransfer.GetTrayExist(DEF_NOT_EXIST);
	if (nDeviceMode != eNoDeviceNoTray && nTrayNotExist != DEF_NOT_EXIST && nLotStartMode != eStartMD_InitContinue) {
		nErr = ERR_TRNASFER_TRAY_EXIST;
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Transfer head tray detect sensor is on!!! please check!!!");
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}


	// continue mode check
	if (nLotStartMode == eStartMD_RetestContinue || nLotStartMode == eStartMD_InitContinue
		|| nLotStartMode == eStartMD_QA_Continue || nLotStartMode == eStartMD_AutoGRR_Continue
		|| nLotStartMode == eStartMD_AutoLoop_Continue)
	{
		// check tray on conveyor
		for (int idx = 0; idx < eMaxStackerCount; idx++) {
			bool bRearTraySensorExist = g_TaskStacker[idx].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST) != ERR_NO_ERROR;
			bool bRearTrayDataExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1 + idx) == DEF_EXIST;
			if (bRearTraySensorExist != bRearTrayDataExist) {
				nErr = ERR_LOT_CMD_CONVEYOR_1_SENSOR_DATA_MISMATCH + idx;
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("conveyor [%d], Tray Exist Sensor and Data Mismatch!! please check!!", idx + 1);
				g_Error.AlarmReport(nErr, AlarmData, this);
				return nErr;
			}

			bool bFrontTraySensorExist = g_TaskStacker[idx].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST) != ERR_NO_ERROR;
			bool bFrontTrayDataExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + idx) == DEF_EXIST;
			if (bFrontTraySensorExist != bFrontTrayDataExist) {
				nErr = ERR_LOT_CMD_STACKER_1_BOTTOM_SENSOR_DATA_MISMATCH + idx;
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Stacker Bottom [%d], Tray Exist Sensor and Data Mismatch!! please check!!", idx + 1);
				g_Error.AlarmReport(nErr, AlarmData, this);
				return nErr;
			}
		}

		bool bTransferTraySensorExist = g_TaskTransfer.GetTrayExist_DvcMode(DEF_NOT_EXIST) != ERR_NO_ERROR;
		bool bTransferTrayDataExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_Transfer) == DEF_EXIST;
		if (bTransferTraySensorExist != bTransferTrayDataExist) {
			nErr = ERR_LOT_CMD_TRANSFER_SENSOR_DATA_MISMATCH;
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Transfer, Tray Exist Sensor and Data Mismatch!! please check!!");
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}

		return ERR_NO_ERROR;
	}

	// check tray on conveyor
	for (int idx = 0; idx < eMaxStackerCount; idx++) {
		int nErrRearPos = g_TaskStacker[idx].GetExistTray_RearPos_Sen(DEF_NOT_EXIST);
		if (nErrRearPos != ERR_NO_ERROR) {
			nErr = ERR_LOT_CMD_CONVEYOR_1_IS_NOT_EMPTY + idx;
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("conveyor [%d], Tray Exist Sensor detected!! please check!!", idx + 1);
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}

		int nErrFrontPos = g_TaskStacker[idx].GetExistTray_FrontPos_Sen(DEF_NOT_EXIST);
		if (nErrFrontPos != ERR_NO_ERROR) {
			nErr = ERR_LOT_CMD_STACKER_1_BOTTOM_IS_NOT_EMPTY + idx;
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Stacker Bottom [%d], Tray Exist Sensor detected!! please check!!", idx + 1);
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}

		
		int nUnloadStackerFullChkCount = 0;
		CString strUnloadStackerFullPos = _T( "" );
		eStacker_Type eType = g_TaskAutoStacker[ idx ].GetStackerType();
		switch( eType )
		{
			case eStackerType_Load: 
				{
					if (nDeviceMode != eNoDeviceNoTray)
					{
						if (g_TaskStacker[idx].GetEmptyTray() == true) {
							nErr = ERR_LOT_CMD_LOAD_TRAY_IS_SHORTAGE;
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Load Stacker is empty");
							g_Error.AlarmReport(nErr, AlarmData, this);
							return nErr;
						}
					}
				}break;
			case eStackerType_Empty_Unload: 
				{
					if (nDeviceMode != eNoDeviceNoTray)
					{
						if (g_TaskStacker[idx].GetEmptyTray() == true) {
							nErr = ERR_LOT_CMD_EMPTY_TRAY_IS_SHORTAGE;
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Unload Empty Stacker is empty");
							g_Error.AlarmReport(nErr, AlarmData, this);
							return nErr;
						}
					}					
				}break;
			case eStackerType_Empty_Load :
				{
					if( g_TaskStacker[ idx ].ChkFullTray(DEF_ON) != true ) {
						nErr = ERR_LOT_CMD_LOAD_EMPTY_STACKER_IS_FULL;
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat( "Load Empty Stacker is full" );
						g_Error.AlarmReport( nErr, AlarmData, this );
						return nErr;
					}
				}break;
			case eStackerType_Unload :
				{
					bool bEmpty = g_TaskStacker[ idx ].GetEmptyTray();
					if( bEmpty == false ) {
						nUnloadStackerFullChkCount++;
						CString strTempPos = _T( "" );
						strTempPos.Format( "[Stacker=%d]", idx + 1 );
						strUnloadStackerFullPos += strTempPos;
					}
				}break;
			default: ASSERT( false );
		}

		if( nUnloadStackerFullChkCount != 0 ) {
			nErr = ERR_LOT_CMD_UNLOAD_STACKER_4_IS_NOT_EMPTY - 3 + idx;
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat( "%s", strUnloadStackerFullPos );
			g_Error.AlarmReport( nErr, AlarmData, this );
			return nErr;
		}
	}

	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	{
		if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) == eNoDeviceNoTray) {
			g_DMCont.m_dmDevice.SetTrayExist(eDeviceStage_Transfer, DEF_NOT_EXIST);
		}
	}
	
	bool bTransferTraySensorExist = g_TaskTransfer.GetTrayExist(DEF_NOT_EXIST);
	if (bTransferTraySensorExist != DEF_NOT_EXIST) {
		nErr = ERR_LOT_CMD_TRANSFER_IS_NOT_EMPTY;
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Transfer, Tray Exist Sensor detected!! please check!!");
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}

	//Batch File ���� InterLock
	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_BatchFile_Use) == DEF_ON) {
		char szBatchPathBuff[MAX_PATH] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchPathBuff, MAX_PATH);

		CFileFind findfile;
		BOOL bFind = findfile.FindFile(szBatchPathBuff);
		if (!bFind) {

			findfile.Close();

			nErr = ERR_BATCH_FIEL_NO_USE;
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}

		findfile.Close();
	}

	CString strAlramData;
	char szPath[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_2DID_Sort_FilePath, szPath, MAX_PATH);
	int m_nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

// 	if (m_nFTRCQC == eStartMD_Auto2DIDSort || m_nFTRCQC == eStartMD_Auto2DIDSort_Continue)
// 	{
// 		int nRet = g_BarcodeList.ReadCSVData(szPath, strAlramData);
// 
// 		if (nRet != NO_ERROR)
// 		{
// 			nRet -= 1;
// 			int nErr = ERR_2DID_FILE_NOT_EXIST + nRet;
// 			CParamList Alarm;
// 			Alarm.m_fnAddItemFormat("2DID CSV %s = %s", strAlramData, szPath);
// 			g_Error.AlarmReport(nErr, Alarm, this);
// 			return nErr;
// 		}
// 	}

	for( int conv_idx = 0; conv_idx < eMaxStackerCount; conv_idx++ ) {
		int chkTrayAlignBack = g_TaskStacker[ conv_idx ].chk_cyl_TrayAlign( DEF_BACK );
		if( chkTrayAlignBack != ERR_NO_ERROR ) {
			nErr = ERR_LOT_CMD_CONVEYOR_TRAY_ALIGN_IS_NOT_BACK_STACKER_1 + conv_idx;
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat( "Conveyor : %d", conv_idx +1 );
			g_Error.AlarmReport( nErr, AlarmData, this );
			return nErr;
		}
	}

	for (int i = 0; i <eDeviceDataStageCountMax; i++)
	{
		if (i == eDeviceStage_TEST_PP_1_CLEAN_BUFF || i == eDeviceStage_TEST_PP_2_CLEAN_BUFF)
			continue;
		g_DMCont.m_dmDevice.ClearDeviceData(i);
	}

	return nErr;
}

int CTaskSystemCtrl::CheckEqpSocketSite()
{
    //int nChk = 0;
	//
    //for(int i=0; i<eMaxPressUnitCount/2; i++)
    //{
    //    //0,1 //2,3 // 4,5 // 6,7 // 8,9 //10,11 // 12,13 // 14,15
    //    int nLeft  = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1+(i*2));
    //    int nRight = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1+(i*2)+1);
    //    
    //    if( (nLeft == DEF_SITE_OFF  && nRight == DEF_SITE_OFF) ){
    //    //|| (nLeft == DEF_SITE_OFF      && nRight == DEF_SITE_DISABLE)){
    //        nChk++;
    //    }
    //}    
	//
    //if(nChk != 0){
    //    return ERR_TEST_SITE_ALL_OFF;
    //}

    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::CheckEqpConditionForStart()
{
	int nErr = ERR_NO_ERROR;
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	// check door (door-lock, slide pack ����)
	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, this);
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK;
	}

	int nIonizer = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ALL_UNIT_IONIZER);
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);


	nErr = g_TaskSystemCtrl.ChkComInterlock();
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}
	
	int nLimit = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_ALARM);
	ULARGE_INTEGER avail, total, free;
	GetDiskFreeSpaceEx(_T("D:/"), &avail, &total, &free );

	double disk_size= (int)(total.QuadPart>>20); //MBytes�� ó�� 

	//GBytes total.QuadPart>>30 , KBytes : total.QuadPart>>10
	//disk_free_size;
	double disk_free_size = (int)(free.QuadPart>>20);
	double dRemainSize = (disk_free_size/disk_size) * 100.0;
	int nLimitInterlock = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_INTERLOCK);
	if( nLimitInterlock >= dRemainSize ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Hard Disk remain space lower, Can not running [Set=%d per, Current=%.2f per]", nLimitInterlock, dRemainSize);
		g_Error.AlarmReport(ERR_HARD_DISK_LOW_INTERLOCK, AlarmData, NULL);
		return ERR_HARD_DISK_LOW_INTERLOCK;
	}

	// cp trip
	std::vector<int> vCP_Trip_Alarm_Sen;
	for(std::vector<int>::size_type i=0; i<m_vCPTrip_Sig.size(); i++)
	{
		if( g_IO.in(m_vCPTrip_Sig[i]) != DEF_ON )
		{
			vCP_Trip_Alarm_Sen.push_back( m_vCPTrip_Sig[i] );
		}
	}

	if( !vCP_Trip_Alarm_Sen.empty() )
	{
		CParamList AlarmData;
		for(std::vector<int>::size_type i=0; i<vCP_Trip_Alarm_Sen.size(); i++)
		{
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, vCP_Trip_Alarm_Sen[i], 1, rDiInfo);
			if( rDiInfo.m_fnGetCount() > 0 )
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if( pItem )
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s : %s = %s", pItem->label, pItem->name, (g_IO.in(vCP_Trip_Alarm_Sen[i]))?_T("ON"):_T("OFF") );
				}
			}
		}
		g_Error.AlarmReport(ERR_CP_IS_TRIP, AlarmData, this);
		return ERR_CP_IS_TRIP;
	}

	// Ionizer
	std::vector<int> vIon_Alarm;
	for(std::vector<int>::size_type i=0; i<m_vIonAlarm_Sig.size(); i++)
	{
		BOOL bChkIonAlarm = !g_IO.in(m_vIonAlarm_Sig[i]);
		if(bChkIonAlarm != DEF_ON )
		{
			vIon_Alarm.push_back( m_vIonAlarm_Sig[i] );
		}
	}
	if( !vIon_Alarm.empty() && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		CParamList AlarmData;
		for(std::vector<int>::size_type i=0; i<vIon_Alarm.size(); i++)
		{
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, vIon_Alarm[i], 1, rDiInfo);
			if( rDiInfo.m_fnGetCount() > 0 )
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if( pItem )
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s : %s = %s", pItem->label, pItem->name, (g_IO.in(vIon_Alarm[i]))?_T("ON"):_T("OFF") );
				}
			}
		}
	}

 	// Ionizer fan alarm
	std::vector<int> vIon_Fan_Alarm;
	for(std::vector<int>::size_type i=0; i<m_vIonFanAlarm_Sig.size(); i++)
	{
		if( g_IO.in(m_vIonFanAlarm_Sig[i]) != DEF_ON )
		{
			vIon_Alarm.push_back( m_vIonFanAlarm_Sig[i] );
		}
	}
	if( !vIon_Fan_Alarm.empty() && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		CParamList AlarmData;
		for(std::vector<int>::size_type i=0; i<vIon_Fan_Alarm.size(); i++)
		{
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, vIon_Fan_Alarm[i], 1, rDiInfo);
			if( rDiInfo.m_fnGetCount() > 0 )
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if( pItem )
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s : %s = %s", pItem->label ,pItem->name, (g_IO.in(vIon_Fan_Alarm[i]))?_T("ON"):_T("OFF") );
				}
			}
		}
		if (bRetOneCycle && nIonizer)
		{

		}
		else
		{
			g_Error.AlarmReport(ERR_ION_FAN_ALARM, AlarmData, this);
			return ERR_ION_FAN_ALARM;
		}
	}

	if( g_Error.Get_Jam_Alarm_Set_State() == TRUE )
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_SYS_JAM_ALARM_DOES_NOT_CLEARED, AlarmData, this);
		return ERR_SYS_JAM_ALARM_DOES_NOT_CLEARED;
	}

	// Online checking loop < network, tester, duplicate, mismatch >
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if( nRunMode == eRUN_MODE_ONLINE )
	{
		int nErr = CheckOnlineTesterInfo();
		if( nErr != ERR_NO_ERROR )
			return nErr;
	}
	
	BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	BOOL bAutoUseGRR = FALSE;
	BOOL bAutoUseAudit = FALSE;	
	//g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	//int nsiteOff = (bAutoUseGRR == TRUE) ? NDM11_GRR_SocketOnOff_Site1 : NDM11_SocketOnOff_Site1;

	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseGRR = TRUE;

	//Socket On/Off Interlock
	CParamList AlarmData;
	
	if (bAutoUseGRR) {
		nErr = ChkSocketOnOffInterlock_GRR(AlarmData);
	}
// 	else if (bAutoUseAudit)
// 		nErr = ChkSocketOnOffInterlock_AUDIT(AlarmData);
	else {
		nErr = ChkSocketOnOffInterlock(AlarmData);
	}

	if(nErr!= ERR_NO_ERROR)
	{
		g_Error.AlarmReport(nErr, AlarmData, this);
		return nErr;
	}


	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nSiteOnTestPp_1 = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(bAutoUseGRR);
	int nSiteOnTestPp_2 = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(bAutoUseGRR);

	if (bVisionUseMode == TRUE && g_pVisionTestHand_1->IsConnected() == FALSE && nSiteOnTestPp_1 > 0) 
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("test hand 1 vision not connected!!!");
		g_Error.AlarmReport(ERR_VISION_NOT_CONNECT, AlarmData, this);
		return ERR_VISION_NOT_CONNECT;
	}

	if (bVisionUseMode == TRUE && g_pVisionTestHand_2->IsConnected() == FALSE && nSiteOnTestPp_2 > 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("test hand 2 vision not connected!!!");
		g_Error.AlarmReport(ERR_VISION_NOT_CONNECT, AlarmData, this);
		return ERR_VISION_NOT_CONNECT;
	}

	//BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
	//if( bSocketClean == eOPTION_USE && !(nLotStartMode == eStartMD_AutoGRR)){
	//	int nTestBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
	//	int nTestBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
	//
	//	for(int nIdx=eTestPP_1; nIdx < eMaxTestPPCount; nIdx++){
	//		int nDeviceCount = 0;
	//		BOOL bIsModuleSiteOff = ModuleSiteOffCheck(nIdx);
	//
	//		if(bIsModuleSiteOff !=TRUE){
	//			for(int nY=0; nY < nTestBuff_Y; nY++)
	//			{
	//				for(int nX=0; nX < nTestBuff_X; nX++)
	//				{
	//					nDeviceCount += g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1_BUFF+nIdx, nX, nY); 
	//				}
	//			}
	//
	//			ST_DD_DEVICE stDeviceData;
	//
	//			if(nDeviceCount ==0 ){
	//				for(int TestPpnY =0; TestPpnY < MAX_TEST_PP_CNT_Y; TestPpnY++){
	//					for(int TestPpnX =0; TestPpnX < MAX_TEST_PP_CNT_X; TestPpnX++){
	//						stDeviceData.clear();
	//						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + nIdx, TestPpnX, TestPpnY, &stDeviceData);
	//						if(stDeviceData.sExist == eDeviceData_Exist && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0 ){
	//							nDeviceCount++;
	//							break;
	//						}
	//					}
	//				}
	//			}
	//			if(nDeviceCount ==0 ){
	//				int nPressUnit[4] ={ePressUnit_Down_1,ePressUnit_Down_2,ePressUnit_Up_1,ePressUnit_Up_2};
	//
	//				for(int i =0; i<MAX_TESTSITE_ONE_AREA_PRESS_UNIT; i++){
	//					stDeviceData.clear();
	//					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+(nPressUnit[i] + (nIdx*2)), &stDeviceData);
	//					if(stDeviceData.sExist == eDeviceData_Exist && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0 ){
	//						nDeviceCount++;
	//						break;
	//					}
	//				}
	//			}
	//
	//			if( nDeviceCount == 0){
	//				CParamList AlarmData;
	//				AlarmData.m_fnAddItemFormat("Module=%d, Socket Clean device is 0!!", nIdx+1);
	//				g_Error.AlarmReport(ERR_SOCKET_CLEAN_DEVICE_LOW, AlarmData, this);
	//				return ERR_SOCKET_CLEAN_DEVICE_LOW;
	//			}
	//		}
	//	}
	//}

	CString strErrMsg = CheckMakeSummary();
	if(strErrMsg.IsEmpty() != TRUE){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("%s",strErrMsg);
		g_Error.AlarmReport(ERR_NOT_MAKE_SUMMARY,AlarmData,this);
		return ERR_NOT_MAKE_SUMMARY; 
	}

	CString strAlramData;
	char szPath[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_2DID_Sort_FilePath, szPath, MAX_PATH);
	int m_nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	
	////Auto Retest�� ����� ��� AAB Rule�� ���� Interlock
	//BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	//if(bAutoRetestUseMode == TRUE && bAutoUseGRR != TRUE){
	////	int nSiteOn =0;
	////	BOOL bChkSite = FALSE;
	////	for(int i =0 ; i<eMaxTestPPCount; i++){
	////		nSiteOn =0;
	////		bChkSite = FALSE;
	////		for(int j =0; j<eMaxATS_Area; j++){
	////			nSiteOn = g_DMCont.m_dmHandlerOpt.GN((nsiteOff+g_TaskAutoTestSite[i].m_nPressUnitIdx[j]));
	////			if(nSiteOn == DEF_SITE_ON){
	////				bChkSite = TRUE;
	////				break;
	////			}
	////		}
	////		if(bChkSite == TRUE){
	////			for(int j = 0; j<eMaxATS_Area; j++){
	////				int nErr =g_TaskAutoTestSite[i].CheckAutoRetestRuleSite(j);
	////				if(nErr != ERR_NO_ERROR){
	////					return nErr;
	////				}
	////			}
	////		}
	////	}
	//}

	if (nRunMode == eRUN_MODE_ONLINE) {
		CString strErrMsg = CheckCategoryInterlock(); 
		if (strErrMsg.IsEmpty() != TRUE) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("%s", strErrMsg);
			g_Error.AlarmReport(ERR_CAN_NOT_START_CATEGORY_INTERLOCK, AlarmData, this);
			return ERR_CAN_NOT_START_CATEGORY_INTERLOCK;
		}

		//if (CompareDefaultBinToCategory() != TRUE) {
		//	CParamList AlarmData;
		//	g_Error.AlarmReport(ERR_COMPARE_DEFAULTBIN_TO_CATEGORY_ERROR, AlarmData, this);
		//	return ERR_COMPARE_DEFAULTBIN_TO_CATEGORY_ERROR;
		//}
	}

	// Socket Clean ���� �� Interlock [5/18/2021 donghyun.shin]

// 	nErr = SocketCleanInterlock();
// 	if (nErr != ERR_NO_ERROR)
// 	{
// 		CParamList AlarmData;
// 		/*AlarmData.m_fnAddItemFormat("%s", strErrMsg);*/
// 		g_Error.AlarmReport(nErr, AlarmData, this);
// 		return nErr;
// 	}

	BOOL bGuiOpen = g_DMCont.m_dmHandlerOpt.GB(BDM11_GUI_OPEN_HISTORY);
	if (bGuiOpen == TRUE)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_GUI_IS_NOT_CLOSE, AlarmData, this);
		return ERR_GUI_IS_NOT_CLOSE;
	}

	//
	for (int nTestPpIdx = 0; nTestPpIdx < eMaxTestPPCount; nTestPpIdx++) {
		int nErr = g_TaskTestPP[nTestPpIdx].Interlock_X_Related_Clean_Table_Range();
		if(nErr != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}
	}

	//Test PP Recovery & Press Safety 충돌 영역 확인.
	for (int nTestPpIdx = 0; nTestPpIdx < eMaxTestPPCount; nTestPpIdx++)
	{
		int nErr = g_TaskTestPP[nTestPpIdx].Check_X_Related_Press_Z_Safety();
		if (nErr != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Please Test PP%s Move Safety", (nTestPpIdx == eTestPP_1) ? _T("1") : _T("2"));
			g_Error.AlarmReport(nErr, AlarmData, this);
			return nErr;
		}
	}


// 	if (g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_OFF)
// 	{
// 		for (int i = 0; i < eMaxPressUnitCount; i++)
// 		{
// 			BOOL bTesting = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + i);
// 			int nErr = g_TaskPressUnit[i].ChkDI(CTaskPressUnit::xZAXIS_VIBRATION_CYL_UP_SEN, DEF_ON);
// 			if (nErr != ERR_NO_ERROR && bTesting == FALSE)
// 				g_TaskPressUnit[i].do_VibratorCylUpDown(eCyl_Up);
// 		}	
// 	}

// 	for (int nTestPpIdx = 0; nTestPpIdx < eMaxTestPPCount; nTestPpIdx++) {
// 		//Safety Sen
// 		int nSafetyX1Sen = g_TaskTestPP[nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
// 
// 		//Safety Pos
// 		double dCmdPos[CTaskTestPP::eAxisMax] = { 0, };
// 		std::vector<CPoint> vTempPkr;
// 		vTempPkr.push_back(CPoint(0, 0));
// 		g_TaskTestPP[nTestPpIdx].GetTeachPos(LOC_IDX_CLEAN_TBL, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dCmdPos, eTestPPLoadType);
// 
// 		OneAxis* pMotorX1 = g_TaskTestPP[nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
// 		double dCurPosX1 = 0.0;
// 		pMotorX1->GetCurrentPos(dCurPosX1);
// 
// 		BOOL bIsX1SafetyPos = (nTestPpIdx == eTestPP_1) ? dCurPosX1 < dCmdPos[CTaskTestPP::eAxisX1] : dCurPosX1 > dCmdPos[CTaskTestPP::eAxisX1];
// 
// 		double dCurPosY = 0.0;
// 		OneAxis* pMotorY = g_TaskTestPP[nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
// 		pMotorY->GetCurrentPos(dCurPosY);
// 
// 		if (dCurPosY <= INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD)
// 		{
// 			if (nSafetyX1Sen != ERR_NO_ERROR || bIsX1SafetyPos != TRUE) {
// 				CParamList AlarmData;
// 				g_Error.AlarmReport(ERR_TEST_PP_1_X1_IS_NOT_SAFETY_IN_CLEANTBL_POS + nTestPpIdx, AlarmData, this);
// 				return ERR_TEST_PP_1_X1_IS_NOT_SAFETY_IN_CLEANTBL_POS + nTestPpIdx;
// 			}
// 		}		
// 	}



	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::GetCurrentLotState()
{
	int nLotState = g_DMCont.m_dmEQP.GN(NDM0_LotStatus);
	return nLotState;
}


//==============================================================================
//
//==============================================================================
void CTaskSystemCtrl::SetCurrentLotState(int nLotState)
{
	g_DMCont.m_dmEQP.SN(NDM0_LotStatus, nLotState);

    // Lot Status change :: Idle or OnGoing
    if(nLotState == eAutoLotState_InitNeed){
        g_DMCont.m_dmEQP.SB(BDM0_SYS_LOT_STATUS, eHandlerLotMode_Idle);
    }else{
        g_DMCont.m_dmEQP.SB(BDM0_SYS_LOT_STATUS, eHandlerLotMode_OnGoing);
    }

	switch(nLotState)
	{
	case eAutoLotState_InitNeed :
//	case eAutoLotState_Idle:
	case eAutoLotState_Pause:
	case eAutoLotState_AlarmStop:
	case eAutoLotState_NormalComplete:
//	case eAutoLotState_AutoGrrComplete:
		{
			g_DMCont.m_dmEQP.SB(BDM0_EQP_RUN_STATE, FALSE);
		}break;
	default :
		{
			g_DMCont.m_dmEQP.SB(BDM0_EQP_RUN_STATE, TRUE);
		}break;
	}

	SendEvent(CEID_LOT_STATUS_CHANGED);
}


//==============================================================================
// ����� Operation Panel�� 'Start' ��ư�� ���� �Ϳ� ���� �̺�Ʈ ó��
//==============================================================================
int CTaskSystemCtrl::OnPanelButtonStart()
{
	int nCurrentLotState = eAutoLotState_Unknown;
	std::vector<int> vLotState;

	for( int i = 0; i<eMaxStackerCount; i++ ) {
		nCurrentLotState = g_TaskAutoStacker[ eSTACKER_01 + i ].GetCurrentLotState();
		vLotState.push_back( nCurrentLotState );
	}
	nCurrentLotState = g_TaskAutoTransfer.GetCurrentLotState();
	vLotState.push_back(nCurrentLotState);
	
	nCurrentLotState = g_TaskAutoTrayUnloadPP.GetCurrentLotState();
	vLotState.push_back(nCurrentLotState);

	nCurrentLotState = g_TaskAutoTrayLoadPP.GetCurrentLotState();
	vLotState.push_back(nCurrentLotState);

	nCurrentLotState = g_TaskAutoTrayFeeder.GetCurrentLotState();
	vLotState.push_back(nCurrentLotState);

	for(int i=0; i<eMaxTestPPCount; i++){
		nCurrentLotState = 	g_TaskAutoTestPp[eTestPP_1+i].GetCurrentLotState();
		vLotState.push_back(nCurrentLotState);
	} 
	for (int i = 0; i < eMaxLoadTblCount; i++) {
		nCurrentLotState = g_TaskAutoLdTable[eLoadTbl_1 + i].GetCurrentLotState();
		vLotState.push_back(nCurrentLotState);
	}

	for( int i = 0; i < eMaxPressUnitCount; i++ ) {
		nCurrentLotState = g_TaskAutoPress[ ePressUnit_Down_1+ i ].GetCurrentLotState();
		vLotState.push_back( nCurrentLotState );
	}

	for(std::vector<int>::size_type i=0; i<vLotState.size(); i++){
		if( vLotState[ i ] == eAutoLotState_InitNeed ) {
			nCurrentLotState = eAutoLotState_InitNeed;
			break;
		}
		if( vLotState[i] == eAutoLotState_AlarmStop
			|| vLotState[ i ] == eAutoLotState_Pause ){
			nCurrentLotState = eAutoLotState_Pause;
			break;
		}
	}
	for(std::vector<int>::size_type i=0; i<vLotState.size(); i++){
		if( eAutoLotState_Unknown == vLotState[ i ] ) {
			nCurrentLotState = eAutoLotState_Unknown;
			break;
		}
	}

	eAutoLotCmd _eAutoLotCmd = eAutoLotCmd_None;
	switch( nCurrentLotState )
	{
//	case eAutoLotState_AlarmStop:
	case eAutoLotState_Pause:
		{
			_eAutoLotCmd = eAutoLotCmd_Resume;
		}break;
//	case eAutoLotState_Idle:
	case eAutoLotState_InitNeed:
		{
			_eAutoLotCmd = eAutoLotCmd_Initial;
		}break;
	default:
		return -1;
	}

	int nErr = OnLotCommand(_eAutoLotCmd, NULL, NULL);
	return nErr;
}

//==============================================================================
// ����� Operation Panel�� 'Stop' ��ư�� ���� �Ϳ� ���� �̺�Ʈ ó��
//==============================================================================
int CTaskSystemCtrl::OnPanelButtonStop()
{
	int nErr = OnLotCommand(eAutoLotCmd_Pause, (WPARAM)NULL, (LPARAM)NULL);
	return nErr;
}



int CTaskSystemCtrl::GetWMX3ReconnectSts()
{
	return m_nWM3ReconnectSts;
}

void CTaskSystemCtrl::SetWMX3ReconnectSts(int nSts)
{
	m_nWM3ReconnectSts = nSts;
}

//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::AddLotInfo(CLotInfo& a_LotInfo)
{
	int nErr = 0;
	m_csLotInfo.Lock();
	CLotInfo* pNewLot = new CLotInfo;
	*pNewLot = a_LotInfo;
	m_LotInfoList.AddTail(pNewLot);
	m_csLotInfo.Unlock();
	return nErr;
}


//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::DelLotInfo(CString strLotNo)
{
	int nErr = 0;

	m_csLotInfo.Lock();

	int nLotCount = m_LotInfoList.GetCount();
	POSITION pos = m_LotInfoList.GetHeadPosition();
	POSITION pos_pre;

	for(int i=0; i<nLotCount; i++)
	{
		pos_pre = pos;
		CLotInfo* pItem = (CLotInfo*)m_LotInfoList.GetNext(pos);

		if( pItem->strLotNo == strLotNo )
		{
			m_LotInfoList.RemoveAt(pos_pre);
			delete pItem;
			pItem = NULL;
			break;
		}
	}

	m_csLotInfo.Unlock();

	return nErr;
}


//==============================================================================
//
//==============================================================================
CLotInfo* CTaskSystemCtrl::GetLotInfo(CString strLotNo)
{
	CLotInfo* pRetLotInfo = NULL;

	m_csLotInfo.Lock();

	int nLotCount = m_LotInfoList.GetCount();
	POSITION pos = m_LotInfoList.GetHeadPosition();

	for(int i=0; i<nLotCount; i++)
	{
		CLotInfo* pItem = (CLotInfo*)m_LotInfoList.GetNext(pos);

		if( pItem->strLotNo == strLotNo )
		{
			pRetLotInfo = pItem;
			break;
		}
	}

	m_csLotInfo.Unlock();

	return pRetLotInfo;
}


//==============================================================================
//
//==============================================================================
void CTaskSystemCtrl::ClearLotInfo()
{
	m_csLotInfo.Lock();

	int nLotCount = m_LotInfoList.GetCount();
	POSITION pos = m_LotInfoList.GetHeadPosition();
	for(int i=0; i<nLotCount; i++)
	{
		CLotInfo* pItem = (CLotInfo*)m_LotInfoList.GetNext(pos);
		delete pItem;
		pItem = NULL;
	}

	m_LotInfoList.RemoveAll();

	m_csLotInfo.Unlock();
}


//==============================================================================
//
//==============================================================================
int CTaskSystemCtrl::GetLotInfoCount()
{
	m_csLotInfo.Lock();
	int nCount = m_LotInfoList.GetCount();
	m_csLotInfo.Unlock();
	return nCount;
}

//void CTaskSystemCtrl::SetAddIntervalCnt(int nPressIdx)
//{
//    m_nCleanIntervalCnt[nPressIdx]++;
//}

//void CTaskSystemCtrl::ClearCleanData()
//{
//    // clear data
//    for(int i=0; i < eMaxPressUnitCount; i++){
//        m_nCleanIntervalCnt[i] = 0;
//		m_stTestedBin[i].nPassBinCnt = 0;
//		m_stTestedBin[i].nRevDataCnt = 0;
//		
//    }
//}

//void CTaskSystemCtrl::SetSocClean_TestedBinData( int nPressIdx, BOOL bPassFail)
//{
//    if(bPassFail == eGoodBin){
//        m_stTestedBin[nPressIdx].nPassBinCnt += 1;
//    }
//    m_stTestedBin[nPressIdx].nRevDataCnt += 1;
//}


/*=============================================================================
  AUTHOR   : �輱��
  NAME     : 
  PARAMS   : NONE
  RETURN   : TRUE  = Need Socket Clean
             FALSE = Don't need socket clean
  COMMENTS : Socket Cleaning�� �ʿ����� ���ؼ� Ȯ���Ѵ�.
==============================================================================*/
//BOOL CTaskSystemCtrl::ChkSocketCleanMode()
//{
//    if(g_TaskTrayPP[m_eTrayIdx].m_bOneCycleCmd == TRUE) { return FALSE; }
//
//    BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
//    BOOL bFn_Interval = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanIntervalCnt);
//    BOOL bFn_LowYield = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanLowYield);
//
//    int  nIntervalCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnIntervalCnt);
//    int  nAlarmCnt    = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnAlarmCnt);
//    double dLowYield  = g_DMCont.m_dmHandlerOpt.GD(DDM11_SockClnLowYield);
//
//    if( bSocketClean == DEF_ON )
//    {
//        // ���� interval count���� contact ���� ���� ��� socket cleaning
//        if(bFn_Interval){
//            for(int i=0; i < eMaxPressUnitCount; i++){
//                if(nIntervalCnt < m_nCleanIntervalCnt[i]){
//                    return TRUE;
//                }
//            }
//        }
//        if(bFn_LowYield){
//            for(int i=0; i < eMaxPressUnitCount ; i++){
//                double dCalcYield = (double)m_stTestedBin[i].nPassBinCnt / (double)m_stTestedBin[i].nRevDataCnt * 100.0;
//                if(dCalcYield < dLowYield){
//                    return TRUE;
//                }
//            }            
//        }
//
////         if( g_TaskAutoStacker.GetSocClnOptFinish() == TRUE)
////         {
////             return TRUE;
////         }
//    }
//
//    return FALSE;
//}

void CTaskSystemCtrl::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	int nAlid = wParam;
	switch( nAlid )
	{
	case ERR_2DBARCODE_NO_CHECK_MODE_ON:
		{
			g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);			
			OnRcv_Req_2D_Duplicate_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_LOT_MSG_REQ_ADD_LOAD_TRAY:
		{
			g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);
			OnRcv_Req_Add_Load_Tray_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_LOT_MSG_ONECYCLE_COMPLETE:
		{
            g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_CYCLE_STOP);

            OnRcv_Req_OneCycle_Cmd(nOperatorCommand, wParam, lParam);		
		}break;
	case ERR_GRR_MSG_AUTOGRR_ONECYCLE_COMPLETE:
		{
			g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_CYCLE_STOP);

			OnRcv_Req_AutoGrr_OneCycle_Cmd(nOperatorCommand, wParam, lParam);
		}break;
    case ERR_LOT_STACKER_FOR_BACK:
        {
            OnRcv_Req_Stacker_For_Back_Ack(nOperatorCommand, wParam, lParam);
        }break;
    case ERR_LOT_MSG_TRAY_END_FEED:
        {
			OnRcv_Req_Tray_End_Feed(nOperatorCommand, wParam, lParam);
        }break;
	case ERR_SYSTEM_AUTO_HEGITH_CAL_FINISH:
		{
			OnRcv_Req_AutoHeightCal_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_SYSTEM_ZPicker_Axis_HEGITH_CAL_FINISH:
		{
			OnRcv_Req_ZPickerAxisHeightCal_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_LOT_MSG_LOT_NORMAL_COMPLETE:
		{
			OnRcv_Req_Lot_End(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_TRAY_PP_1_DEVICE_MISS:
	case ERR_TRAY_PP_2_DEVICE_MISS:
		{
			int nTrayPPIdx = nAlid - ERR_TRAY_PP_1_DEVICE_MISS;
			OnRcv_Req_TrayPp_Device_Loss(nOperatorCommand, (WPARAM)nTrayPPIdx, lParam);
		}break;
	case ERR_2DBARCODE_NO_USE:
		{
			OnRcv_Req_2D_Use_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_2DBARCODE_MARK_NO_USE:
	{
		OnRcv_Req_2D_Mark_Use_Ack(nOperatorCommand, wParam, lParam);
	}break;
	case ERR_OFFLINE_USE:
		{
			OnRcv_Req_OffLine_Use_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_ONLINE_USE:
		{
			OnRcv_Req_OnLineMode_Use_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_TEST_PP_1_Z1_DEVICE_MISS:
	case ERR_TEST_PP_1_Z2_DEVICE_MISS:
		{
			int nTestIdx = eTestPP_1;

			int nHandType = (nAlid == ERR_TEST_PP_1_Z1_DEVICE_MISS) ? eTestPPLoadType : eTestPPUnLoadType;

			OnRcv_Req_TestPp_Device_Loss(nOperatorCommand, (WPARAM)(nTestIdx), LPARAM(nHandType));
		}break;
	case ERR_TEST_PP_2_Z1_DEVICE_MISS:
	case ERR_TEST_PP_2_Z2_DEVICE_MISS:
		{
			int nTestIdx = eTestPP_2;

			int nHandType = (nAlid == ERR_TEST_PP_2_Z1_DEVICE_MISS) ? eTestPPLoadType : eTestPPUnLoadType;

			OnRcv_Req_TestPp_Device_Loss(nOperatorCommand, (WPARAM)(nTestIdx), LPARAM(nHandType));
		}break;
	case ERR_VISION_OFF:
		{
			OnRcv_Req_Vision_Off_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_DEVICE_MODE_NO_USE:
		{
			OnRcv_Req_Device_Mode_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_RETEST_SKIP_MODE_USE:
		{
			OnRcv_Req_Retest_Skip_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN1:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN2:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN3:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN4:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN5:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN6:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN7:
	case ERR_TESTER_MAX_TEST_TIME_OVER_DN8:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP1:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP2:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP3:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP4:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP5:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP6:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP7:
	case ERR_TESTER_MAX_TEST_TIME_OVER_UP8:
		{
			OnRcv_Req_Test_Time_Out_Ack(nOperatorCommand, wParam, lParam, nAlid - ERR_TESTER_MAX_TEST_TIME_OVER_DN1);
		}break;
	case ERR_PRESS_ARRIVE_SENSOR_NOT_USE:
		{
			OnRcv_Req_Press_Arrive_Sensor_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_TRAY_PP_1_DEVICE_AUTO_SKIP_PICK_FAIL_OVER:
	case ERR_TRAY_PP_2_DEVICE_AUTO_SKIP_PICK_FAIL_OVER:
	case ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN:
	case ERR_TRAY_PP_2_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN:
		{
			OnRcv_Req_Remain_AutoSkipDevice_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_STATION_DISCONNECTION:
		{
			OnRcv_Req_DisConnectionStation_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_SOCKET_CLEAN_TABLE_1_DEVICE_LIFE_OVER:
	case ERR_SOCKET_CLEAN_TABLE_2_DEVICE_LIFE_OVER:
		{
			OnRcv_Req_SocketCleaning_Dvc_Clear_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_AUTO_RETEST_NOT_USE:
		{
			OnRcv_Req_Auto_Retest_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_SWITCH_TRAY_FEEDER:
		{
			OnRcv_Req_Switch_Tray_Feeder_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_CB_DATA_MODIFY_TIME_OVER:
		{
			OnRcv_Req_CB_Modify_Time_Check_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_CB_DATA_ONOFF:
		{
			OnRcv_Req_CB_Data_OnOff_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_CB_FILE_NOT_EXIST:
		{
			OnRcv_Req_CB_File_Not_Exist_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_AUTO_GRR_MODE_USE:
		{
			OnRcv_Req_GRR_Mode_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_AUTO_LOOP_MODE_USE:
		{
			OnRcv_Req_LOOP_Mode_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_AUTO_AUDIT_MODE_USE:
		{
			OnRcv_Req_AUDIT_Mode_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_AUTO_QA_MODE_USE:
		{
			OnRcv_Req_QA_Mode_Ack(nOperatorCommand, wParam, lParam);
		}break;
	case ERR_QA_MODE_QTY_MISMATCH:
		{
		if (nOperatorCommand == TWMB_ID_QA_QTY) {
			g_GuiComm.SendQAQTYShow();
		}
		
		}break;
	}
}

void CTaskSystemCtrl::OnRcv_Req_Add_Load_Tray_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if( nOperatorCommand == TWMB_ID_CANCEL )
	{
		g_TaskAllAutoStacker.SetLoadTraySupplyEnd(true);
		g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, FALSE);
        //g_TaskAutoStacker.SetSocClnOptFinish(TRUE);
		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		if(nLotStartMode == eStartMD_QA_Continue || nLotStartMode == eStartMD_QA){
			int nLoadDeviceCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_QaSampleCount);
			g_DMCont.m_dmEQP.SN(NDM0_QaSampleLoadCnt, nLoadDeviceCnt);
		}
	}
}

void CTaskSystemCtrl::OnRcv_Req_Stacker_For_Back_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
    if( nOperatorCommand == TWMB_ID_CANCEL )
    {
        //g_TaskAutoStacker.SetStackerForBack(DEF_BACK);
    }
    else{
        //g_TaskAutoStacker.SetStackerForBack(DEF_FORW);
    }
}

void CTaskSystemCtrl::OnRcv_Req_Tray_End_Feed(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	if (bAutoGrrUse != DEF_ON) {
		for (int i = 0; i < eMaxTestPPCount; i++)
			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + i, -1);
	}

    if( nOperatorCommand == TWMB_ID_TRAYEND){
		g_TaskAllAutoStacker.SetTrayEndFeed(DEF_LOTEND_TRAYEND);
		g_TaskSystemCtrl.MakeSummarySetTrayEndFeed(DEF_LOTEND_TRAYEND);
    }else if( nOperatorCommand == TWMB_ID_TRAYFEED){
		g_TaskAllAutoStacker.SetTrayEndFeed(DEF_LOTEND_TRAYFEED);
		g_TaskSystemCtrl.MakeSummarySetTrayEndFeed(DEF_LOTEND_TRAYFEED);
    }else{
		g_TaskAllAutoStacker.SetTrayEndFeed( DEF_LOTEND_TRAYSKIP );
		
		

		/*if( g_TaskAutoStacker.GetLotHistoryState() != eAutoLotState_AutoGrr )
		{
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Pause, NULL, eEqpCtrlMode_Auto);
			if( g_TaskAutoStacker.GetLotHistoryState() >= eAutoLotState_QaSample && g_TaskAutoStacker.GetLotHistoryState() <= eAutoLotState_QaSample_CleanOut )
			{	
				g_TaskAutoTestSite[0].SetLotHistoryState(eAutoLotState_QaSample);
				g_TaskAutoTestSite[1].SetLotHistoryState(eAutoLotState_QaSample);
				g_TaskAutoTestSite[2].SetLotHistoryState(eAutoLotState_QaSample);
				g_TaskAutoTestSite[3].SetLotHistoryState(eAutoLotState_QaSample);
				g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_QaSample);
				g_TaskAutoTrayLD.m_nAutoRunBackUpMainStep = eAutoLotState_QaSample;
				g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_QaSample);
			}else{
				g_TaskAutoTestSite[0].SetLotHistoryState(eAutoLotState_Executing);
				g_TaskAutoTestSite[1].SetLotHistoryState(eAutoLotState_Executing);
				g_TaskAutoTestSite[2].SetLotHistoryState(eAutoLotState_Executing);
				g_TaskAutoTestSite[3].SetLotHistoryState(eAutoLotState_Executing);
				g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_Executing);
				g_TaskAutoTrayLD.m_nAutoRunBackUpMainStep = eAutoLotState_Executing;
				g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Executing);
			}
			g_TaskAutoStacker.SetLoadTraySupplyEnd(FALSE);
		}*/
	}
}

void CTaskSystemCtrl::OnRcv_Req_AutoHeightCal_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if( nOperatorCommand == TWMB_ID_CANCEL ){
		m_nAutoHeightCalSelect = DEF_AUTO_HEIGHT_CAL_CANCEL;
	} else {
		m_nAutoHeightCalSelect = DEF_AUTO_HEIGHT_CAL_SAVE;
	}
}

void CTaskSystemCtrl::OnRcv_Req_ZPickerAxisHeightCal_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_CANCEL) {
		m_nZPickerAxisHeightCalSelect = DEF_ZAXIS_PICKER_HEIGHT_CAL_CANCEL;
	}
	else {
		m_nZPickerAxisHeightCalSelect = DEF_ZAXIS_PICKER_HEIGHT_CAL_SAVE;
	}
}

void CTaskSystemCtrl::OnRcv_Req_2D_Duplicate_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if( nOperatorCommand == TWMB_ID_CANCEL )
	{
		g_TaskAllAutoStacker.Set2dDuplicateIgnore(DEF_REQ_CANCEL);
	}
	if( nOperatorCommand == TWMB_ID_OK )
	{
		g_TaskAllAutoStacker.Set2dDuplicateIgnore(DEF_REQ_OK);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Lot_End(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if(nOperatorCommand == TWMB_ID_OK){
		SendEvent(CEID_SOUND_SW_DOWN);
		g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_FINISH);
		do_Buzzer(0);
	}
}

int CTaskSystemCtrl::GetAutoHeightCalSelect()
{
	return m_nAutoHeightCalSelect;
}

void CTaskSystemCtrl::SetAutoHeightCalSelect(int nSelectType)
{
	m_nAutoHeightCalSelect = nSelectType;
}


int CTaskSystemCtrl::GetZPickerAxisHeightCalSelect()
{
	return m_nZPickerAxisHeightCalSelect;
}

void CTaskSystemCtrl::SetZPickerAxisHeightCalSelect(int nSelectType)
{
	m_nZPickerAxisHeightCalSelect = nSelectType;
}



/************************************************************************/
// One Cylce Complete ���Ŀ� Retry
/************************************************************************/
void CTaskSystemCtrl::OnRcv_Req_OneCycle_Cmd(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	if (bAutoGrrUse != DEF_ON) {
		for (int i = 0; i < eMaxTestPPCount; i++)
			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + i, -1);
	}

	// lot load time 1ȸ ��� ���� �ʴ´�.
	for (int i = 0; i < eMaxPressUnitCount; i++)
		g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + i, 0);

	if( nOperatorCommand == TWMB_ID_RETRY )
	{
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);
		Sleep(1000);
		g_TaskSystemCtrl.SetOneCycleMode( DEF_OFF );
		g_TaskSystemCtrl.SetCleanDeviceSkipOneCycleMode(DEF_OFF);
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Retry, NULL, NULL);
		if( bAutoGrrUse != DEF_ON){
			// LoadTbl Arrived time
			CTime time = CTime::GetCurrentTime();
			g_TaskLoadTbl[eLoadTbl_1].SetMoveSetPlateTm(time);
			g_TaskLoadTbl[eLoadTbl_4].SetMoveSetPlateTm(time + 100);

		}else {
			//g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_AutoGrr);
			//for(int i=0; i < eMaxTestPPCount; i++){
			//	g_TaskAutoTestSite[eTestPP_1+i].SetLotHistoryState(eAutoLotState_AutoGrr);
			//}
		}
        

		// onecycle after check vision reset
		BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
		if( bUseNoUse == DEF_ON)
		{
			for(int i=0; i<eMaxPressUnitCount; i++)
			{
				for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
				{
					for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
					{
						g_TaskPressUnit[i].SetSiteDvcTestingInfo(nX, nY, eTestSite_Empty);
					}
				}
			}
		}

		// Skip 눌렀을땐 다시 재시작 개념으로, Table을 Load Site로 초기화 해준다.
		for (int i = 0; i < eMaxLoadTblCount; i++) {
			g_TaskLoadTbl[i].SetWorkReserve(RID_INIT_IDLE_RESERVE);
		}

		// Press SOT,EOT Time 초기화.
		SYSTEMTIME sysTimeInit;
		::GetLocalTime(&sysTimeInit);
		for (int i = 0; i < eMaxPressUnitCount; i++) {
			g_SOT[i] = sysTimeInit;
			g_EOT[i] = sysTimeInit;
		}

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_RUNNING);
	}
	else if( nOperatorCommand == TWMB_ID_SKIP )
	{
		g_TaskSystemCtrl.SetOneCycleMode( DEF_OFF );
		g_TaskSystemCtrl.SetCleanDeviceSkipOneCycleMode(DEF_OFF);
		
		g_TaskSystemCtrl.m_dBestTestTime = 0.0;

	

        // Onecycle complete ���Ŀ� Site ���� ���� �ϵ��� �Ѵ�.
        {
            g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE);
        }

		// onecycle after check vision reset
		BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
		if( bUseNoUse == DEF_ON)
		{
			for(int i=0; i<eMaxPressUnitCount; i++)
			{
				for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
				{
					for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
					{
						g_TaskPressUnit[i].SetSiteDvcTestingInfo(nX, nY, eTestSite_Empty);
					}
				}
			}
		}
		
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Skip, NULL, NULL);
		if( bAutoGrrUse != DEF_ON){			
			//if( g_TaskAutoStacker.GetLotHistoryState() >= eAutoLotState_QaSample && g_TaskAutoStacker.GetLotHistoryState() <= eAutoLotState_QaSample_CleanOut ){
			//	g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_QaSample);
			//	for(int i=0; i < eMaxTestPPCount; i++){
			//		g_TaskAutoTestSite[eTestPP_1+i].SetLotHistoryState(eAutoLotState_QaSample);
			//		g_TaskTrayPP[m_eTrayIdx].SetCYL_2DResrv(eLoadTbl_1+i, FALSE);
			//	}
			//}else{
			//	g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_Executing);
			//	for(int i=0; i < eMaxTestPPCount; i++){
			//		g_TaskAutoTestSite[eTestPP_1+i].SetLotHistoryState(eAutoLotState_Executing);
			//		g_TaskTrayPP[m_eTrayIdx].SetCYL_2DResrv(eLoadTbl_1+i, FALSE);
			//	}
			//}
			

			// LoadTbl Arrived time
			CTime time = CTime::GetCurrentTime();
			g_TaskLoadTbl[eLoadTbl_1].SetMoveSetPlateTm(time);
			g_TaskLoadTbl[eLoadTbl_2].SetMoveSetPlateTm(time + 100);
		}else {

			//g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_AutoGrr);
			//for(int i=0; i < eMaxTestPPCount; i++){
			//	g_TaskAutoTestSite[eTestPP_1+i].SetLotHistoryState(eAutoLotState_AutoGrr);
			//}
		}

		// Skip 눌렀을땐 다시 재시작 개념으로, Table을 Load Site로 초기화 해준다.
		for (int i = 0; i < eMaxLoadTblCount; i++) {
			g_TaskLoadTbl[i].SetWorkReserve(RID_INIT_IDLE_RESERVE);
		}

		// Press SOT,EOT Time 초기화.
		SYSTEMTIME sysTimeInit;
		::GetLocalTime(&sysTimeInit);
		for (int i = 0; i < eMaxPressUnitCount; i++) {
			g_SOT[i] = sysTimeInit;
			g_EOT[i] = sysTimeInit;
		}

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_PAUSE);
	}
	else /*nOperatorCommand == TWMB_ID_CLEANOUT*/
	{
		g_TaskAllAutoStacker.SetLoadTraySupplyEnd( true ); 
		g_TaskAllAutoStacker.SetOneCycleCleanOut( true );
		g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, FALSE);

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_RUNNING);
	}
}

/************************************************************************/
// Auto GRR One Cylce Complete ���Ŀ� Retry
/************************************************************************/
void CTaskSystemCtrl::OnRcv_Req_AutoGrr_OneCycle_Cmd(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if( nOperatorCommand == TWMB_ID_RETRY )
	{		
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Retry, NULL, NULL);
		//g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_AutoGrr);
		//for(int i=0; i < eMaxTestPPCount; i++){
		//	g_TaskAutoTestSite[i].SetLotHistoryState(eAutoLotState_AutoGrr);
		//}

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_RUNNING);
	}
	else if( nOperatorCommand == TWMB_ID_SKIP )
	{
		// Onecycle complete ���Ŀ� Site ���� ���� �ϵ��� �Ѵ�.
		{
			g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE);
		}

		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Skip, NULL, NULL);
		//g_TaskAutoStacker.SetLotHistoryState(eAutoLotState_AutoGrr);
		//for(int i=0; i < eMaxTestPPCount; i++){
		//	g_TaskAutoTestSite[i].SetLotHistoryState(eAutoLotState_AutoGrr);
		//}

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_PAUSE);
	}
	else /*nOperatorCommand == TWMB_ID_CLEANOUT*/
	{
		g_TaskAllAutoStacker.SetLoadTraySupplyEnd( true );
		g_TaskAllAutoStacker.SetOneCycleCleanOut( true );
		g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, FALSE);

		// AutoGRR Use�� OFF�Ѵ�
		g_TaskSystemCtrl.SetAutoGrrUse(FALSE);
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_CleanOut, NULL, NULL);

		g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_RUNNING);
	}
}

void CTaskSystemCtrl::OnRcv_Req_TrayPp_Device_Loss(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if( nOperatorCommand == TWMB_ID_SKIP )
	{
		int nIdx = (int)wParam;
		if (nIdx == eTrayPP_1_LD) {
			g_TaskTrayLoadPP.LossDevice();
		}else if( nIdx == eTrayPP_2_UD){
			g_TaskTrayUnloadPP.LossDevice();
		}
	}
}

void CTaskSystemCtrl::OnRcv_Req_2D_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if(nOperatorCommand == TWMB_ID_OK){
		//g_TaskAutoStacker.Set2dUseIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.Set2dUseIgnore(DEF_REQ_OK);
	}else if(nOperatorCommand == TWMB_ID_CANCEL){
		//g_TaskAutoStacker.Set2dUseIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.Set2dUseIgnore(DEF_REQ_CANCEL);
	}

}

void CTaskSystemCtrl::OnRcv_Req_2D_Mark_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam) {
	if (nOperatorCommand == TWMB_ID_OK) {
		//g_TaskAutoStacker.Set2dUseIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.Set2dMarkUseIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		//g_TaskAutoStacker.Set2dUseIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.Set2dMarkUseIgnore(DEF_REQ_CANCEL);
	}

}

void CTaskSystemCtrl::OnRcv_Req_OffLine_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if(nOperatorCommand == TWMB_ID_OK){
		//g_TaskAutoStacker.SetOffLineUseIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_OK);
	}else if(nOperatorCommand == TWMB_ID_CANCEL){
		//g_TaskAutoStacker.SetOffLineUseIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_OnLineMode_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_TestPp_Device_Loss(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if( nOperatorCommand == TWMB_ID_SKIP ){
		int nIdx = GetErrTestPpIdx( (int)wParam);
		if(nIdx != -1){
			int nHandType = (int)lParam;
			g_TaskAutoTestPp[(int)wParam].LossDevice(nHandType);
		}
	}
	SetErrTestPpIdx((int)wParam , -1);
}

void CTaskSystemCtrl::OnRcv_Req_Vision_Off_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if(nOperatorCommand == TWMB_ID_OK){
		//g_TaskAutoStacker.SetVisionOffIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.SetVisionOffIgnore(DEF_REQ_OK);
	}else if(nOperatorCommand == TWMB_ID_CANCEL){
		//g_TaskAutoStacker.SetVisionOffIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.SetVisionOffIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Device_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if(nOperatorCommand == TWMB_ID_OK){
		//g_TaskAutoStacker.SetDeviceModeIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.SetDeviceModeIgnore(DEF_REQ_OK);
	}else if(nOperatorCommand == TWMB_ID_CANCEL){
		//g_TaskAutoStacker.SetDeviceModeIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.SetDeviceModeIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Retest_Skip_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam){
	if(nOperatorCommand == TWMB_ID_OK){
		//g_TaskAutoStacker.SetRetestSkipIgnore(DEF_REQ_OK);
		g_TaskAllAutoStacker.SetRetestSkipIgnore(DEF_REQ_OK);
	}else if(nOperatorCommand == TWMB_ID_CANCEL){
		//g_TaskAutoStacker.SetRetestSkipIgnore(DEF_REQ_CANCEL);
		g_TaskAllAutoStacker.SetRetestSkipIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Test_Time_Out_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam, int nStationID)
{
	ST_DD_DEVICE pDvcData;

	int nMaxSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nMaxSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int y = 0; y < nMaxSiteY; y++)
	{
		for (int x = 0; x < nMaxSiteX; x++)
		{
			int nTargetPos = (x + (y*STATION_MAX_X_SITE))+1;
			pDvcData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + nStationID, x, y, &pDvcData);

			if (pDvcData.sExist == DEF_EXIST)
			{
				g_TaskPressUnit[nStationID].SetSiteDvcTestingInfo(x, y, eTestSite_TestEnd);
				
				pDvcData.nTestingInfo = eTestSite_TestEnd;
				// setting device data
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + nStationID, x, y, &pDvcData);

				MakeLog("[OnRcv_Req_Test_Time_Out_Ack] Station = [%d] Socket = [%d], DeviceTestingInfo = [%d]", nStationID, nTargetPos,  pDvcData.nTestingInfo);
			}
		}
	}

	// index time ( EOT )
	g_TaskSystemCtrl.m_tIndexTime[nStationID].StartTimer();
}
void CTaskSystemCtrl::OnRcv_Req_Press_Arrive_Sensor_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetPressArriveSensorIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetPressArriveSensorIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Auto_Retest_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetAutoRetestIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetAutoRetestIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_Switch_Tray_Feeder_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetSwitchTrayFeederIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetSwitchTrayFeederIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_CB_Modify_Time_Check_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetCBModifyTimeCheckIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetCBModifyTimeCheckIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_CB_Data_OnOff_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetCBDataOnOffIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetCBDataOnOffIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_CB_File_Not_Exist_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetCBFileNotExistIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetCBFileNotExistIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_GRR_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetGRRModeIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetGRRModeIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_LOOP_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetLoopModeIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetLoopModeIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_AUDIT_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetAuditModeIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetAuditModeIgnore(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_QA_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetQAModeIgnore(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetQAModeIgnore(DEF_REQ_CANCEL);
	}
}


BOOL CTaskSystemCtrl::IsLotInfoEmpty()
{
	char szLotInfo[64] = {0,};
	BOOL bisLotInforEmpty = FALSE;
	//int nLotInfoMemory[3] ={SDM11_LotNo, SDM11_ScheduleNo, SDM11_OperatorID};
	int nLotInfoMemory[2] ={SDM11_LotNo, SDM11_SLT_ProgramName};
	for(int i=0; i</*3*/2; i++)
	{
		g_DMCont.m_dmHandlerOpt.GS(nLotInfoMemory[i],szLotInfo,sizeof(szLotInfo));
		if(strcmp(szLotInfo,"") == 0){
			bisLotInforEmpty = TRUE;
		}
	}
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID,szLotInfo,sizeof(szLotInfo));
	if(strcmp(szLotInfo,"")==0){
		bisLotInforEmpty = TRUE;
	}
	return bisLotInforEmpty;
}

int CTaskSystemCtrl::CheckEqpConditionForMultiOrg()
{
	// check door (door-lock, slide pack ����)
	if( !g_COVER_SW.GetStatus() ){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, this);
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK;
	}	
	int nErr = g_TaskSystemCtrl.ChkComInterlock();
	if( nErr != ERR_NO_ERROR )
		return nErr;

	// cp trip
	std::vector<int> vCP_Trip_Alarm_Sen;
	for(std::vector<int>::size_type i=0; i<m_vCPTrip_Sig.size(); i++)
	{
		if( g_IO.in(m_vCPTrip_Sig[i]) != DEF_ON )
		{
			vCP_Trip_Alarm_Sen.push_back( m_vCPTrip_Sig[i] );
		}
	}

	if( !vCP_Trip_Alarm_Sen.empty() )
	{
		CParamList AlarmData;
		for(std::vector<int>::size_type i=0; i<vCP_Trip_Alarm_Sen.size(); i++)
		{
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, vCP_Trip_Alarm_Sen[i], 1, rDiInfo);
			if( rDiInfo.m_fnGetCount() > 0 )
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if( pItem )
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s : %s = %s", pItem->label, pItem->name, (g_IO.in(vCP_Trip_Alarm_Sen[i]))?_T("ON"):_T("OFF") );
				}
			}
		}
		g_Error.AlarmReport(ERR_CP_IS_TRIP, AlarmData, this);
		return ERR_CP_IS_TRIP;
	}

	if( g_Error.Get_Jam_Alarm_Set_State() == TRUE )
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_SYS_JAM_ALARM_DOES_NOT_CLEARED, AlarmData, this);
		return ERR_SYS_JAM_ALARM_DOES_NOT_CLEARED;
	}
	return ERR_NO_ERROR;
}


int CTaskSystemCtrl::Check2dMarkDeviceName()
{
	BOOL b2dBarcodeUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	BOOL b2dMarkUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_MARK_OPT_USE);
	if (b2dBarcodeUse == eOPTION_USE && b2dMarkUse == eOPTION_USE) {
		//===============Lot Info 입력 Device Name
		CString strLotDeviceName = _T("");
		char szLotDeviceName[MAX_PATH] = { 0, };

		g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szLotDeviceName, MAX_PATH);
		strLotDeviceName = (LPCTSTR)szLotDeviceName;
		strLotDeviceName.Replace(" ", "");



		//==================2d Mark Validate 입력 Device Name
		CString strSavedDeviceName = _T("");
		char szSavedDeviceName[MAX_PATH] = { 0, };
		
		g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szSavedDeviceName, MAX_PATH);
		strSavedDeviceName = (LPCTSTR)szSavedDeviceName;
		strSavedDeviceName.Replace(" ", "");

		//2가지 비교
		if (strLotDeviceName != strSavedDeviceName)
		{
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_2D_DEVICE_NAME_MISSMATCH, AlarmData, this);
			return ERR_2D_DEVICE_NAME_MISSMATCH;
		}
	}

	return ERR_NO_ERROR;
}

CString CTaskSystemCtrl::CheckMakeSummary()
{
	// Lot Info Save �� Lot No, Schedule No�� �� �ϱ� ������  Tray End �� Lot Mode �� RT �κ� �� �Ǳ� ���� Lot Mode �� Ȯ��
	CString strErrMsg = _T(""),strBeforeMode = _T(""), strCurrentMode = _T("");

	int nBeforeLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_LOT_MODE);
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	// nBeforeLotStartMode == -1  -> Make Summary �� -1�� ����  
	if(nBeforeLotStartMode == -1 || g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0)
	{
		return strErrMsg;
	}
	switch(nBeforeLotStartMode){
		case eStartMD_Initial:
		case eStartMD_InitContinue:
			{strBeforeMode.Format("FT");} break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
			{strBeforeMode.Format("RT");} break;
		case eStartMD_QA:
		case eStartMD_QA_Continue:
			{strBeforeMode.Format("QA");}break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue:
			{strBeforeMode.Format("GRR"); }break;
	}

	switch(nLotStartMode){
		case eStartMD_Initial:
		case eStartMD_InitContinue:
			{strCurrentMode.Format("FT");} break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
			{strCurrentMode.Format("RT");} break;
		case eStartMD_QA:
		case eStartMD_QA_Continue:
			{strCurrentMode.Format("QA");}break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue:
			{strCurrentMode.Format("GRR"); }break;
	}

	if(strBeforeMode.Compare(strCurrentMode) != 0){
		strErrMsg.Format("Lot Mode is different [Before Lot Mode = %s, Current Lot Mode = %s]",strBeforeMode,strCurrentMode);
	}

	return strErrMsg;
}


int CTaskSystemCtrl::MakeSummary(BOOL bIsGrrMode /*FALSE*/)
{
	MakeLog("GUI SLT -> SLT MC MakeSummary Message Send Start");
	char szCompany[256] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nTrayEndFeed = g_TaskSystemCtrl.MakeSummaryGetTrayEndFeed();

	
	MakeLog("MakeSummary File Create Start");

	if(bIsGrrMode == FALSE){
		//imsi 
		int nSummaryInterlock = SummaryNetworkDriveInterlock();
		
		if(nSummaryInterlock != ERR_NO_ERROR){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Summary Network Driver disconnect!!. Please check Summary Path");
			g_Error.AlarmReport(nSummaryInterlock, AlarmData, NULL);
			return 0;
		}

		//g_JamRate.SaveSummaryTwType();

		int nError = g_JamRate.SaveLotJamRate_Summary();

		if (nError != ERR_NO_ERROR) {
			MakeLog("Summary Network Driver disconnect!!. Please check Summary Path");
			return 0;
		}

		CString strRetest;
		char szRetest[MAX_PATH] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szRetest, MAX_PATH);
		strRetest.Format("%s", szRetest);

		int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
		

		BOOL bLotSummarySave = FALSE;
		if (strcmp(szRetest, "1FT1") == 0) //고객 요청으로 1FT1 Mode 일 때만 Lot Summary 저장 한다.
		{
			bLotSummarySave = TRUE;
		}

		if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0 && bLotSummarySave == TRUE)
		{
			nError = g_JamRate.SaveAmkorLotSummaryFileCreate();

			if (nError != ERR_NO_ERROR) {
				MakeLog("AmkorLotSummary Make Fail!!. Please check Amkor_Lot_Summary Path");
				return 0;
			}
		}
		

		g_JamRate.ProductCsvTransfer(); // ProductCSV�� �̵�
		g_JamRate.ClearRecDvcData();
	}

	MakeLog("MakeSummary File Create End");

	MakeLog("MakeSummary Lot Data Clear Start");

	g_TaskSystemCtrl.LotInfoDataClear();

	g_TaskSystemCtrl.ResetUPHtm();
	g_DMCont.m_dmEQP.SN(NDM0_LotRunTime , 0);
	g_DMCont.m_dmEQP.SN(NDM0_LotStopTime, 0);

	// clear lot done data
	char szFileName[512] = {0,};
	g_DMCont.m_dmEQP.SS(SDM0_LotDoneFileName, szFileName, sizeof(szFileName));

	g_DMCont.m_dmEQP.SN(NDM0_CurLotLoadingCnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotSortingCnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotPassBinCnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotFailBinCnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput1Cnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput2Cnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput3Cnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput4Cnt, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_LoadingCnt, 0);		
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_SortingCnt, 0);	
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_PassBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_FailBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output1Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output2Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output3Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output4Cnt, 0);

	g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL , 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS  , 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT1_TOTAL, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT1_PASS , 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT2_TOTAL, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT2_PASS , 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotJamCount, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotQAEmptyLoadCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_QaSampleAddSumLoadCnt, 0);

	g_DMCont.m_dmFeeder.SB(BDM10_FEEDER_TRAY_END, FALSE);

	g_DMCont.m_dmEQP.SN( NDM0_CurLot2D_Read_Error_Cnt, 0 );

	g_DMCont.m_dmEQP.SB(BDM0_CYCLE_UPH_CHECK, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CycleCurLotSortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CycleLotStartTime,0);
	
	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE1_RETRY_CNT, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE2_RETRY_CNT, 0);

	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE1_TOTAL_TRIGGER_CNT, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE2_TOTAL_TRIGGER_CNT, 0);
	
	g_Error.ResetCurrentLotJamCount();

	for(int i=0; i<eMaxPressUnitCount; i++){
		g_DMCont.m_dmEQP.SN(NDM0_CleaningContact_Press1+i, 0);
		g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE+i), FALSE);

		for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ )
		{
			for(int nX=0; nX < STATION_MAX_X_SITE; nX++ )
			{
				int nPos = nX + nY*STATION_MAX_X_SITE + STATION_MAX_PARA*i;
				g_DMCont.m_dmContac_Dn.SN(NDM1_GdBinCnt_Press1_1+nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1+nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_FailBinCnt_Press1_1 + nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1 + nPos, 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nPos, 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_2DIDFailBinCnt_Press1_1 + nPos, 0);

					
			}
		}
	}

	g_TaskSystemCtrl.YieldSampleClear();
	//g_TaskSystemCtrl.YieldVarianceClear();
	g_TaskSystemCtrl.YieldContiFailClear();
	g_TaskSystemCtrl.Yield1stFailClear();

	g_JamRate.ResetAllData();
	//if(nTrayEndFeed == DEF_LOTEND_TRAYFEED){
		g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_Initial);
	//	MakeLog("[Tray Feed end Make Summary]");
	//}else if(nTrayEndFeed == DEF_LOTEND_TRAYEND){
	//	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_RetestContinue);
	//	// record lot start time
	//	SYSTEMTIME time;
	//	::GetLocalTime( &time );
	//	CString strLOTstartTm = _T("");
	//	// sample 2016-01-05 21:10:14.343
	//	strLOTstartTm.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	//	g_DMCont.m_dmEQP.SS(SDM0_CurLotStartTm, (LPSTR)(LPCTSTR)strLOTstartTm, strLOTstartTm.GetLength()+1);
	//	g_TaskSystemCtrl.m_tcUPH.StartTimer();
	//	MakeLog("[Tray End end Make Summary]");
	//}
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_LOT_MODE,-1);
	g_TaskSystemCtrl.MakeSummarySetTrayEndFeed(DEF_LOTEND_UNK);

	for(int i=0; i< eMaxPressUnitCount; i++)
	{
		for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ )
		{
			for(int nX=0; nX < STATION_MAX_X_SITE; nX++ )
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_SiteHistory1_1+ nX + nY*STATION_MAX_X_SITE + (i * STATION_MAX_PARA ), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_SiteStatusHistory1_1 + nX + nY*STATION_MAX_X_SITE + (i * STATION_MAX_PARA), 0);
			}
		}
		g_DMCont.m_dmContac_Dn.SD(DDM1_SUMMARY_INDEX_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + i, 0);

		g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_TIME_ACC_CNT_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SD(DDM1_INDEX_TIME_ACC_TIME_STATION1 + i, 0);

		g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + i, 0);
		g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_CNT_STATION1 + i, 0);
	}
	//Make Summary �� 2D Barcode �ʱ�ȭ 
	g_2DBarcodeChk.ResetData();


	char szData[MAX_PATH] = { 0, };
	for (int i = 0; i < 1024; i++) {
		g_DMCont.m_dm2DID.SS(SDM13_2DID_STRING_LIST1 + i, "", sizeof(szData));
	}
	//if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		//Socket ID �� Contact Count �� Check �ϱ� ����
		if (GetSocketIDContactCnt() > 0) {
			g_TaskSystemCtrl.MakeSocketIDContactCnt();
		}
		g_TaskSystemCtrl.ClearSocketIDContactCnt();
	//}		
	MakeLog("MakeSummary Lot Data Clear End");

	LotDataClear();

	MakeLog("GUI SLT -> SLT MC MakeSummary Message Send End");

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE && nProjectOption == DEF_PROJECT_WATT)
	{
		g_AppleTestIF.m_TesterMgr.SendQuerySetLotStatus(eTesterLotStatus::eTester_Lot_End);
	}

	g_GuiComm.SendLotClearEnd();

	return 0;
}

CString CTaskSystemCtrl::ScopesInterlock()
{
	/// File ���� Ȯ��

	// Lot Info Data ��
	char szTemp[MAX_PATH] ={0,}, szRetBuff[MAX_PATH]={0,};
	CString strLotInfo =_T(""),  strTitle = _T(""), strData = _T(""),strLotInfoPath =_T(""), strErrMsg = _T("");
	char szLotInfoPath[MAX_PATH] ={0,};

	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotInfoFile_Path,szLotInfoPath,sizeof(szLotInfoPath));

	strLotInfoPath.Format("%s\\uflex_lot_info.txt",szLotInfoPath);
	std::ifstream ifile;

	ifile.open(strLotInfoPath);                                                                                                                                                                    

	if(ifile.is_open())
	{
		while(ifile.getline(szTemp,sizeof(szTemp)))
		{
			strLotInfo = szTemp;

			int nIndx = strLotInfo.Find(':');
			CString strTitle = strLotInfo.Left(nIndx);
			CString strData = strLotInfo.Mid(nIndx+1);
			//strData.Replace(" ","");

			strTitle.MakeUpper();

			// Lot Info Dialog �� ǥ�� �Ǵ� �׸�
			if(strTitle.Compare("LOT") == 0 ){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo,szRetBuff,sizeof(szRetBuff));
				strData.Replace(" ","");
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Lot No different!! [Save Lot No: %s, Lot Information Lot No: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			else if(strTitle.Compare("CUST") == 0){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName,szRetBuff,sizeof(szRetBuff));
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Customer Name different!! [Save Customer Name: %s, Lot Information Customer Name: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			else if(strTitle.Compare("DEVICE") == 0){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName,szRetBuff,sizeof(szRetBuff));
				strData.Replace(" ","");
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Device Name different!! [Save Device Name: %s, Lot Information Device Name: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			else if(strTitle.Compare("RETEST") == 0){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo,szRetBuff,sizeof(szRetBuff));
				strData.Replace(" ","");
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Process different!! [Save Process: %s, Lot Information Process: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			else if(strTitle.Compare("TP_PROG") == 0){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName,szRetBuff,sizeof(szRetBuff));
				strData.Replace(" ","");
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Program Name different!! [Save Program Name: %s, Lot Information Program Name: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			else if(strTitle.Compare("OPERATION") == 0){
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation,szRetBuff,sizeof(szRetBuff));
				strData.Replace(" ","");
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Process ID different!! [Save Process ID: %s, Lot Information Process ID: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				};
			}
			else if(strTitle.Compare("QTY") == 0){
				strData.Replace(" ","");
				g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY,szRetBuff,sizeof(szRetBuff));
				if(strData.Compare(szRetBuff) !=0){
					strErrMsg.Format("Lot Size different!! [Save Lot Size: %s, Lot Information Lot Size: %s]",szRetBuff,strData);
					MakeLog("[Scops %s]",strErrMsg);
					break;
				}
			}
			Sleep(1);
		}
		ifile.close();	
	}else{
		strErrMsg.Format("Can not find Lot Information File!! please check Scops Program and File Path");
		MakeLog("[Scops Can not find Lot Information File!!]");
	}

	return strErrMsg;
}
int CTaskSystemCtrl::SummaryNetworkDriveInterlock()
{
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if( nRunMode == eRUN_MODE_ONLINE )
	{
		// get path
		char szFilePath[MAX_PATH] = {0,};
		g_DMCont.m_dmHandlerOpt.GS(SDM11_SummaryFile_Path,szFilePath,MAX_PATH);
	
		CString strDriveName = _T("");
		strDriveName.Format("%c:", szFilePath[0]);
		// checking network driver
		char szRemoteName[256] = {0,};
		int  nRemoteNameLen = 256;

		DWORD dRetErr = NO_ERROR;
		int nDirType = GetDriveType(strDriveName);
		if(nDirType == DRIVE_FIXED ){
			dRetErr = _access(strDriveName,0); 
		}else if(nDirType == DRIVE_REMOTE){
			dRetErr = WNetGetConnection(strDriveName, szRemoteName, (LPDWORD)&nRemoteNameLen);
		}

		if(dRetErr != NO_ERROR){
			return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskSystemCtrl::SummaryNetworkDriveInterlockCheck(char* szFilePath)
{
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		// get path

		CString strDriveName = _T("");
		strDriveName.Format("%c:", szFilePath[0]);
		// checking network driver
		char szRemoteName[256] = { 0, };
		int  nRemoteNameLen = 256;

		DWORD dRetErr = NO_ERROR;
		int nDirType = GetDriveType(strDriveName);
		if (nDirType == DRIVE_FIXED) {
			dRetErr = _access(strDriveName, 0);
		}
		else if (nDirType == DRIVE_REMOTE) {
			dRetErr = WNetGetConnection(strDriveName, szRemoteName, (LPDWORD)&nRemoteNameLen);
		}

		if (dRetErr != NO_ERROR ) {
			return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskSystemCtrl::CheckManualable(BOOL bManualMode)
{
	BOOL bAutoGrrUse  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	//int nOffsite = (bAutoGrrUse == TRUE) ? NDM11_GRR_SocketOnOff_Site1 : NDM11_SocketOnOff_Site1;
	
	if (bManualMode == FALSE)
	{
		for (int i = 0; i < eMaxPressUnitCount; i++) {
			int nRetSocket = g_TaskPressUnit[i].GetSocketOnCnt(bAutoGrrUse);

			if (nRetSocket > 0) {
				if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + i) == TRUE) {
					return ERR_MANUAL_MODE_INTERLOCK_TEST_STATUS;
				}
			}
		}
	}

	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);

	if(bLotState == TRUE){
		return ERR_MANUAL_MODE_INTERLOCK_RUNNING;
	}

	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

	return ERR_NO_ERROR;
}

CString CTaskSystemCtrl::CheckCategoryInterlock()
{
	CString strErrMsg = _T("");

	BOOL bHWBinTextFlag = FALSE;
	BOOL bPassFailTextFlag = FALSE;
	BOOL bSWBinTextFlag = FALSE;
	BOOL bHWBinDataFlag = FALSE;
	BOOL bPASSTextFlag = FALSE;
	BOOL bFAILTextFlag = FALSE;
	BOOL bSWBinID = FALSE;
	BOOL bTestTimeOutFlag = FALSE;
	BOOL b2DIDFailHWBinFlag = FALSE;

	int nProject = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	std::set<int> sID;
	std::pair<std::set<int>::iterator, bool > pr;
	std::vector<char*> vErrorStr;
	std::vector<int> vHWBinData1, vHWBinData2;

	CString strTemp;
	for (int i = 0; i < g_BinCategory.nIndexCount; i++)
	{
		//�빮��
		strTemp = g_BinCategory.aCateInfo[i].szDesc;
		strTemp.MakeUpper();
		strcpy_s(g_BinCategory.aCateInfo[i].szDesc, sizeof(g_BinCategory.aCateInfo[i].szDesc), strTemp);

		// HW Bin�� 4�̻� �Է� �Ұ�
		if (atoi(g_BinCategory.aCateInfo[i].szHWBin) > 4 || atoi(g_BinCategory.aCateInfo[i].szHWBin) <= 0) {
			bHWBinTextFlag = TRUE;
		}

		// PASS/FAIL�� 0 or 1�� ����
		if (strcmp(g_BinCategory.aCateInfo[i].szDesc, DEF_HANDLER_ERR_DEVICE) == 0
			|| strcmp(g_BinCategory.aCateInfo[i].szDesc, DEF_2D_READ_ERR_DEVICE) == 0
			|| strcmp(g_BinCategory.aCateInfo[i].szDesc, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0) {
			if (atoi(g_BinCategory.aCateInfo[i].szPassFail) != 0)
			{
				bPassFailTextFlag = TRUE;
			}
		}
		else if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "FAIL") == 0 && atoi(g_BinCategory.aCateInfo[i].szPassFail) != 0) {
			bFAILTextFlag = TRUE;
		}
		else if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "PASS") == 0 && atoi(g_BinCategory.aCateInfo[i].szPassFail) != 1) {
			bPASSTextFlag = TRUE;
		}
		else {
			if (atoi(g_BinCategory.aCateInfo[i].szPassFail) != 0 && atoi(g_BinCategory.aCateInfo[i].szPassFail) != 1) {
				bPassFailTextFlag = TRUE;
			}
		}

		// Pass -> 1 / Fail -> 0
		if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "PASS") == 0 && atoi(g_BinCategory.aCateInfo[i].szSWBin) != 1) {
			bSWBinTextFlag = TRUE;
		}
		else if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "FAIL") == 0 && atoi(g_BinCategory.aCateInfo[i].szSWBin) != 0) {
			bSWBinTextFlag = TRUE;
		}

		//if (g_TaskSystemCtrl.IsCompanyProjectIndex() == eUSI_Marlin)
// 		if(nProject != DEF_PROJECT_MAUI)
// 		{
			if (strcmp(g_BinCategory.aCateInfo[i].szDesc, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0 && atoi(g_BinCategory.aCateInfo[i].szSWBin) != 97) 
				bTestTimeOutFlag = TRUE;

// 			if (nLotStartMode == eStartMD_Auto2DIDSort || nLotStartMode == eStartMD_Auto2DIDSort_Continue) {
// 				if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "FAIL") == 0 && atoi(g_BinCategory.aCateInfo[i].szHWBin) != 2)
// 					b2DIDFailHWBinFlag = TRUE;
// 			}
//		}

		// SW Bin ID �ߺ��Ǹ� �ȵ�
		pr = sID.insert(atoi(g_BinCategory.aCateInfo[i].szSWBin));
		if (pr.second == FALSE) { bSWBinID = TRUE; }

		// Pass/Fail�� HW Bin�� �����ϸ� �ȵ�
		if (atoi(g_BinCategory.aCateInfo[i].szPassFail) == 1) {
			vHWBinData1.push_back(atoi(g_BinCategory.aCateInfo[i].szHWBin));
		}
		else if (atoi(g_BinCategory.aCateInfo[i].szPassFail) == 0) {
			vHWBinData2.push_back(atoi(g_BinCategory.aCateInfo[i].szHWBin));
		}

		// �ʼ� ErrStr ���翩��
		vErrorStr.push_back(g_BinCategory.aCateInfo[i].szDesc);
	}

	for (std::vector<int>::size_type i = 0; i < vHWBinData2.size(); i++) {
		auto iter3 = find(vHWBinData1.begin(), vHWBinData1.end(), vHWBinData2[i]);
		if (iter3 != vHWBinData1.end()) {
			bHWBinDataFlag = TRUE;
		}
	}

//	auto iter = std::find_if(vErrorStr.begin(), vErrorStr.end(),
//		[](char* errorString) -> bool
//	{
//		if (strcmp(errorString, DEF_HANDLER_ERR_DEVICE) == 0)
//		{
//			return TRUE;
//		}
//		else { return FALSE; }
//	});

	auto iter1 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, DEF_2D_READ_ERR_DEVICE) == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter2 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, "PASS") == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter3 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, "FAIL") == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter4 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter5 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, DEF_TEST_ALARM_DEVICE) == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter6 = std::find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, DEF_TEST_RESULT_WRONG_APN) == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	for (std::vector<int>::size_type i = 0; i < vHWBinData2.size(); i++) {
		auto iter3 = find(vHWBinData1.begin(), vHWBinData1.end(), vHWBinData2[i]);
		if (iter3 != vHWBinData1.end()) {
			bHWBinDataFlag = TRUE;
		}
	}

	if (bHWBinDataFlag == TRUE) {
		strErrMsg.Format("HW Bin of Pass and Fail is equal!!");
		return strErrMsg;
	}

	//if (g_TaskSystemCtrl.IsCompanyProjectIndex() == eUSI_Marlin)

	if (iter1 == vErrorStr.end() || iter2 == vErrorStr.end() || iter3 == vErrorStr.end() || iter4 == vErrorStr.end()) {
		strErrMsg.Format("%s/%s/PASS/FAIL are not existed!!", DEF_2D_READ_ERR_DEVICE,DEF_TEST_TIME_OUT_ERR_DEVICE);
		return strErrMsg;
	}

	if (nProjectOption == DEF_PROJECT_WATT)
	{
		if (iter6 == vErrorStr.end())
		{
			strErrMsg.Format("%s are not existed!!", DEF_TEST_RESULT_WRONG_APN);
			return strErrMsg;
		}
	}



	if (bHWBinTextFlag == TRUE) {
		strErrMsg.Format("HW Bin can only be entered from 1 to 4!!");
		return strErrMsg;
	}

	if (bPassFailTextFlag == TRUE) {
		strErrMsg.Format("Pass/Fail can only be 1 or 0!!");
		return strErrMsg;
	}

	if (bPASSTextFlag == TRUE) {
		strErrMsg.Format("PASS must be 1!!");
		return strErrMsg;
	}

	if (bFAILTextFlag == TRUE) {
		strErrMsg.Format("FAIL must be 0!!");
		return strErrMsg;
	}

	if (bSWBinTextFlag == TRUE) {
		strErrMsg.Format("The SW Bin of the PASS must be 1 and the SW Bin of the FAIL must be 0!!");
		return strErrMsg;
	}

	if (bSWBinID == TRUE) {
		strErrMsg.Format("Duplicate SW ID");
		return strErrMsg;
	}

	if (bTestTimeOutFlag == TRUE) {
		strErrMsg.Format("DEF_TEST_TIME_OUT_ERR_DEVICE SW Bin change it to 97 ");
		return strErrMsg;
	}

	if (b2DIDFailHWBinFlag == TRUE) {
		strErrMsg.Format("Handler Sorting Function [FAIL] HW Bin change it to 2 ");
		return strErrMsg;
	}

	return strErrMsg;
}

BOOL CTaskSystemCtrl::CompareDefaultBinToCategory()
{
	BOOL bIsOk = FALSE;

	for (int nDefaultIdx = 0; nDefaultIdx<(int)g_DefaultBinCategory.vDefaultBin.size(); nDefaultIdx++) {
		bIsOk = FALSE;
		for (int nCategoryIdx = 0; nCategoryIdx<g_BinCategory.nIndexCount; nCategoryIdx++) {
			if (g_DefaultBinCategory.vDefaultBin[nDefaultIdx].Compare(g_BinCategory.aCateInfo[nCategoryIdx].szDesc) == 0) {
				bIsOk = TRUE;
				break;
			}
		}
		if (bIsOk != TRUE) {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTaskSystemCtrl::AddSocketIDContactCnt(CString strSocketID, int nStationNum)
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nCnt = m_SocketCountCnt.nIndexCount;
	BOOL bExist = FALSE;
	ST_SOCKEID_INFO stInfo;

	if (nTesterOption == eTester_Agent) {
		strSocketID.Format("%d_%s", nStationNum, strSocketID);
	}

	if (strSocketID.IsEmpty() != TRUE) {
		for (int i = 0; i < nCnt; i++) {
			if (m_SocketCountCnt.vSocketCountactInfo[i].strSocketID.Compare(strSocketID) == 0)
			{
				m_SocketCountCnt.vSocketCountactInfo[i].nTD++;
				m_SocketCountCnt.vSocketCountactInfo[i].nTotalTD++;
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE) {			
			stInfo.strSocketID = strSocketID;
			stInfo.nTD = 1;
			stInfo.nTotalTD = 1;

			m_SocketCountCnt.vSocketCountactInfo.push_back(stInfo);
			m_SocketCountCnt.nIndexCount++;
		}
	}
	return TRUE;
}

BOOL CTaskSystemCtrl::MakeSocketIDContactCnt()
{
	CString strFilePath = _T("");

	CStdioFile f;
	CString strTitle = _T("");
	CString strFileLine = _T("");
	CString strFileSocketID = _T("");
	CString strFileTD = _T("");
	CString strFIleTotalTD = _T("");
	CString strFileLimit = _T("");

	int nCount = 0;
	ST_SOCKET_CONTACT_COUNT stCountInfo;
	BOOL bFileExist = TRUE;
	CString strDir = _T("D:\\Techwing\\Socket_Contact");

	char szEqpName[64] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));

	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	strFilePath.Format(_T("%s\\%s.csv"), strDir, szEqpName);
	//file �����ϴ��� Ȯ��
	if (!f.Open(strFilePath, CFile::modeReadWrite))
	{
		//f.Close();
		bFileExist = FALSE;
	}
	MakeLog("[MakeSocketIDContactCnt File Start]");
	if (bFileExist) {
		f.SeekToBegin();

		while (f.ReadString(strFileLine) != NULL)
		{
			std::pair<CString, std::pair<int, int>> p;
			ST_SOCKEID_INFO stInfo;

			AfxExtractSubString(strFileSocketID, strFileLine, 0, ',');

			if (strFileSocketID.Compare("Socket ID") != 0) {
				AfxExtractSubString(strFileTD, strFileLine, 1, ',');
				AfxExtractSubString(strFIleTotalTD, strFileLine, 2, ',');
				AfxExtractSubString(strFileLimit, strFileLine, 3, ',');

				stInfo.strSocketID = strFileSocketID;
				stInfo.nTD = atoi(strFileTD);
				stInfo.nTotalTD = atoi(strFIleTotalTD);
				stInfo.nLimit = atoi(strFileLimit);

				stCountInfo.vSocketCountactInfo.push_back(stInfo);
				nCount++;
			}
			Sleep(1);
		}
		stCountInfo.nIndexCount = nCount;
		f.Close();

		remove(strFilePath);
	}

	CStdioFile file;
	//File ����
	if (!file.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate))
	{
		AfxMessageBox(_T("Socket ID Count File Create Fail!!"));
		return FALSE;
	}

	int nFileDataCnt = stCountInfo.nIndexCount;
	int nMemoryDataCnt = m_SocketCountCnt.nIndexCount;
	BOOL bFind = FALSE;
	MakeLog("[MakeSocketIDContactCnt] nFileDataCnt = %d , nMemoryDataCnt =%d", nFileDataCnt, nMemoryDataCnt);
	CString strOverSocketID = _T("");

	for (int i = 0; i < nMemoryDataCnt; i++) {
		bFind = FALSE;
		for (int j = 0; j < nFileDataCnt; j++) {
			if (stCountInfo.vSocketCountactInfo[j].strSocketID.Compare(m_SocketCountCnt.vSocketCountactInfo[i].strSocketID) == 0)
			{
				stCountInfo.vSocketCountactInfo[j].nTD += m_SocketCountCnt.vSocketCountactInfo[i].nTD;
				stCountInfo.vSocketCountactInfo[j].nTotalTD += m_SocketCountCnt.vSocketCountactInfo[i].nTotalTD;

				// Socket ID �� TD Count �� Limit Count �� �� �Ͽ� Message �߻� ���� Ȯ��
				if (stCountInfo.vSocketCountactInfo[j].nTD > stCountInfo.vSocketCountactInfo[j].nLimit) {
					if (strOverSocketID.IsEmpty() != TRUE) {
						strOverSocketID += _T(", ");
					}
					strOverSocketID += stCountInfo.vSocketCountactInfo[j].strSocketID;
				}
				bFind = TRUE;
				break;
			}
		}

		if (bFind != TRUE) {
			m_SocketCountCnt.vSocketCountactInfo[i].nLimit = g_DMCont.m_dmEQP.GN(NDM0_SocketContactCntLimit);
			stCountInfo.vSocketCountactInfo.push_back(m_SocketCountCnt.vSocketCountactInfo[i]);

			stCountInfo.nIndexCount++;
		}
	}
	MakeLog("[MakeSocketIDContactCnt] stCountInfo.vSocketCountactInfo = %d ,stCountInfo.nIndexCount =%d", (int)stCountInfo.vSocketCountactInfo.size(), stCountInfo.nIndexCount);
	CString strTmp = _T("");
	strFileLine.Empty();

	strTmp += _T("Socket ID, TD, Total TD, Limit");
	strTmp += _T("\n");
	for (int i = 0; i < /*stCountInfo.nIndexCount*/(int)stCountInfo.vSocketCountactInfo.size(); i++) {
		strFileLine.Format(_T("%s,%d,%d,%d"), stCountInfo.vSocketCountactInfo[i].strSocketID, stCountInfo.vSocketCountactInfo[i].nTD, stCountInfo.vSocketCountactInfo[i].nTotalTD, stCountInfo.vSocketCountactInfo[i].nLimit);
		strFileLine += _T("\n");
		strTmp += strFileLine;
	}

	file.SeekToEnd();
	file.WriteString(strTmp);
	file.Close();

	if (strOverSocketID.GetLength() <= (MAX_BUFF - 1)) {
		MakeLog("[MakeSocketIDContactCnt File Close] strOverSocketID = %s, Length = %d", strOverSocketID, strOverSocketID.GetLength());
	}
	else{
		MakeLog("[MakeSocketIDContactCnt File Close] strOverSocketID Over  Length = %d", strOverSocketID.GetLength());
	}
	
	if (strOverSocketID.IsEmpty() != TRUE && strOverSocketID.GetLength() <= (MAX_BUFF-1)) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat(strOverSocketID);
		g_Error.AlarmReport(ERR_SOCKET_CONTACT_COUNT_OVER, AlarmData, &g_TaskSystemCtrl);
	}

	MakeLog("[MakeSocketIDContactCnt File Close]");

	return TRUE;
}

void CTaskSystemCtrl::ClearSocketIDContactCnt()
{
	m_SocketCountCnt.clear();
}

int CTaskSystemCtrl::GetSocketIDContactCnt()
{
	return m_SocketCountCnt.nIndexCount;
}

BOOL CTaskSystemCtrl::IsDefaultBin(ST_DD_DEVICE stDvcData)
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	
	if (strcmp(stDvcData.szErrString, DEF_HANDLER_ERR_DEVICE) == 0) {
		return TRUE;
	}
	

	// DefaultBin�� ������ ��쿡 �ٸ� Station���� ������ �Ǵ´� �׷��� �Ǹ� �ִ� Test�� 16�� ��, 
	// Marlin�� 3���� �ϰ� ���� ���;� �ϱ⿡ DefaultBin ���� ��û. By Amkor ��Կ� ������ ��û[5/11/2021 donghyun.shin]
	
	return FALSE;
	

	for (int i = 0; i < (int)g_DefaultBinCategory.vDefaultBin.size(); i++) {
		if (g_DefaultBinCategory.vDefaultBin[i].Compare(stDvcData.szErrString) == 0) {
			return TRUE;
		}
	}

	

	return FALSE;
}

void CTaskSystemCtrl::OnRcv_Req_Remain_AutoSkipDevice_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_RETRY ) {
		MakeLog("OnRcv_Req_Remain_AutoSkipDevice_Ack = TWMB_ID_RETRY");
		int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
		int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
		ST_DD_DEVICE stDevice;

		for (int j = 0; j < nPocketCntY; j++)
		{
			for (int i = 0; i < nPocketCntX; i++)
			{
				stDevice.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
				{
					StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), DEF_NEW_LOAD_DEVICE);
					stDevice.sExist = eDeviceData_Exist;
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				}
			}
		}

// 		bool bFindAutoSkipDevice = false;
// 		std::for_each(g_load_stackers.begin(), g_load_stackers.end(),
// 			[&](CTaskAutoStacker* _load_stacker)
// 		{
// 			if (_load_stacker->GetConveyorState() == ReadyToUnload)
// 			{
// 				int stacker_idx = _load_stacker->GetStackerIndex();
// 
// 				for (int j = 0; j < nPocketCntY; j++)
// 				{
// 					for (int i = 0; i < nPocketCntX; i++)
// 					{
// 						stDevice.clear();
// 						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// 						if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
// 						{
// 							StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), DEF_NEW_LOAD_DEVICE);
// 							stDevice.sExist = eDeviceData_Exist;
// 							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// 						}
// 					}
// 				}
// 			}
// 		});



	}
	else if (nOperatorCommand == TWMB_ID_SKIP) {
		MakeLog("OnRcv_Req_Remain_AutoSkipDevice_Ack = TWMB_ID_SKIP");
	}
	else if (nOperatorCommand == TWMB_ID_PARTIAL_TRAYEND) {
		MakeLog("OnRcv_Req_Remain_AutoSkipDevice_Ack = TWMB_ID_PARTIAL_TRAYEND");

		g_DMCont.m_dmTrayPP.SN(NDM5_TM_TrayPP_Tray_End, TRUE);

		int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
		int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
		ST_DD_DEVICE stDevice;

		bool bFindAutoSkipDevice = false;

		for (int j = 0; j < nPocketCntY; j++)
		{
			for (int i = 0; i < nPocketCntX; i++)
			{
				stDevice.clear();
				//g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				/*if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)*/
				{
					StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), "");
					stDevice.sExist = eDeviceData_None;
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				}
			}
		}

// 		BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
// 		if (bAutoQAUse == TRUE) //QA Mode �δ� Tray End�� �Ǹ� ���� ���� �� �������� ���� �� �ش�.
// 		{
// 			CString strQAQTY;
// 			int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //���� �� ����
// 			int nLoadTrayPPCnt = g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();   // Hand�� Pick �Ǿ� �ִ� ����
// 			int nLoadDeviceSum = nLotLoadCnt + nLoadTrayPPCnt;
// 			char szLotSize[64] = { 0, };
// 			strQAQTY.Format("%d", nLoadDeviceSum);
// 			strcpy_s(szLotSize, strQAQTY);
// 			g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY, szLotSize, sizeof(szLotSize));	
// 		}
// 
// 		BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
// 		if (bAutoQAUse == TRUE) //QA Mode �δ� Tray End�� �Ǹ� ���� ���� �� �������� ���� �� �ش�.
// 		{
// 			CString strQAQTY;
// 			int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //���� �� ����
// 			int nLoadTrayPPCnt = g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();   // Hand�� Pick �Ǿ� �ִ� ����
// 			int nLoadDeviceSum = nLotLoadCnt + nLoadTrayPPCnt;
// 			char szLotSize[64] = { 0, };
// 			strQAQTY.Format("%d", nLoadDeviceSum);
// 			strcpy_s(szLotSize, strQAQTY);
// 			g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY, szLotSize, sizeof(szLotSize));	
// 		}


// 		std::for_each(g_load_stackers.begin(), g_load_stackers.end(),
// 			[&](CTaskAutoStacker* _load_stacker)
// 		{
// 			if (_load_stacker->GetConveyorState() == ReadyToUnload)
// 			{
// 				int stacker_idx = _load_stacker->GetStackerIndex();
// 
// 				for (int j = 0; j < nPocketCntY; j++)
// 				{
// 					for (int i = 0; i < nPocketCntX; i++)
// 					{
// 						stDevice.clear();
// 						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// 						if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
// 						{
// 							StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), "");
// 							stDevice.sExist = eDeviceData_None;
// 							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// 						}
// 					}
// 				}
// 			}
// 		});
	}
	else if (nOperatorCommand == TWMB_ID_SKIP_TO_ASSIGN) {
		g_GuiComm.SendDataAssignShow(LOC_IDX_TRAY_PP_FEEDER);
	}
}

void CTaskSystemCtrl::OnRcv_Req_DisConnectionStation_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_OK) {
		g_TaskAllAutoStacker.SetDisConnectionStation(DEF_REQ_OK);
	}
	else if (nOperatorCommand == TWMB_ID_CANCEL) {
		g_TaskAllAutoStacker.SetDisConnectionStation(DEF_REQ_CANCEL);
	}
}

void CTaskSystemCtrl::OnRcv_Req_SocketCleaning_Dvc_Clear_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	if (nOperatorCommand == TWMB_ID_CLEAR) {
		// Socket Cleaning Device Contact Count Limit �ʰ��� Device Contact Count �ʱ�ȭ ����
		// Station�� �� �ִ� Device, Socket Cleaning Buffer�� �ִ� Device ���� Count�� Ȯ�� �Ͽ� Clear ������ �Ѵ�.

		int nClnBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
		int nClnBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

		int nAlramCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnAlarmCnt);

		ST_DD_DEVICE stDeviceData;

		//Socket Cleaning Buffer clear
		for (int nIdx = 0; nIdx < MAX_TEST_PP_CNT; nIdx++) {

			for (int nY = 0; nY < nClnBuff_Y; nY++)
			{
				for (int nX = 0; nX < nClnBuff_X; nX++)
				{
					stDeviceData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + nIdx, nX, nY, &stDeviceData);
					
					if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {

						if (nAlramCnt < stDeviceData.nRealTestingCnt) {
							stDeviceData.nRealTestingCnt = 0;
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + nIdx, nX, nY, &stDeviceData);
						}
					}
				}
			}

			//Test pp Data Ȯ��
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) 
			{
				for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) 
				{
					stDeviceData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + nIdx, nX, nY, &stDeviceData);

					if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {

						if (nAlramCnt < stDeviceData.nRealTestingCnt) {
							stDeviceData.nRealTestingCnt = 0;
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1 + nIdx, nX, nY, &stDeviceData);
						}
					}
				}
			}

		}
		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		//Station Data Ȯ��
		for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++) {
			for (int nY = 0; nY < nSiteDivY; nY++)
			{
				for (int nX = 0; nX < nSiteDivX; nX++)
				{
					stDeviceData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + nIdx, nX, nY, &stDeviceData);

					if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {

						if (nAlramCnt < stDeviceData.nRealTestingCnt) {
							stDeviceData.nRealTestingCnt = 0;
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + nIdx, nX, nY, &stDeviceData);
						}
					}
				}
			}
		}
	}
}


BOOL CTaskSystemCtrl::IsRunningFinish()
{
	BOOL bSupplyEnd = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();

	if (bSupplyEnd != TRUE && g_TaskSystemCtrl.IsOneCycleModeOn() !=TRUE ) {
		return FALSE;
	}

	for (int idx = 0; idx < eMaxPressUnitCount; idx++) {
		int SiteInDvc = g_TaskPressUnit[idx].GetSiteInDvc();
		if (SiteInDvc > 0) {
			return FALSE;
		}
	}

	for (int idx = 0; idx < eMaxTestPPCount; idx++) {
		int loadPickerHaveDvcCnt = g_TaskTestPP[idx].GetLoadPickerDeviceCnt();
		int UnloadPickerHaveDvcCnt = g_TaskTestPP[idx].GetUnloadPickerHaveDvcCnt();
		int BufferTableCnt = g_TaskTestPP[idx].GetBufferTableCnt();
		if (UnloadPickerHaveDvcCnt > 0 || BufferTableCnt > 0 || loadPickerHaveDvcCnt > 0) {
			return FALSE;
		}
	}

	for (int idx = 0; idx < eMaxLoadTblCount; idx++) {
		int UnloadDeviceCnt = g_TaskLoadTbl[idx].GetUnloadDeviceCnt();
		int LoadDeviceCnt = g_TaskLoadTbl[idx].GetLoadDeviceCnt();
		if (UnloadDeviceCnt > 0 || LoadDeviceCnt > 0) {
			return FALSE;
		}
	}

	CTaskTrayBasePP* pTrayPP[]={&g_TaskTrayLoadPP, &g_TaskTrayUnloadPP};	
	for (int idx = 0; idx < eMaxTrayPPCount; idx++) {
		int TrayPpUnloadDvcCnt = pTrayPP[idx]->GetTrayPpUnloadDvcCnt();
		int TrayPpLoadDvcCnt = pTrayPP[idx]->GetTrayPpLoadDvcCnt();
		if (TrayPpUnloadDvcCnt > 0 || TrayPpLoadDvcCnt > 0)
			return FALSE;
	}
	

	return TRUE;
}

BOOL CTaskSystemCtrl::CheckTesterDisConnection(int& nIdx, std::vector<int> vDisConnectionStationIdx)
{
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	BOOL bDisConnectionIdx = FALSE;

	int nTesterCount = 0;

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single || nTesterOption == eTester_Agent || nTesterOption == eTester_EachSocket)
		nTesterCount = MAX_TESTER_SINGLE;
	else
		nTesterCount = MAX_TESTER_DUAL;
	
	BOOL bSocketOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
	if (nLotStartMode == eStartMD_InitContinue) {

		for (int i = 0; i < nTesterCount; i++) {
			bDisConnectionIdx = FALSE;

			for (int nDisIdx = 0; nDisIdx < (int)vDisConnectionStationIdx.size(); nDisIdx++) {
				if (i == vDisConnectionStationIdx[nDisIdx]) {
					bDisConnectionIdx = TRUE;
					break;
				}
			}
			if (bDisConnectionIdx == FALSE) {
				int nRetSocket = 0;
				if (nTesterOption == eTester_Single || nTesterOption == eTester_Agent || nTesterOption == eTester_EachSocket)
				{
					nRetSocket = g_TaskPressUnit[i].GetSocketOnCnt();
					g_TaskPressUnit[i].GetSocketStsForEachSocket(FALSE, bSocketOnOff);
				}
				else
					nRetSocket = g_TaskPressUnit[i / 2].GetDirDualSocketOnCnt(FALSE,i % 2);
				
				if (nRetSocket > 0)
				{
					int nStationID = i + 1;
					BOOL bConnect =0;
					if (nTesterOption == eTester_Single || nTesterOption == eTester_Dual || nTesterOption == eTester_EachSocket)
					{
						bConnect = g_AppleTestIF.IsConnected(nStationID, nTesterOption, bSocketOnOff);
					}
					else 
						//bConnect = g_AppleAgentIF.IsConnected(nStationID);

					if (bConnect != TRUE) {
						nIdx = i;
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL CTaskSystemCtrl::IsPossibleStationOff(int nPressUnit)
{
	int nCnt = 0;
	int nConnetChkCnt = 0;

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	// Site On �̰� ���� �Ǿ��ִ� Station �� Ȯ��
	std::vector<int> vIdx;

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single || nTesterOption == eTester_Agent || nTesterOption == eTester_EachSocket)
	{
		switch (nPressUnit) {
		case ePressUnit_Down_1: case ePressUnit_Down_2: case ePressUnit_Down_3: case ePressUnit_Down_4:
//		case ePressUnit_Up_1: case ePressUnit_Up_2: case ePressUnit_Up_3: case ePressUnit_Up_4:
			{
				vIdx.push_back(ePressUnit_Down_1);
				vIdx.push_back(ePressUnit_Down_2);
				vIdx.push_back(ePressUnit_Down_3);
				vIdx.push_back(ePressUnit_Down_4);

// 				vIdx.push_back(ePressUnit_Up_1);
// 				vIdx.push_back(ePressUnit_Up_2);
// 				vIdx.push_back(ePressUnit_Up_3);
// 				vIdx.push_back(ePressUnit_Up_4);
			}break;
		case ePressUnit_Down_5: case ePressUnit_Down_6: case ePressUnit_Down_7: case ePressUnit_Down_8:
//		case ePressUnit_Up_5: case ePressUnit_Up_6: case ePressUnit_Up_7: case ePressUnit_Up_8:
			{
				vIdx.push_back(ePressUnit_Down_5);
				vIdx.push_back(ePressUnit_Down_6);
				vIdx.push_back(ePressUnit_Down_7);
				vIdx.push_back(ePressUnit_Down_8);

// 				vIdx.push_back(ePressUnit_Up_5);
// 				vIdx.push_back(ePressUnit_Up_6);
// 				vIdx.push_back(ePressUnit_Up_7);
// 				vIdx.push_back(ePressUnit_Up_8);
			}break;
		}
	}
	else
	{
		switch (nPressUnit) {
		case ePressUnit_Down_1_1: case ePressUnit_Down_1_2: case ePressUnit_Down_2_1: case ePressUnit_Down_2_2:
		case ePressUnit_Down_3_1: case ePressUnit_Down_3_2: case ePressUnit_Down_4_1: case ePressUnit_Down_4_2:
		case ePressUnit_Up_1_1: case ePressUnit_Up_1_2: case ePressUnit_Up_2_1: case ePressUnit_Up_2_2:
		case ePressUnit_Up_3_1: case ePressUnit_Up_3_2: case ePressUnit_Up_4_1: case ePressUnit_Up_4_2:
		{
			vIdx.push_back(ePressUnit_Down_1_1);
			vIdx.push_back(ePressUnit_Down_1_2);
			vIdx.push_back(ePressUnit_Down_2_1);
			vIdx.push_back(ePressUnit_Down_2_2);
			vIdx.push_back(ePressUnit_Down_3_1);
			vIdx.push_back(ePressUnit_Down_3_2);
			vIdx.push_back(ePressUnit_Down_4_1);
			vIdx.push_back(ePressUnit_Down_4_2);

			vIdx.push_back(ePressUnit_Up_1_1);
			vIdx.push_back(ePressUnit_Up_1_2);
			vIdx.push_back(ePressUnit_Up_2_1);
			vIdx.push_back(ePressUnit_Up_2_2);
			vIdx.push_back(ePressUnit_Up_3_1);
			vIdx.push_back(ePressUnit_Up_3_2);
			vIdx.push_back(ePressUnit_Up_4_1);
			vIdx.push_back(ePressUnit_Up_4_2);

		}break;
		case ePressUnit_Down_5_1: case ePressUnit_Down_5_2: case ePressUnit_Down_6_1: case ePressUnit_Down_6_2:
		case ePressUnit_Down_7_1: case ePressUnit_Down_7_2: case ePressUnit_Down_8_1: case ePressUnit_Down_8_2:
		case ePressUnit_Up_5_1: case ePressUnit_Up_5_2: case ePressUnit_Up_6_1: case ePressUnit_Up_6_2:
		case ePressUnit_Up_7_1: case ePressUnit_Up_7_2: case ePressUnit_Up_8_1: case ePressUnit_Up_8_2:
		{
			vIdx.push_back(ePressUnit_Down_5_1);
			vIdx.push_back(ePressUnit_Down_5_2);
			vIdx.push_back(ePressUnit_Down_6_1);
			vIdx.push_back(ePressUnit_Down_6_2);
			vIdx.push_back(ePressUnit_Down_7_1);
			vIdx.push_back(ePressUnit_Down_7_2);
			vIdx.push_back(ePressUnit_Down_8_1);
			vIdx.push_back(ePressUnit_Down_8_2);

			vIdx.push_back(ePressUnit_Up_5_1);
			vIdx.push_back(ePressUnit_Up_5_2);
			vIdx.push_back(ePressUnit_Up_6_1);
			vIdx.push_back(ePressUnit_Up_6_2);
			vIdx.push_back(ePressUnit_Up_7_1);
			vIdx.push_back(ePressUnit_Up_7_2);
			vIdx.push_back(ePressUnit_Up_8_1);
			vIdx.push_back(ePressUnit_Up_8_2);
		}break;
		}
	}

	for (int i = 0; i < (int)vIdx.size(); i++) {
		if (nPressUnit == vIdx[i]) {
			if (nTesterOption == eTester_Single)
			{
				if (g_TaskPressUnit[vIdx[i]].GetStationOn() > 0 && g_AppleTestIF.IsConnected(vIdx[i] + 1) == TRUE) {
					nCnt++;
				}
				else if(g_TaskPressUnit[vIdx[i]].GetStationOn() > 0 && g_AppleTestIF.IsConnected(vIdx[i] + 1) == FALSE) 
					nConnetChkCnt++;
			}
			else if (nTesterOption == eTester_Dual)
			{
				BOOL bRet = g_AppleTestIF.IsConnected(vIdx[i] + 1);
				if (g_TaskPressUnit[vIdx[i] / 2].GetDirDualSocketOnCnt(0, i % 2) > 0 && g_AppleTestIF.IsConnected(vIdx[i] + 1) == TRUE) {
					nCnt++;
				}
				else if (g_TaskPressUnit[vIdx[i] / 2].GetDirDualSocketOnCnt(0, i % 2) > 0 && g_AppleTestIF.IsConnected(vIdx[i] + 1) == FALSE)
					nConnetChkCnt++;
			}
			else if(nTesterOption == eTester_Agent)
			{
// 				if (g_TaskPressUnit[vIdx[i]].GetStationOn() > 0 && g_AppleAgentIF.IsConnected(vIdx[i] + 1) == TRUE) {
// 					nCnt++;
// 				}
// 				else if (g_TaskPressUnit[vIdx[i]].GetStationOn() > 0 && g_AppleAgentIF.IsConnected(vIdx[i] + 1) == FALSE) 
// 					nConnetChkCnt++;
			}
			else // (nTesterOption == eTester_EachSocket) ���� DUT���� ���� ��� �ϴ� ���.
			{
				if (g_TaskPressUnit[vIdx[i]].GetStationOn() == TRUE && g_AppleTestIF.IsConnected(vIdx[i] + 1) == TRUE) {
					nCnt++;
				}
				else if (g_TaskPressUnit[vIdx[i]].GetStationOn() == TRUE && g_AppleTestIF.IsConnected(vIdx[i] + 1) == FALSE)
					nConnetChkCnt++;
			}
		}
	}


	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
	{
		if (nCnt >= 2) {
			//Module �� Station�� �ּ� 2�� �־�� �ȴ� , Default Bin ������
			return TRUE;
		}
	}
	else {
		if (nConnetChkCnt > 0)
			return TRUE;
	}

	return FALSE;
}


void CTaskSystemCtrl::SetDisConnectionStationIdx(int nPressUnit)
{
	m_vDisConnectionStationIdx.push_back(nPressUnit);
}

std::vector<int> CTaskSystemCtrl::GetDisConnectionStationIdx()
{
	return m_vDisConnectionStationIdx;
}

void CTaskSystemCtrl::ClearDisConnectionStationIdx()
{
	m_vDisConnectionStationIdx.clear();
}

void CTaskSystemCtrl::LotDataClear()
{
	MakeLog("MakeSummary Retry LotDataClear() Start");

	g_TaskSystemCtrl.LotInfoDataClear();

	g_TaskSystemCtrl.ResetUPHtm();
	g_DMCont.m_dmEQP.SN(NDM0_LotRunTime, 0);
	g_DMCont.m_dmEQP.SN(NDM0_LotStopTime, 0);

	// clear lot done data
	char szFileName[512] = { 0, };
	g_DMCont.m_dmEQP.SS(SDM0_LotDoneFileName, szFileName, sizeof(szFileName));

	g_DMCont.m_dmEQP.SN(NDM0_CurLotLoadingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotSortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotPassBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotFailBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput1Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput2Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput3Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput4Cnt, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_LoadingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_SortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_PassBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_FailBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output1Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output2Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output3Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output4Cnt, 0);

	g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT1_TOTAL, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT1_PASS, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT2_TOTAL, 0);
	g_DMCont.m_dmEQP.SN(NDM0_ART_RT2_PASS, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotJamCount, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotQAEmptyLoadCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_QaSampleAddSumLoadCnt, 0);

	g_DMCont.m_dmFeeder.SB(BDM10_FEEDER_TRAY_END, FALSE);

	g_DMCont.m_dmEQP.SN(NDM0_CurLot2D_Read_Error_Cnt, 0);

	g_DMCont.m_dmEQP.SB(BDM0_CYCLE_UPH_CHECK, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CycleCurLotSortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CycleLotStartTime, 0);

	g_Error.ResetCurrentLotJamCount();

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		g_DMCont.m_dmEQP.SN(NDM0_CleaningContact_Press1 + i, 0);
		g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), FALSE);

		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				int nPos = nX + nY*STATION_MAX_X_SITE + (STATION_MAX_PARA*i);
				g_DMCont.m_dmContac_Dn.SN(NDM1_GdBinCnt_Press1_1 + nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1 + nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_FailBinCnt_Press1_1 + nPos, 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1 + nPos, 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nPos, 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_2DIDFailBinCnt_Press1_1 + nPos, 0);
			}
		}
	}

	g_TaskSystemCtrl.YieldSampleClear();
	//g_TaskSystemCtrl.YieldVarianceClear();
	g_TaskSystemCtrl.YieldContiFailClear();
	g_TaskSystemCtrl.Yield1stFailClear();

	g_JamRate.ResetAllData();
	//if(nTrayEndFeed == DEF_LOTEND_TRAYFEED){
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_Initial);
	//	MakeLog("[Tray Feed end Make Summary]");
	//}else if(nTrayEndFeed == DEF_LOTEND_TRAYEND){
	//	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_RetestContinue);
	//	// record lot start time
	//	SYSTEMTIME time;
	//	::GetLocalTime( &time );
	//	CString strLOTstartTm = _T("");
	//	// sample 2016-01-05 21:10:14.343
	//	strLOTstartTm.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	//	g_DMCont.m_dmEQP.SS(SDM0_CurLotStartTm, (LPSTR)(LPCTSTR)strLOTstartTm, strLOTstartTm.GetLength()+1);
	//	g_TaskSystemCtrl.m_tcUPH.StartTimer();
	//	MakeLog("[Tray End end Make Summary]");
	//}

	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_LOT_MODE, -1);
	g_TaskSystemCtrl.MakeSummarySetTrayEndFeed(DEF_LOTEND_UNK);

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_SiteHistory1_1 + nX + nY*STATION_MAX_X_SITE + (i * STATION_MAX_PARA), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_SiteStatusHistory1_1 + nX + nY*STATION_MAX_X_SITE + (i * STATION_MAX_PARA), 0);
			}
		}
		g_DMCont.m_dmContac_Dn.SD(DDM1_SUMMARY_INDEX_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + i, 0);

		g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_TIME_ACC_CNT_STATION1 + i, 0);
		g_DMCont.m_dmContac_Dn.SD(DDM1_INDEX_TIME_ACC_TIME_STATION1 + i, 0);

		g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + i, 0);
		g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_CNT_STATION1 + i, 0);
	}
	//Make Summary �� 2D Barcode �ʱ�ȭ 
	g_2DBarcodeChk.ResetData();

	//if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
	//Socket ID �� Contact Count �� Check �ϱ� ����
	if (GetSocketIDContactCnt() > 0) {
		g_TaskSystemCtrl.MakeSocketIDContactCnt();
	}
	g_TaskSystemCtrl.ClearSocketIDContactCnt();
	//}	

	MakeLog("MakeSummary Retry LotDataClear() End");
}

int CTaskSystemCtrl::SocketCleanInterlock()
{
	if(g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean) == FALSE)
		return NO_ERROR;
	

	int nCleanDeivceCnt[eMaxTestPPCount] = { 0, };
	int nSiteModuleCnt[eMaxTestPPCount] = { 0, };
	int nSiteOnOff[eMaxPressUnitCount] = { 0, };
	std::vector<int> vModuleSetPressIdx[eMaxTestPPCount];

	for (int  i = 0; i < eMaxTestPPCount; i++)
	{
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_1 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_2 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_3 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_4 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_1 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_2 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_3 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_4 + (eMaxATS_Area / 2 * i));
	}

	int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		for (int nY = 0; nY < TestSite_Div_Y; nY++) {
			for (int nX = 0; nX < TestSite_Div_X; nX++) {
				int nSiteAddr = (STATION_MAX_PARA*i) + nX + nY*STATION_MAX_X_SITE;
				nSiteOnOff[i] += g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nSiteAddr);
			}
		}
	}

	for (int i = 0; i < eMaxTestPPCount; i++) {
		for (int nT = 0; nT < eMaxPressUnitCount / 2; nT++)
		{
			nSiteModuleCnt[i] += nSiteOnOff[vModuleSetPressIdx[i][nT]];
		}
	}
	
	for (int nidx = 0; nidx < eMaxTestPPCount; nidx++)
	{
		int nCleanDevCnt = SocketCleanDevCnt(nidx);
		if (nSiteModuleCnt[nidx] > 0 && nCleanDevCnt == 0)
		{
			return ERR_AUTO_SOCKET_CLEAN_BUFFER1_DEIVCE_NOT_CHECK+ nidx;
		}
	}

	return NO_ERROR;
}

int CTaskSystemCtrl::SocketCleanDevCnt(int nIdx)
{

	int nCleanDvcCnt = 0;
	ST_DD_DEVICE stDeviceData;

	int nClnBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nClnBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	std::vector<int> vModuleSetPressIdx[eMaxTestPPCount];

	for (int i = 0; i < eMaxTestPPCount; i++)
	{
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_1 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_2 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_3 + (eMaxATS_Area / 2 * i));
		vModuleSetPressIdx[i].push_back(ePressUnit_Down_4 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_1 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_2 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_3 + (eMaxATS_Area / 2 * i));
// 		vModuleSetPressIdx[i].push_back(ePressUnit_Up_4 + (eMaxATS_Area / 2 * i));
	}


	for (int nY = 0; nY < nClnBuff_Y; nY++)
	{
		for (int nX = 0; nX < nClnBuff_X; nX++)
		{
			stDeviceData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + nIdx, nX, nY, &stDeviceData);

			if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
				nCleanDvcCnt++;
		}
	}
	

	int nMaxBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
	int nMaxBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);


	for (int nBuffY = 0; nBuffY < nMaxBuff_Y; nBuffY++)
	{
		for (int nBuffX = 0; nBuffX < nMaxBuff_X; nBuffX++)
		{
			stDeviceData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_ART_BUFF + nIdx, nBuffX, nBuffY, &stDeviceData);

			if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
				nCleanDvcCnt++;
		}
	}
	
	for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			stDeviceData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + nIdx, nX, nY, &stDeviceData);
			if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
				nCleanDvcCnt++;
		}
	}
	
	

	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


	for(int vidx = 0; vidx < (int)vModuleSetPressIdx[nIdx].size(); vidx++)
	for (int nY = 0; nY < nSiteDivY; nY++)
	{
		for (int nX = 0; nX < nSiteDivX; nX++)
		{
			stDeviceData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + vModuleSetPressIdx[nIdx][vidx], nX, nY, &stDeviceData);

			if (stDeviceData.sExist == DEF_EXIST && strcmp(stDeviceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
				nCleanDvcCnt++;
			}
		}
	}
	
	return nCleanDvcCnt;
}


void CTaskSystemCtrl::LotTesterModeText(CString& strHandlerMode, CString& strMode)
{
	int nMD_TesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	switch (nLotStartMode)
	{
	case eStartMD_Initial: strHandlerMode = _T("FT"); break;
	case eStartMD_Retest: strHandlerMode = _T("RT"); break;
	case eStartMD_AutoGRR: strHandlerMode = _T("GRR"); break;
	case eStartMD_QA: strHandlerMode = _T("QA"); break;
	case eStartMD_AutoLoop: strHandlerMode = _T("LOOP"); break;
	case eStartMD_InitContinue: strHandlerMode = _T("FT Continue"); break;
	case eStartMD_RetestContinue: strHandlerMode = _T("RT Continue"); break;
	case eStartMD_AutoGRR_Continue: strHandlerMode = _T("GRR FT Continue"); break;
	case eStartMD_QA_Continue: strHandlerMode = _T("QA FT Continue"); break;
	case eStartMD_AutoLoop_Continue: strHandlerMode = _T("LOOP FT Continue"); break;
	}

	switch (nMD_TesterIF)
	{
	case eTesterIF_Mode_Normal: strMode = _T("PRODUCTION"); break;
	case eTesterIF_Mode_Grr: strMode = _T("GRR"); break;
	case eTesterIF_Mode_LoopTest: strMode = _T("LOOP TEST"); break;
	case eTesterIF_Mode_Audit: strMode = _T("AUDIT"); break;
	}
}






CString CTaskSystemCtrl::GetTestModeintToString(int nTestMode)
{
	CString strTestMode = _T("");
	switch (nTestMode)
	{
	case _eTestModeSLT1: { strTestMode = "SLT1";}break;
	case _eTestModeSLTQA1: { strTestMode = "SLTQA1";}break;
	default:
		break;
	}
	return strTestMode;
}

int CTaskSystemCtrl::GetTestModeStringToint(CString strTestMode)
{
	int nTestMode=-1;

	if (strTestMode == "SLT1")
		nTestMode = _eTestModeSLT1;
	else if(strTestMode == "SLTQA1")
		nTestMode = _eTestModeSLTQA1;
	else 
		nTestMode = _eTestModeNull;

	return nTestMode;
}

CString CTaskSystemCtrl::GetLotCmdString(int nLotCmd)
{
	CString strLotCmd = _T("");
	switch (nLotCmd)
	{
	case eAutoLotCmd_None: {strLotCmd = "eAutoLotCmd_None";}break;
	case eAutoLotCmd_Initial: {strLotCmd = "eAutoLotCmd_Initial";}break;
	case eAutoLotCmd_Start: {strLotCmd = "eAutoLotCmd_Start";}break;
	case eAutoLotCmd_Pause: {strLotCmd = "eAutoLotCmd_Pause";}break;
	case eAutoLotCmd_Resume: {strLotCmd = "eAutoLotCmd_Resume";}break;
	case eAutoLotCmd_Retry: {strLotCmd = "eAutoLotCmd_Retry";}break;
	case eAutoLotCmd_Skip: {strLotCmd = "eAutoLotCmd_Skip";}break;
	case eAutoLotCmd_CleanOut: {strLotCmd = "eAutoLotCmd_CleanOut";}break;
	case eAutoLotCmd_AlarmPause: {strLotCmd = "eAutoLotCmd_AlarmPause";}break;
	default:
		break;
	}

	return strLotCmd;
}

