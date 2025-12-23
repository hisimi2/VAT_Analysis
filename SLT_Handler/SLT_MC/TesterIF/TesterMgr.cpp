#include "StdAfx.h"
#include "TesterMgr.h"
#include "Tester.h"
#include <afxstr.h>
#include "TesterIF.h"
#include <atlcoll.h>
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CCriticalSection  m_cs;

CTesterMgr::CTesterMgr(void)
{
	::InitializeCriticalSection(m_cs);
	m_nTesterOption = 0;

	int nSocketNumber = 0;
	for (int nTester = 0; nTester < MAX_TESTER_DUAL; nTester++)
	{
		for (int nSocketNo = 0; nSocketNo < MAX_TESTER_1EA_SOCKET_CNT; nSocketNo++)
		{
			m_veIdentityNo[nTester].push_back(++nSocketNumber);
		}
	}	
}


CTesterMgr::~CTesterMgr(void)
{
	DeleteListAll(m_listTester);
}

void CTesterMgr::DeleteListAll(CList<CTester*> &list)
{
	POSITION pos = list.GetHeadPosition();

	while(pos != NULL)
	{
		CTester* tester = list.GetNext(pos);
		delete tester;
		tester->m_pSock = NULL;
	}

	list.RemoveAll();
}
//Watt 패키지 전용 Tester PC가 한대라서 통신 끊어지면 전부 끊어준다.
void CTesterMgr::DisconnectSockAll()
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		POSITION posDel = pos;
		CTester* tester = m_listTester.GetAt(pos);

		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			tester->Disconnection();
			//CTesterStatus* tester_status = &tester->GetTesterStatus();
			//g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (tester_status->m_nStationID - 1), FALSE);
			//tester->m_pSock->Close();

			//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_SSH) == TRUE)
			//{
			//	m_SshClient.Send("/usr/bin/pkill -9 python");
			//	m_Ssh Client.Send("open /Library/FCT/TT/TestGUIApplication.app");
			//}			
			delete tester;
			tester->m_pSock = NULL;
			m_listTester.RemoveAt(pos);
		}
		else {
			m_listTester.GetNext(pos);
		}
	}
	LeaveCriticalSection(m_cs);
}

BOOL CTesterMgr::DisconnectSock(CTesterClientSock* sock)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		POSITION posDel = pos;
		CTester* tester = m_listTester.GetAt(pos);

		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (tester->m_pSock == sock)
			{
				tester->Disconnection();
				//CTesterStatus* tester_status = &tester->GetTesterStatus();
				//g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (tester_status->m_nStationID - 1), FALSE);
				//tester->m_pSock->Close();

				//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_SSH) == TRUE)
				//{
				//	m_SshClient.Send("/usr/bin/pkill -9 python");
				//	m_Ssh Client.Send("open /Library/FCT/TT/TestGUIApplication.app");
				//}			
				delete tester;
				tester->m_pSock = NULL;
				m_listTester.RemoveAt(pos);
				LeaveCriticalSection(m_cs);
				return TRUE;
			}
			else
			{
				m_listTester.GetNext(pos);
			}
		}
		else {
			m_listTester.GetNext(pos);
		}
	}
	LeaveCriticalSection(m_cs);

	return TRUE;
}
//Carpenter 패키지
void CTesterMgr::SendQueryInformation()
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	CTesterStatus enumTesterStatus;
	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) != NULL)
			{
				enumTesterStatus = tester->GetTesterStatus();

				if (enumTesterStatus.m_nTesterInformation == _eSetTesterInformation::_TesterInformationRegReceiveOK)
				{
					tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterInformationReq);
					tester->SendAction(ACTION_INFO, NULL, NULL);	//information 요청 한다.
					Sleep(10);
				}
				else if (enumTesterStatus.m_nTesterInformation == _eSetTesterInformation::_TesterInformationOK)
				{
					tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterInformationNeedResetSend);
					tester->SendAction(ACTION_RESET, NULL, NULL);	//information 요청 한다.
					Sleep(10);
				}
				if (nProjectOption == DEF_PROJECT_CARPENTERS) { // Carpenter 신규 패키지에서는 Tester 연결이 정상적으로 진행 된 후, Tester Script를 보내 준다.
					CString strTPRev;
					char szTP_Rev[MAX_PATH] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, MAX_PATH);
					strTPRev.Format("%s", szTP_Rev);
					if (enumTesterStatus.m_nTesterInformation == _eSetTesterInformation::_TesterInformationNeedResetOK && strTPRev.IsEmpty() != TRUE)
					{
						tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterScriptInformationNeedResetSend);
						tester->SendAction(ACTION_TEST_SCRIPT_INFO, NULL, NULL);	//information 요청 한다.				
						Sleep(10);
					}
				}		
			}	
		}

		m_listTester.GetNext(pos);
	}
	LeaveCriticalSection(m_cs);
}

void CTesterMgr::SendQuerySetAPLInfo(BOOL bManual)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	CTesterStatus cTesterStatus;
	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) != NULL)
			{
				cTesterStatus = tester->GetTesterStatus();
				CString strTPRev;
				char szTP_Rev[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, MAX_PATH);
				strTPRev.Format("%s", szTP_Rev);

				CString strTPProg;
				char szTP_Prog[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szTP_Prog, MAX_PATH);
				strTPProg.Format("%s", szTP_Prog);

				if (cTesterStatus.m_nTesterInformation == _eSetTesterInformation::_TesterAPLInfoSend || bManual == eEqpCtrlMode_Manual)
				{
					if (strcmp(strTPRev, "") != 0 && strcmp(szTP_Prog, "") != 0)
					{
						if (bManual == eEqpCtrlMode_Auto) {
							tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterAPLInfoSendResetOK);
						}
						tester->SendTestAPLInfoAction();	//information 요청 한다.				
						Sleep(10);
					}
				}
			}
		}

		m_listTester.GetNext(pos);
	}
	LeaveCriticalSection(m_cs);
}



void CTesterMgr::SendQuerySetTestMode(BOOL bManual)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	CTesterStatus cTesterStatus;
	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) != NULL)
			{		
				cTesterStatus = tester->GetTesterStatus();
				char szOperation[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation, szOperation, MAX_PATH);

				int nTestMode = g_TaskSystemCtrl.GetTestModeStringToint(szOperation);
				LPARAM lparam = (LPARAM)nTestMode;
				if(cTesterStatus.m_nTesterInformation == _eSetTesterInformation::_TesterScriptHeartBeatNeedResetSend || bManual == eEqpCtrlMode_Manual)
				{
					if (strcmp(szOperation, "") != 0 && nTestMode != _eTestModeNull)
					{
						if (bManual == eEqpCtrlMode_Auto) {
							tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterScriptHeartBeatNeedResetOK);
						}					
						tester->SendAction(ACTION_SET_TEST_MODE, NULL, (LPARAM)lparam);	//information 요청 한다.				
						Sleep(10);
					}
				}
			}
		}

		m_listTester.GetNext(pos);
	}
	LeaveCriticalSection(m_cs);
}



void CTesterMgr::SendSetTestMode(int nStationID)
{
	CTester* tester = GetTester(nStationID);
	if (tester == NULL) {
		return;
	}

	char szOperation[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation, szOperation, sizeof(szOperation));
	LPARAM lparam = g_TaskSystemCtrl.GetTestModeStringToint(szOperation);

	tester->SendAction(ACTION_SET_TEST_MODE, WPARAM(NULL), lparam);
}

void CTesterMgr::SetTestMode(int stationID, CString info)
{
	CTester* tester = GetTester(stationID);
	if (tester == NULL) {
		return;
	}

	CTesterStatus* tester_status = &tester->GetTesterStatus();
	std::vector<std::pair<std::string, std::string>> tester_data;
	tester_status->PasingMsgType(info, tester_data, DEF_MSG_TYPE_TEST_MODE);

	int nTestMode = atoi(tester_data[0].first.c_str());
	tester->SetTestMode(nTestMode);
	
}


void CTesterMgr::SendQueryScriptInfromation()
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) != NULL)
			{
				if (nProjectOption == DEF_PROJECT_CARPENTERS) { // Carpenter 신규 패키지에서는 Tester 연결이 정상적으로 진행 된 후, Tester Script를 보내 준다.
					CString strTPRev;
					char szTP_Rev[MAX_PATH] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, MAX_PATH);
					strTPRev.Format("%s", szTP_Rev);
					if(strTPRev.IsEmpty() != TRUE)
					{
						tester->SendAction(ACTION_TEST_SCRIPT_INFO, NULL, NULL);	//information 요청 한다.				
						Sleep(10);
					}
				}

			}
		}

		m_listTester.GetNext(pos);
	}
	LeaveCriticalSection(m_cs);
}

void CTesterMgr::SetTestScriptInfomation(int stationID, CString info)
{
	CTester* tester = GetTester(stationID);
	if (tester == NULL) {
		return;
	}

	CTesterStatus* tester_status = &tester->GetTesterStatus();
	std::vector<std::pair<std::string, std::string>> tester_data;
	tester_status->PasingMsgType(info, tester_data, DEF_MSG_TYPE_TEST_SCRIPT);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{
		CString strTestScriptInfo = tester_data[0].first.c_str();
		tester->SetTestScript(strTestScriptInfo);
	}
}

void CTesterMgr::SetInformation( int stationID, CString info ) 
{
	CTester* tester = GetTester( stationID);
	if (tester == NULL) {
		return;
	}

	CTesterStatus* tester_status = &tester->GetTesterStatus();
	std::vector<std::pair<std::string, std::string>> tester_data;
	tester_status->PasingMsgType( info, tester_data, DEF_MSG_TYPE_INFORMATION);

	tester->SetInfo( info );

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{
		tester->SetInformation(tester_data);
	}
	else
	{
		tester->SetInformation(
			tester_data[eTesterInfo_TesterID].second.c_str(),
			tester_data[eTesterInfo_SocketID].second.c_str(),
			tester_data[eTesterInfo_SW_Version].second.c_str(),
			tester_data[eTesterInfo_IP].second.c_str()
		);
	}

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 	tester->SetInformation(tester_data);
// #else
// 	tester->SetInformation(
// 		tester_data[ eTesterInfo_TesterID ].second.c_str(),
// 		tester_data[ eTesterInfo_SocketID ].second.c_str(),
// 		tester_data[ eTesterInfo_SW_Version ].second.c_str(),
// 		tester_data[ eTesterInfo_IP ].second.c_str()
// 	);
// #endif


}

void CTesterMgr::SendQuerySetLotStatus(eTesterLotStatus _eTesterLotStatus)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	CTesterStatus cTesterStatus;
	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) != NULL)
			{				
				tester->SendTestLotStatusAction(_eTesterLotStatus);		
				Sleep(10);						
			}
		}

		m_listTester.GetNext(pos);
	}
	LeaveCriticalSection(m_cs);
}

BOOL CTesterMgr::RegTester( RecvData& recvData, CTesterClientSock* sock)
{
	// 중복해서 registration을 수신 할 경우 무시 한다.
	EnterCriticalSection(m_cs);
	CTester* tester = GetTester(recvData.m_nStationID);
	if( tester != NULL ){
		UnRegTester(recvData);
	}

	tester = new CTester;

	tester->m_SendData.m_DataHeader.m_nData = recvData.m_DataHeader.m_nData;
	tester->m_SendData.m_DataHeader.m_bit.m_nType = ACTION;
	tester->m_pSock = sock;
	tester->SetRegistration(REG_TESTER);
	tester->SetStatus(STATUS_INIT);
	tester->SetStationID(recvData.m_nStationID); // tester id 확인.

//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{

		CTesterStatus* tester_status = &tester->GetTesterStatus();
		std::vector<std::pair<std::string, std::string>> tester_data;
		tester_status->PasingMsgType(recvData.m_cRegStr, tester_data, DEF_MSG_TYPE_REGIST);
		
		CString strDutNo = tester_data[0].first.c_str();

		LPCTSTR lpctstr = static_cast<LPCTSTR>(strDutNo);

		int nIdentityDutNo = _ttoi(lpctstr);

		tester->SetRegiDutIdentityNo(nIdentityDutNo);
		m_listTester.AddTail(tester);
		tester->Connection();

		tester->SetTesterInformationStatus(_eSetTesterInformation::_TesterInformationRegReceiveOK); // 정상적으로 테스터로 부터 Registration 수신 완료

		// 	ST_STATION_TO_IDENTITYNO stStationToIdentity;
		// 	stStationToIdentity.nStationNo = recvData.m_nStationID;
		// 	stStationToIdentity.nIdentityNo = nIdentityDutNo;
		// 
		// 	m_veStationToIdentityNo.push_back(stStationToIdentity);
	}
	else
	{		
		m_listTester.AddTail(tester);
		SetInformation(recvData.m_nStationID, recvData.m_cRegStr);
		tester->Connection();
	}
//#endif
	//m_listTester.AddTail(tester);
	//tester->Connection();


	//g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (recvData.m_nStationID - 1), TRUE);
	//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_SSH) == TRUE)
	//{
	//	CTesterStatus m_teststat = tester->GetTesterStatus();
	//	m_SshClient.Disconnect();
	//	m_SshClient.Connect(m_teststat.m_sIP, 22, "gdlocal", "gdlocal");
	//}

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 	tester->SeTesterInformationStatus(_eSetTesterInformation::_TesterInformationRegReceiveOK); // 정상적으로 테스터로 부터 Registration 수신 완료
// #endif

	LeaveCriticalSection(m_cs);
	return TRUE;
}

BOOL CTesterMgr::UnRegTester( RecvData & recvData )
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	while( pos != NULL )
	{
		CTester* tester = m_listTester.GetAt( pos );
		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
		{
			if (tester->GetStationID() == recvData.m_nStationID)
			{				
				tester->Disconnection();
				//g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (recvData.m_nStationID - 1), FALSE);
				//tester->m_pSock->Close();

				//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_SSH) == TRUE)
				//{
				//	m_SshClient.Send("/usr/bin/pkill -9 python");
				//	m_SshClient.Send("open /Library/FCT/TT/TestGUIApplication.app");
				//}


				delete tester;
				tester->m_pSock = NULL;
				m_listTester.RemoveAt(pos);
				LeaveCriticalSection(m_cs);
				return TRUE;
			}
			m_listTester.GetNext(pos);
		}
		else {
			m_listTester.GetNext(pos);
		}
	}
	LeaveCriticalSection(m_cs);
	return FALSE;
}

//Watt 패키지 전용 Tester 8개당 PC1개로 구성 되어 있어서, 모두 끊어 준다.
void CTesterMgr::AllUnRegTester()
{
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		UnRegTesterEach(i+1);
		Sleep(10);
	}

}
// Yan made Tester Carpenter, Cambeltown
std::vector<int> CTesterMgr::GetTesterSocketIndexRange(int nIdentityNo)
{
	for (int nTester = 0; nTester < MAX_TESTER_DUAL; nTester++)
	{
		for (int nSocketNo = 0; nSocketNo < m_veIdentityNo[nTester].size(); nSocketNo++)
		{
			if (nIdentityNo == m_veIdentityNo[nTester][nSocketNo])
			{
				return m_veIdentityNo[nTester];
			}
		}
	}
}

void CTesterMgr::UnRegTesterIdentity(RecvData& recvData)
{
	CTester* tester = GetTester(recvData.m_nStationID);	
	if (tester == NULL)
		return;

	CTesterStatus* tester_status = &tester->GetTesterStatus();
	std::vector<std::pair<std::string, std::string>> tester_data;
	tester_status->PasingMsgType(recvData.m_cRegStr, tester_data, DEF_MSG_TYPE_REGIST);

	if (tester_data.size() == 0)
		return;

	CString strDutNo = tester_data[0].first.c_str();

	LPCTSTR lpctstr = static_cast<LPCTSTR>(strDutNo);

	int nIdentityDutNo = _ttoi(lpctstr);	
	std::vector<int> vTesterSocketIdxRange;
	vTesterSocketIdxRange = GetTesterSocketIndexRange(nIdentityDutNo);

	for (int i = 0; i < vTesterSocketIdxRange.size(); i++)
	{
		UnRegTesterEach(vTesterSocketIdxRange[i], TRUE);
		Sleep(10);
	}
}



CTester* CTesterMgr::GetTester(int stationID, BOOL bUseIdentityNo /*= FALSE*/ )
{
	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetNext(pos);

		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption == eTester_EachSocket)
			{
				//ASSERT(stationID > 0 && stationID <= 64); // dut no는 1~64까지 등록 가능 하다.
				// 물리적인 번호를 호출 하여 station ID가가 아닌 identity dut No으로 구분 한다.

				if (bUseIdentityNo == TRUE)
				{
					if (tester->GetRegiDutIdentityNo() == stationID) // 1dut 별 소켓 통신은 Station ID가 physical Dut No,이다.
					{
						return tester;
					}
				}
				else
				{
					if (tester->GetStationID() == stationID)
					{
						return tester;
					}
				}
			}
			else
			{
				if (tester->GetStationID() == stationID)
				{
					return tester;
				}
			}
		}

	}
	return NULL;
}

//Single, Dual은 사용 안함.
int CTesterMgr::GetDutIdentityNo(int stationID)
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption != eTester_EachSocket)
		return ERR_FAILURE;

	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetNext(pos);
		
#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{
			//ASSERT(stationID > 0 && stationID <= 64); // dut no는 1~64까지 등록 가능 하다.
			// 물리적인 번호를 호출 하여 station ID가가 아닌 identity dut No으로 구분 한다.

			if (tester->GetStationID() == stationID)
			{
				return tester->GetRegiDutIdentityNo();
			}
		}
#else		

	if (tester->GetStationID() == stationID)
	{
		return tester;
	}
		
#endif
	}
	return ERR_NO_ERROR;
}

BOOL CTesterMgr::SendActionAll(int action)
{
	POSITION pos = m_listTester.GetHeadPosition();

	while(pos != NULL)
	{
		CTester* tester = m_listTester.GetNext(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterTotalNullPointer) != NULL)
		{	
			if (tester->m_pSock->m_bConnect == TRUE)
			{
				tester->SendAction(action);
			}		
		}		
	}

	return TRUE;
}

int CTesterMgr::SendAction(int stationID, int action, WPARAM wParam, LPARAM lParam,int* pDirSiteOnOff, int nTesterMode)
{
	//int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
	if (nTesterMode == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
	{
		CTester* tester = GetTester(stationID);

		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) == NULL){
			return ERR_IS_NOT_REG_TESTER_ID;
		}
		if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) == NULL){
			return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
		}
		if (tester->m_pSock->m_bConnect == FALSE) {
			return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
		}


		// 	if(tester != NULL && tester->m_pSock != NULL 
		// 		&& tester->m_pSock->m_bConnect == TRUE)
		// 	{
		tester->SendAction(action, wParam, lParam);
		//	}
		// 	else
		// 	{
		// 		return FALSE;
		// 	}
	}
	else if(nTesterMode == eTester_Dual)  // Dual Tester [8/27/2021 donghyun.shin]
	{
		CTester* testerDual[eMaxTester];
		testerDual[eRearTester] = GetTester(DualTesterIndex(stationID, eRearTester)); // Rear Tester [8/30/2021 donghyun.shin]
		testerDual[eFrontTester] = GetTester(DualTesterIndex(stationID, eFrontTester));     // Front Tester

		int nTesterNum[eMaxTester];
		nTesterNum[eRearTester] = DualTesterIndex(stationID, eRearTester); // Rear Tester [8/30/2021 donghyun.shin]
		nTesterNum[eFrontTester] = DualTesterIndex(stationID, eFrontTester);     // Front Tester

		switch (action)
		{
		case ACTION_START:
		{	
			WPARAM wDataParam[eMaxTester];

			ST_TESTER_ACTION_PARAM* stActionParam = (ST_TESTER_ACTION_PARAM*)wParam;
			ST_TESTER_ACTION_PARAM stActionParamBuf[eMaxTester][STATION_MAX_X_SITE];

			for (int i = 0; i < STATION_MAX_X_SITE; i++)
			{
				stActionParamBuf[eRearTester][i] = stActionParam[i];
				stActionParamBuf[eFrontTester][i] = stActionParam[STATION_MAX_X_SITE + i];
			}

			for (int t = 0; t < eMaxTester; t++)
			{			
				if(pDirSiteOnOff[t] == FALSE)
					continue;

				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL) {
				
					return ERR_IS_NOT_REG_TESTER_ID;
				}
				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterSockNullPointer) == NULL) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}
				if (testerDual[t]->m_pSock->m_bConnect == FALSE) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}

				wDataParam[t] = (WPARAM)stActionParamBuf[t];
				testerDual[t]->SendAction(action, wDataParam[t], lParam);
				Sleep(30);  // 8개로 분리 할 경우 30 x 7이라서 210ms 소요 됨.
			}
		}break;
		case ACTION_STATUS:
		case ACTION_STOP:
		{
			for (int t = 0; t < eMaxTester; t++)
			{			
				if (pDirSiteOnOff[t] == FALSE)
					continue;

				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL) {
					return ERR_IS_NOT_REG_TESTER_ID;
				}

				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterSockNullPointer) == NULL) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}
				if (testerDual[t]->m_pSock->m_bConnect == FALSE) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}

				testerDual[t]->SendAction(action, wParam, lParam);
			}
		}
		break;
		case ACTION_SET_TEST_MODE:
		{
			for (int t = 0; t < eMaxTester; t++)
			{
			
				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL) {
					return ERR_IS_NOT_REG_TESTER_ID;
				}
				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterSockNullPointer) == NULL) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}

				if (testerDual[t]->m_pSock->m_bConnect == FALSE) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}

				testerDual[t]->SendAction(action, wParam, lParam);
			}
		}
		default:
			break;
		}
	}
	else // each socket
	{
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		CTester* testerEachSocket[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };
		int nSocketcnt = 0;
		for (nSocketcnt = 0; nSocketcnt < TestSite_Div_X*TestSite_Div_Y; nSocketcnt++)
		{		
			int nPhysicalCommSocketNo = EachTesterIndex(stationID, nSocketcnt);
			testerEachSocket[nSocketcnt] = GetTester(nPhysicalCommSocketNo, TRUE);
		}

		switch (action)
		{
		case ACTION_START:
		{
			WPARAM wDataParam;

			ST_TESTER_ACTION_PARAM* stActionParam = (ST_TESTER_ACTION_PARAM*)wParam;
			int nSocketIdx = 0;
	

			for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
			{
				if (pDirSiteOnOff != NULL)
				{
					if (pDirSiteOnOff[t] == FALSE)
					{
						continue;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterNullPointerInstance) == NULL) { // 제품도 있고 DUT도 ON상태인데 클라이언트 통신 소켓이 생성이 되어 있지 않으면 알람발생.
						return ERR_IS_NOT_REG_TESTER_ID;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterSockNullPointer) == NULL) {
						return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
					}

					if (testerEachSocket[t]->m_pSock->m_bConnect == FALSE) {
						return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
					}

				}
				else // dut on/off 정보를 parameter로 넘기지 않으면 DUT 소켓 통신 미연결시. skip 한다.
				{
					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterNullPointerInstance) == NULL) { // 제품도 있고 DUT도 ON상태인데 클라이언트 통신 소켓이 생성이 되어 있지 않으면 알람발생.
						continue;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterSockNullPointer) == NULL) {
						continue;
					}

					if (testerEachSocket[t]->m_pSock->m_bConnect == FALSE) {
						continue;
					}
				}

				if (wParam != NULL)
				{
					//if (stActionParam[t].m_bSiteOn != true)
					//{
					//	continue;
					//}
				}

				
				wDataParam = (WPARAM)&stActionParam[t];
				testerEachSocket[t]->SendAction(action, wDataParam, lParam);
				Sleep(30);  // station당 8dut라서 8회 보내어 30ms x 7이라서 210ms 소요 됨.
			}
		}break;
		case ACTION_STATUS:
		case ACTION_STOP:
		case ACTION_RESET:
		case ACTION_TEST_SCRIPT_INFO:
		{
			for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
			{
				if (pDirSiteOnOff != NULL)
				{
					if (pDirSiteOnOff[t] == FALSE)
					{
						continue;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterNullPointerInstance) == NULL) {
						return ERR_IS_NOT_REG_TESTER_ID;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterSockNullPointer) == NULL) {
						return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
					}
					if (testerEachSocket[t]->m_pSock->m_bConnect == FALSE) {
						return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
					}
				}
				else // dut on/off 정보를 parameter로 넘기지 않으면 DUT 소켓 통신 미연결시. skip 한다.
				{
					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterNullPointerInstance) == NULL) { // 제품도 있고 DUT도 ON상태인데 클라이언트 통신 소켓이 생성이 되어 있지 않으면 알람발생.
						continue;
					}

					if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterSockNullPointer) == NULL) {
						continue;
					}
					if (testerEachSocket[t]->m_pSock->m_bConnect == FALSE) {
						continue;
					}

				
				}

				ST_TESTER_ACTION_PARAM* stActionParam = (ST_TESTER_ACTION_PARAM*)wParam;
				int nSocketIdx = 0;


				if (wParam != NULL)
				{
					//if (stActionParam[t].m_bSiteOn != true)
					//{
					//	continue;
					//}
				}
				
				testerEachSocket[t]->SendAction(action, wParam, lParam);
			}
		}
		break;
		case ACTION_SET_TEST_MODE:	// yanmade 테스터 사양에서는 미사용.
		{
			for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
			{
				if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterNullPointerInstance) == NULL) {
					return ERR_IS_NOT_REG_TESTER_ID;
				}
				if (TesterNullPointerCheck(testerEachSocket[t], _eCheckTesterSockNullPointer) == NULL) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}
				if (testerEachSocket[t]->m_pSock->m_bConnect == FALSE) {
					return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				}

				testerEachSocket[t]->SendAction(action, wParam, lParam);
			}
		}
		default:
			break;
		}
	}

	return ERR_NO_ERROR;
}

int CTesterMgr::GetStatus(int stationID, int* pDirSiteOnOff, int nTesterOption,int* nTesterNum)
{
	if (nTesterOption == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
	{
		CTester* tester = GetTester(stationID);

		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
		{
			return tester->GetStatus();
		}
	}
	else if(nTesterOption == eTester_Dual)
	{
		int nGetStatus[eMaxTester] = { 0, };
		int nTesterOnOff = { 0, };
		int nReturnStatus = 0;
		CTester* testerDual[2];
		testerDual[eRearTester] = GetTester(DualTesterIndex(stationID, eRearTester)); // Rear Tester [8/30/2021 donghyun.shin]
		testerDual[eFrontTester] = GetTester(DualTesterIndex(stationID, eFrontTester));     // Front Tester


// 		nTesterNum[eRearTester] = DualTesterIndex(stationID, eRearTester);
// 		nTesterNum[eFrontTester] = DualTesterIndex(stationID, eFrontTester);
		for (int t = 0; t < eMaxTester; t++)
		{
			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) != NULL)
			{
				nGetStatus[t] = testerDual[t]->GetStatus();
			}

			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) != NULL && pDirSiteOnOff[t] > 0)
				nTesterOnOff++;			
		}

		if (nTesterOnOff >= eMaxTester)
		{
			if (nGetStatus[eRearTester] == STATUS_INIT && nGetStatus[eFrontTester] == STATUS_INIT)
				nReturnStatus = STATUS_INIT;
			else if (nGetStatus[eRearTester] == STATUS_IDLE && nGetStatus[eFrontTester] == STATUS_IDLE)
				nReturnStatus = STATUS_IDLE;
			else if (nGetStatus[eRearTester] == STATUS_TESTING && nGetStatus[eFrontTester] == STATUS_TESTING)
				nReturnStatus = STATUS_TESTING;
			else if (nGetStatus[eRearTester] == STATUS_ERROR && nGetStatus[eFrontTester] == STATUS_ERROR)
				nReturnStatus = STATUS_ERROR;
			else if (nGetStatus[eRearTester] == STATUS_UNREGISTER && nGetStatus[eFrontTester] == STATUS_UNREGISTER)
				nReturnStatus = STATUS_UNREGISTER;
			else
				nReturnStatus = STATUS_RESULT_WAIT;
		}
		else
		{
			for (int t = 0; t < eMaxTester; t++)
			{
				if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) != NULL && pDirSiteOnOff[t] > 0)
				{
					nReturnStatus = testerDual[t]->GetStatus();
				}
				else if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL && pDirSiteOnOff[t] > 0)
				{
					return RET_ERROR;
				}					
			}
		}
		return nReturnStatus;
	}
	else// if (nTesterOption == eTester_EachSocket) dut 단위로 통신 송수신 한다.
	{
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		int nGetStatus[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
		int nTesterOnOff = { 0, };
		int nReturnStatus = 0;
		CTester* testerOctal[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };

		int nSocketCnt = 0; 
		for (nSocketCnt = 0; nSocketCnt < TestSite_Div_X*TestSite_Div_Y; nSocketCnt++) // phsical 물리 번호
		{
			 // 전면 기준 
			 // 0 1 2 3
			 // 4 5 6 7
			if (pDirSiteOnOff[nSocketCnt] == DEF_ON) // DUT ON인 곳만 등록 한다.
			{
				// physical 물리 번호에 대한 물리위치에 대한 통신소켓을 지정 한다.
				int nPhysicalCommSocketNo = EachTesterIndex(stationID, nSocketCnt);
				testerOctal[nSocketCnt] = GetTester(nPhysicalCommSocketNo, TRUE);
			}
		}

		for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
		{
			if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterNullPointerInstance) == NULL && pDirSiteOnOff[t] == DEF_ON) // dut on인데 해당 dut에 등록된 client 통신 소켓이 없다.
			{
				return RET_ERROR;
			}

			if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterNullPointerInstance) != NULL && pDirSiteOnOff[t] == DEF_ON)
			{
				nGetStatus[t] = testerOctal[t]->GetStatus();
				nTesterOnOff++;
			}				
		}

		int nCntInit = 0;
		int nCntIdle = 0;
		int nCntTesting = 0;
		int nCntError = 0;
		int nCntUnregister = 0;

		int nLoopSocketcnt = 0;
		int nData = 0;

		for (nLoopSocketcnt = 0; nLoopSocketcnt < TestSite_Div_X*TestSite_Div_Y; nLoopSocketcnt++)
		{
			if (pDirSiteOnOff[nLoopSocketcnt] != DEF_ON) //해당 DUT가 socket off이거나 제품이 없다
			{
				continue;
			}

			nData = nGetStatus[nLoopSocketcnt];

			if (nData == STATUS_INIT)
			{
				nCntInit++;
			}
			else if (nData == STATUS_IDLE)
			{
				nCntIdle++;
			}
			else if (nData == STATUS_TESTING)
			{
				nCntTesting++;
			}
			else if (nData == STATUS_ERROR)		// error이면 바로 발생 시킨다.
			{
				return STATUS_ERROR;
			}
			else if (nData == STATUS_UNREGISTER)
			{
				nCntUnregister++;
			}
		}

		if (nCntInit == nTesterOnOff)
			nReturnStatus = STATUS_INIT;
		else if (nCntIdle == nTesterOnOff)
			nReturnStatus = STATUS_IDLE;
		else if(nCntTesting == nTesterOnOff) //(nCntTesting == nTesterOnOff)
			nReturnStatus = STATUS_TESTING;
		else if (nCntUnregister == nTesterOnOff)
			nReturnStatus = STATUS_UNREGISTER;
		else
			nReturnStatus = STATUS_RESULT_WAIT;
	
		return nReturnStatus;
	}
	return RET_ERROR;
}

int CTesterMgr::SetInformationStatus(int stationID, _eSetTesterInformation eStatus)
{
	CTester* tester = GetTester(stationID);

	if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetTesterInformationStatus(eStatus);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;

}
BOOL CTesterMgr::SetStatus(int stationID, int status)
{
	CTester* tester = GetTester( stationID );

	if(TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetStatus(status);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;
}

BOOL CTesterMgr::SetTimeStamp(int stationID, CString timeStamp)
{
	CTester* tester = GetTester(stationID, eIdentityNo);

	if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetHeartBit(timeStamp);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;
}


BOOL CTesterMgr::IsConnected(int stationID , int nTesterMode /*= eTester_Single*/, BOOL *pSocketOnffInfo /*= NULL*/)
{
	if (nTesterMode == eTester_Single) {
		CTester* tester = GetTester(stationID,eIdentityNo);

		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) == NULL)
		{
			return FALSE;
		}
		if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) == NULL)
		{
			return FALSE;
		}
		if (tester->m_pSock->m_bConnect == FALSE)
		{
			return FALSE;
		}

		return TRUE;

	}
	else if(nTesterMode == eTester_Dual){
		CTester* testerDual[eMaxTester];
		testerDual[eRearTester] = GetTester(DualTesterIndex(stationID, eRearTester));
		testerDual[eFrontTester] = GetTester(DualTesterIndex(stationID, eFrontTester));
		int nTesterConnetCount = 0;

		for (int t = 0; t < eMaxTester; t++)
		{
			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL)
			{
				continue;
			}
			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterSockNullPointer) == NULL)
			{
				continue;
			}
			if (testerDual[t]->m_pSock->m_bConnect == FALSE)
			{			
				continue;
			}

			nTesterConnetCount++;
		}

		if (nTesterConnetCount == eMaxTester)
		{
			return TRUE;
		}	
	}
	else // each socket
	{
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		CTester* testerOctal[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };

		int nDutcnt = 0;
		for (nDutcnt = 0; nDutcnt < TestSite_Div_X*TestSite_Div_Y; nDutcnt++)
		{
			int nPhysicalNo = EachTesterIndex(stationID, nDutcnt);
			testerOctal[nDutcnt] = GetTester(nPhysicalNo, TRUE);
		}

		int nTesterConnetCount = 0;
		for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
		{	
			int nSocketSts = DEF_ON;
			if (pSocketOnffInfo != NULL)
			{
				if ( pSocketOnffInfo[t] == DEF_ON ) // socket이 켜져 있는데도 불구 하고 통신 소켓이 생성 되지 않았다.
				{
					if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterNullPointerInstance) == NULL)
					{
						return FALSE;
					}
					if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterSockNullPointer) == NULL)
					{
						return FALSE;
					}		
					if (testerOctal[t]->m_pSock->m_bConnect != TRUE)
					{
						return FALSE;
					}								
				}
			}
		}

		return TRUE;
	}
		
	return FALSE;
}

BOOL CTesterMgr::SetEachTestResult(int stationID, int nResult, int nReserved, const char* szErrStr, const char* szDutSn, const char* szSocketID, const char* szErrCode, int nIdx)
{
	CTester* tester = GetTester(stationID);

	if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetEachTestResult(nResult, nReserved, szErrStr, szDutSn, szSocketID, szErrCode, nIdx);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;
}

BOOL CTesterMgr::SetTestResult(int stationID, int nResult, int nReserved, const char* szErrStr, const char* szDutSn, const char* szSocketID, int nIdx)
{
	CTester* tester = GetTester(stationID);

	if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetTestResult(nResult, nReserved, szErrStr, szDutSn, szSocketID, nIdx);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;
}


BOOL CTesterMgr::SetAPLInfo(int stationID, const char* szTestScriptInfo, const char* szTestSWVersion, const char* szTestRetest, const char* szTestHandlerID, const char* szTestDevice)
{
	CTester* tester = GetTester(stationID);

	if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
	{
		tester->SetAPLInfo(szTestScriptInfo, szTestSWVersion, szTestRetest, szTestHandlerID , szTestDevice);
	}
	else
	{
		return RET_ERROR;
	}

	return TRUE;
}

void CTesterMgr::GetTesterInfoAll()
{
	POSITION pos = m_listTester.GetHeadPosition();
	while(pos != NULL)
	{
		CTester* tester = m_listTester.GetNext(pos);
		if(TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
		{
			tester->SendAction(ACTION_INFO, NULL, NULL);
		}
	}
}

void CTesterMgr::SendHeartBeat(int ntesterID, RecvData recvData,  WPARAM wParam/*=NULL*/, LPARAM lParam/*=NULL*/)
{
	CTester* tester = GetTester(ntesterID, eIdentityNo);

	if(TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) == NULL){
		return; 
	}
	if(TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) == NULL ){
		return; 
	}
	if (tester->m_pSock->m_bConnect == FALSE) {
		return;
	}

	tester->SendHeartBeatAction(recvData, wParam, lParam);
}

void CTesterMgr::HeartBeatOverCutClient(int stationID, BOOL bUseIdentityNo)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
		{
			if (bUseIdentityNo == TRUE)
			{
				if (tester->GetRegiDutIdentityNo() == stationID)
				{
					tester->Disconnection();
					delete tester;
					m_listTester.RemoveAt(pos);
					LeaveCriticalSection(m_cs);
					return;
				}
			}
			else {
				if (tester->GetStationID() == stationID)
				{
					tester->Disconnection();
					delete tester;
					m_listTester.RemoveAt(pos);
					LeaveCriticalSection(m_cs);
					return;
				}
			}
		
			m_listTester.GetNext(pos);
		}
		else {
			m_listTester.GetNext(pos);
		}
	}
	LeaveCriticalSection(m_cs);
}

void CTesterMgr::UnRegTesterEach(int stationID, BOOL bUseIdentityNo)
{
	EnterCriticalSection(m_cs);
	POSITION pos = m_listTester.GetHeadPosition();

	while (pos != NULL)
	{
		CTester* tester = m_listTester.GetAt(pos);
		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) != NULL)
		{
			if (bUseIdentityNo == TRUE)
			{
				if (tester->GetRegiDutIdentityNo() == stationID)
				{
					tester->Disconnection();
					delete tester;
					m_listTester.RemoveAt(pos);
					LeaveCriticalSection(m_cs);
					return;
				}
			}
			else {
				if (tester->GetStationID() == stationID)
				{
					tester->Disconnection();
					delete tester;
					m_listTester.RemoveAt(pos);
					LeaveCriticalSection(m_cs);
					return;
				}
			}

			m_listTester.GetNext(pos);
		}
		else {
			m_listTester.GetNext(pos);
		}
	}
	LeaveCriticalSection(m_cs);
}


int CTesterMgr::DualTesterIndex(int stationID,int nDir)
{
	int nStationID=-1;

	if (nDir == eRearTester)
		nStationID = stationID * 2 - 1;
	else
		nStationID = stationID * 2;

	return nStationID;
}
// station 번호 받아서 처리 함.
int CTesterMgr::EachTesterIndex(int stationID, int nSocketNo)
{
// Station No.
// station 04  08
// station 03  07
// station 02  06
// station 01  05

// dut No.
//	04 08                 05  01
//	03 07				  06  02
//	02 06				  07  03
//	01 05				  08  04
//  Left station          Right station

// Machine Front

	int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nStationID = -1;

	nStationID = (stationID -1) * (TestSite_Div_X * TestSite_Div_Y) + nSocketNo + 1;
	return nStationID;
}


int CTesterMgr::LastResultReset(int stationID, int* nDirSiteOnOff)
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
	{
		CTester* tester = GetTester(stationID);

		if (TesterNullPointerCheck(tester, _eCheckTesterNullPointerInstance) == NULL) {
			return ERR_IS_NOT_REG_TESTER_ID;
		}
		if (TesterNullPointerCheck(tester, _eCheckTesterSockNullPointer) == NULL) {
			return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
		}
		if (tester->m_pSock->m_bConnect == FALSE) {
			return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
		}

		tester->LastResultReset();
	}
	else if(nTesterOption == eTester_Dual)
	{
		CTester* testerDual[2];
		testerDual[eRearTester] = GetTester(DualTesterIndex(stationID, eRearTester)); // Rear Tester [8/30/2021 donghyun.shin]
		testerDual[eFrontTester] = GetTester(DualTesterIndex(stationID, eFrontTester));     // Front Tester

		for (int t = 0; t < eMaxTester; t++)
		{
			if (nDirSiteOnOff[t] == FALSE)
				continue;

			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterNullPointerInstance) == NULL) {
				//return ERR_IS_NOT_REG_TESTER_ID;
				continue;
			}
			if (TesterNullPointerCheck(testerDual[t], _eCheckTesterSockNullPointer) == NULL) {
				//return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				continue;
			}
			if (testerDual[t]->m_pSock->m_bConnect == FALSE) {
				//return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				continue;
			}

			testerDual[t]->LastResultReset();
			Sleep(30);
		}
	}
	else // Each Socket
	{
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		CTester* testerOctal[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };

		int nDutCnt = 0;
		for (nDutCnt = 0; nDutCnt < TestSite_Div_X*TestSite_Div_Y; nDutCnt++)
		{
			int nPhysicalIdx = EachTesterIndex(stationID, nDutCnt);
			testerOctal[nDutCnt] = GetTester(nPhysicalIdx, TRUE);
		}

		for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
		{
			if (nDirSiteOnOff[t] == FALSE)
				continue;

			if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterNullPointerInstance) == NULL) {
				//return ERR_IS_NOT_REG_TESTER_ID;
				continue;
			}
			if (TesterNullPointerCheck(testerOctal[t], _eCheckTesterSockNullPointer) == NULL) {
				//return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				continue;
			}
			if (testerOctal[t]->m_pSock->m_bConnect == FALSE) {
				//return ERR_TESTER_AND_HANDLER_NOT_CONNECTED;
				continue;
			}

			testerOctal[t]->LastResultReset();
			Sleep(30);
		}
	}

	return NO_ERROR;
}


int CTesterMgr::TesterNullPointerCheck(CTester* tester, int nMode)
{
	switch (nMode)
	{
		case _eCheckTesterNullPointerInstance: 
		{
			if (tester == NULL || tester == DEF_TESTER_HEAP_POINTER_DELETE || tester == DEF_TESTER_POINTER_DELETE)
				return NULL;
		}break;
		case _eCheckTesterSockNullPointer:
		{
			if (tester->m_pSock == NULL || tester->m_pSock == DEF_TESTER_HEAP_POINTER_DELETE || tester->m_pSock == DEF_TESTER_POINTER_DELETE)
				return NULL;
		}break;
		default:
		{
			if (tester == NULL || tester == DEF_TESTER_HEAP_POINTER_DELETE || tester == DEF_TESTER_POINTER_DELETE)
				return NULL;

			if (tester->m_pSock == NULL || tester->m_pSock == DEF_TESTER_HEAP_POINTER_DELETE || tester->m_pSock == DEF_TESTER_POINTER_DELETE)
				return NULL;
		}break;
	}

	return TRUE;
}



// int CTesterMgr::GetStationToIdentityNo(int nStationID)
// {
// 	for (int i = 0; i < (int)m_veStationToIdentityNo.size(); i++)
// 	{
// 		if (m_veStationToIdentityNo[i].nStationNo == nStationID)
// 		{
// 			return m_veStationToIdentityNo[i].nIdentityNo;
// 		}
// 	}
// }

