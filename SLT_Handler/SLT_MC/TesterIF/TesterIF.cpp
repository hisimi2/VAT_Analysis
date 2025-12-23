// TesterIF.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TesterIF.h"
#include <atlcoll.h>
#include <afxtempl.h>
#include <afxwin.h>
#include "LBaseSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTesterIF

IMPLEMENT_DYNAMIC(CTesterIF, CWnd)

CTesterIF::CTesterIF()
{
	m_nType = 0;
	m_ServerSock.m_pTesterIF = this;

	//m_pCSTesterIF = new CRITICAL_SECTION;
	//::InitializeCriticalSection( m_pCSTesterIF );

	//GetUnitData(); // TODO KSJ :: TESTER IF
	//SetUnitDataClear();
	//DeleteListAll(m_listUnitIdx);
}

CTesterIF::~CTesterIF()
{
	//DeleteListAll(m_listResult);
	//DeleteListAll(m_listUnitIdx);

	//if( m_pCSTesterIF != NULL )
	//{
	//	::DeleteCriticalSection( m_pCSTesterIF );
	//	delete m_pCSTesterIF; m_pCSTesterIF = NULL;
	//}

}

BEGIN_MESSAGE_MAP(CTesterIF, CWnd)
END_MESSAGE_MAP()

// CTesterIF 메시지 처리기입니다.

BOOL CTesterIF::ServerStart(int type)
{
	m_nType = type;

	//char szLogFileName[ 512 ] = { 0, };

	//sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, "TeserIF.log");
	//m_Logger.SetFileName(szLogFileName);

    if(type == TESTER_TYPE_TCP)
    {
        return ServerStartTcp();
    }
    else if(type == TESTER_TYPE_GPIB)
    {
        return ServerStartGpib();
    }
	 
	return FALSE;
}

BOOL CTesterIF::ServerStartTcp()
{
	if(m_ServerSock.Open("TEST_SOCK") != EWS_NO_ERR)
		return FALSE;

	if(m_ServerSock.Listen(NULL, 8008, SOMAXCONN) != EWS_NO_ERR)
		return FALSE;

	return TRUE;
}

BOOL CTesterIF::ServerStartGpib()
{
	return TRUE;
}

int CTesterIF::SendActionAll(int action)
{
	if(m_nType == TESTER_TYPE_TCP)
	{
		return SendActionTcp(action);
	}
	else if(m_nType == TESTER_TYPE_GPIB)
	{
		return SendActionGpib(action);
	}

	return FALSE;
}

BOOL CTesterIF::SendActionTcp(int action)
{
	m_TesterMgr.SendActionAll(action);

	return TRUE;
}

BOOL CTesterIF::SendActionGpib(int action)
{
	return TRUE;
}

BOOL CTesterIF::SendAction(int stationID, int action, WPARAM wParam/*=NULL*/, LPARAM lParam/*=NULL*/,int* pDirSiteOnOff,int nTesterMode)
{
	return m_TesterMgr.SendAction( stationID, action, wParam, lParam, pDirSiteOnOff, nTesterMode);
}

int CTesterIF::GetStatus(int stationID, int* pDirSiteOnOff, int nTesterOption,int* nTesterNum)
{
//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
	ASSERT(stationID > 0 && stationID < MAX_TESTER_SINGLE + 1);

	// -1: Error
	//  0: Init
	//	1: Idle
	//	2: Testing
	//	3: Error

	return m_TesterMgr.GetStatus( stationID, pDirSiteOnOff, nTesterOption, nTesterNum);
}

BOOL CTesterIF::IsConnected(int stationID, int nTesterMode/* = eTester_Single*/, BOOL *pSocketOnffInfo /*= NULL*/)
{
	//ASSERT( stationID > 0 && stationID < eMaxPressUnitCount+1 );
	return m_TesterMgr.IsConnected( stationID , nTesterMode, pSocketOnffInfo);
}

void CTesterIF::HeartBeatOverCutClient(int stationID, BOOL bUseIdentityNo)
{
	return m_TesterMgr.HeartBeatOverCutClient(stationID, bUseIdentityNo);
}

//void CTesterIF::AddResultData(RecvData& recvData)
//{
//	ResultData* rd = new ResultData;
//	rd->m_nStationID = recvData.m_nStationID;
//	rd->m_nTestResult = recvData.m_nTestResult;
//	_itoa_s(recvData.m_nErrorCode, rd->m_cErrorCode, 10);
//	
//	memcpy(rd->m_cErrorStr, recvData.m_cErrorStr, sizeof(BYTE)*ERR_STR);
//	memcpy(rd->m_cDutSn, recvData.m_cDutSn, sizeof(BYTE)*DUT_SN_STR);
//	
//	m_listResult.AddTail(rd);
//}

BOOL CTesterIF::GetResultDataFromTester(int stationID, ResultData* rd, int* pDirSiteOnOff, int nTesterOption)
{
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if (nTesterOption == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
	{
		CTester* pTester = m_TesterMgr.GetTester(stationID);
		CTesterStatus cTesterStatus;
		if (pTester != NULL) {

			cTesterStatus = pTester->GetTesterStatus();
			rd->m_nStationID = cTesterStatus.m_nStationID;

			for (int i = 0; i < STATION_MAX_PARA; i++) {
				rd->m_nTestResult[i] = cTesterStatus.m_nLastTestResult[i];
				rd->m_PhysicalNo[i]  = cTesterStatus.m_nRegiDutIdentityNo[i];
				sprintf_s(rd->m_cErrorCode[i], sizeof(rd->m_cErrorCode[i]), "%d", cTesterStatus.m_nLastReserved[i]);
				sprintf_s(rd->m_cErrorStr[i], sizeof(rd->m_cErrorStr[i]), "%s", cTesterStatus.m_sLastErrString[i].GetString());
				sprintf_s(rd->m_cDutSn[i], sizeof(rd->m_cDutSn[i]), "%s", cTesterStatus.m_sLastDutSN[i].GetString());
			}

			//rd->m_nTestResult = cTesterStatus.m_nLastTestResult;
			//sprintf_s(rd->m_cErrorCode, sizeof(rd->m_cErrorCode), "%d", cTesterStatus.m_nLastReserved);
			//sprintf_s(rd->m_cErrorStr, sizeof(rd->m_cErrorStr), "%s", cTesterStatus.m_sLastErrString);
			//sprintf_s(rd->m_cDutSn, sizeof(rd->m_cDutSn), "%s", cTesterStatus.m_sLastDutSN);
			return TRUE;
		}
	}
	else if(nTesterOption == eTester_Dual)
	{
		CTester* testerDual[eMaxTester];
		testerDual[eRearTester] = m_TesterMgr.GetTester(m_TesterMgr.DualTesterIndex(stationID, eRearTester)); // Rear Tester [8/30/2021 donghyun.shin]
		testerDual[eFrontTester] = m_TesterMgr.GetTester(m_TesterMgr.DualTesterIndex(stationID, eFrontTester));     // Front Tester
		CTesterStatus cTesterStatus[eMaxTester];
		int nTesterCnt = 0, nTesterErrCnt = 0;

		for (int t = 0; t < eMaxTester; t++)
		{
			if (testerDual[t] != NULL) {
				nTesterCnt++;
				cTesterStatus[t] = testerDual[t]->GetTesterStatus();

				if (pDirSiteOnOff[t] > 0)
					rd->m_nStationID = cTesterStatus[eRearTester].m_nStationID;
				else
					rd->m_nStationID = cTesterStatus[eFrontTester].m_nStationID / 2;

				if (t == eRearTester)
				{
					for (int i = 0; i < STATION_MAX_X_SITE; i++) {
						rd->m_nTestResult[i] = cTesterStatus[t].m_nLastTestResult[i];
						sprintf_s(rd->m_cErrorCode[i], sizeof(rd->m_cErrorCode[i]), "%s", cTesterStatus[t].m_nLastReserved[i]);
						sprintf_s(rd->m_cErrorStr[i], sizeof(rd->m_cErrorStr[i]), "%s", cTesterStatus[t].m_sLastErrString[i].GetString());
						sprintf_s(rd->m_cDutSn[i], sizeof(rd->m_cDutSn[i]), "%s", cTesterStatus[t].m_sLastDutSN[i].GetString());
					}
				}
				else {
					for (int i = 0; i < STATION_MAX_X_SITE; i++) {
						rd->m_nTestResult[i + STATION_MAX_X_SITE] = cTesterStatus[t].m_nLastTestResult[i];
						sprintf_s(rd->m_cErrorCode[i + STATION_MAX_X_SITE], sizeof(rd->m_cErrorCode[i + STATION_MAX_X_SITE]), "%s", cTesterStatus[t].m_nLastReserved[i]);
						sprintf_s(rd->m_cErrorStr[i + STATION_MAX_X_SITE], sizeof(rd->m_cErrorStr[i + STATION_MAX_X_SITE]), "%s", cTesterStatus[t].m_sLastErrString[i].GetString());
						sprintf_s(rd->m_cDutSn[i + STATION_MAX_X_SITE], sizeof(rd->m_cDutSn[i + STATION_MAX_X_SITE]), "%s", cTesterStatus[t].m_sLastDutSN[i].GetString());
					}
				}

				//rd->m_nTestResult = cTesterStatus.m_nLastTestResult;
				//sprintf_s(rd->m_cErrorCode, sizeof(rd->m_cErrorCode), "%d", cTesterStatus.m_nLastReserved);
				//sprintf_s(rd->m_cErrorStr, sizeof(rd->m_cErrorStr), "%s", cTesterStatus.m_sLastErrString);
				//sprintf_s(rd->m_cDutSn, sizeof(rd->m_cDutSn), "%s", cTesterStatus.m_sLastDutSN);
				if (nTesterCnt == eMaxTester)
					return TRUE;
			}
			else
			{
				nTesterErrCnt++;
			}
		}
		if (nTesterErrCnt > 0)
			return FALSE;
	}
	else// (nTesterOption == eTester_EachSocket)
	{
		CTester* testerOctal[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };

		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		int nSocketCnt = 0;

		for (nSocketCnt = 0; nSocketCnt < TestSite_Div_X * TestSite_Div_Y; nSocketCnt++)
		{
			int nPhysicalDutNo = m_TesterMgr.EachTesterIndex(stationID, nSocketCnt);

			testerOctal[nSocketCnt] = m_TesterMgr.GetTester(nPhysicalDutNo, eIdentityNo);     // Front Tester
		}
		
		CTesterStatus cTesterStatus;
		int nTesterCnt = 0, nTesterErrCnt = 0;

		for (int t = 0; t < TestSite_Div_X * TestSite_Div_Y; t++)
		{
			if (testerOctal[t] != NULL) {

				cTesterStatus = testerOctal[t]->GetTesterStatus();

				if (pDirSiteOnOff != NULL)
				{
					if (pDirSiteOnOff[t] == DEF_ON)
					{
						rd->m_nStationID = cTesterStatus.m_nStationID;
					}
				}
				rd->m_PhysicalNo[t] = cTesterStatus.m_nRegiDutIdentityNo[0];
				rd->m_nTestResult[t] = cTesterStatus.m_nLastTestResult[0];
				sprintf_s(rd->m_cErrorCode[t], sizeof(rd->m_cErrorCode[0]), "%d", cTesterStatus.m_nLastReserved[0]);
				sprintf_s(rd->m_cErrorStr[t], sizeof(rd->m_cErrorStr[0]), "%s", cTesterStatus.m_sLastErrString[0].GetString());
				sprintf_s(rd->m_cDutSn[t], sizeof(rd->m_cDutSn[0]), "%s", cTesterStatus.m_sLastDutSN[0].GetString());

			}
			else
			{
				if (pDirSiteOnOff[t] == DEF_ON)	// socket on상태인데 DUT client와 conntect 상태가 아니면 에러.
				{
					nTesterErrCnt++;
					rd->m_nTestResult[t] = -1;
					sprintf_s(rd->m_cErrorStr[t], sizeof(rd->m_cErrorStr[0]), _T(""));
				}
			}
		}

		if (nTesterErrCnt > 0)
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

//BOOL CTesterIF::GetResultData(int stationID, ResultData* rd)
//{
//	::EnterCriticalSection( m_pCSTesterIF );
//	POSITION pos = m_listResult.GetHeadPosition();
//
//	while(pos != NULL)
//	{
//		ResultData* tester = m_listResult.GetAt(pos);
//		if(tester->m_nStationID == stationID )
//		{
//			memcpy(rd, tester, sizeof(ResultDataInfo));
//
//			delete tester;
//			m_listResult.RemoveAt(pos);
//
//			::LeaveCriticalSection( m_pCSTesterIF );
//
//			return TRUE;
//		}
//		m_listResult.GetNext(pos);
//	}
//	::LeaveCriticalSection( m_pCSTesterIF );
//
//	return FALSE;
//}

//void CTesterIF::DeleteListAll(CList<ResultData*> &list)
//{
//	POSITION pos = list.GetHeadPosition();
//
//	while(pos != NULL)
//	{
//		ResultData* tester = list.GetNext(pos);
//		delete tester;
//	}
//
//	list.RemoveAll();
//}
//
//void CTesterIF::DeleteListAll(CList<UnitIdx*> &list)
//{
//	POSITION pos = list.GetHeadPosition();
//
//	while(pos != NULL)
//	{
//		UnitIdx* tester = list.GetNext(pos);
//		delete tester;
//	}
//
//	list.RemoveAll();
//}

int CTesterIF::GetStationID(int stationID )
{
	CTester* pTester = m_TesterMgr.GetTester(stationID);
	CTesterStatus cTesterStatus;

	BOOL bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + stationID - 1);
	if (pTester != NULL && bRegister == TRUE) {
		cTesterStatus = pTester->GetTesterStatus();
		return cTesterStatus.m_nStationID;
	}

	return -1;
//	int testerID = 0;
//
//	if(stationID > 0 && stationID <= UNIT_MAX)
//	{
//		POSITION pos = m_listUnitIdx.GetHeadPosition();
//
//		while(pos != NULL)
//		{
//			UnitIdx* tester = m_listUnitIdx.GetNext(pos);
//// 			if(tester->m_nUnitIdx == unitIdx){
//// 				return tester->m_nStationID;
////             }
//			if(tester->m_nStationID == stationID){
//				return tester->m_nStationID;
//			}
//		}
//	}
//
//	return RET_ERROR;
}

// tester Id로 Unit Index 역추적 함.
//int CTesterIF::GetUnitIndex(int stationID )
//{
//	ASSERT( stationID > 0 && stationID <= UNIT_MAX );
//    if( stationID > 0 && stationID <= UNIT_MAX )
//    {
////         POSITION pos = m_listUnitIdx.GetHeadPosition();
//// 
////         while(pos != NULL)
////         {
////             UnitIdx* tester = m_listUnitIdx.GetNext(pos);
////             if(tester->m_nStationID == stationID )
////                 return (tester->m_nStationID -1 );
////         }
//		CTester* pTester = m_TesterMgr.GetTester( stationID );
//		if( pTester != NULL ){
//			return (pTester->GetStationID() -1 );
//		}
//    }
//
//    return RET_ERROR;
//}

//void CTesterIF::GetUnitData()
//{
//	TCHAR idx[256]= {0,};
//	TCHAR buf[256]= {0,};
//
//	for(int i=0; i<UNIT_MAX; i++)
//	{
//		sprintf_s(idx, "%d", i);
//		GetPrivateProfileString("UNIT_INDEX", idx, "0", buf, 256, UNIT_INDEX_PATH);
//
//		UnitIdx* ui = new UnitIdx;
//		ui->m_nUnitIdx = i;
//		ui->m_nStationID = _tstoi(buf);
//		m_listUnitIdx.AddTail(ui);
//	}
//}

//void CTesterIF::SetUnitData(int unitIdx, int stationID )
//{
//	TCHAR idx[256]= {0,};
//	TCHAR buf[256]= {0,};
//
//	sprintf_s(idx, "%d", unitIdx);
//	sprintf_s(buf, "%d", stationID );
//	WritePrivateProfileString("UNIT_INDEX", idx, buf, UNIT_INDEX_PATH);
//}

//void CTesterIF::SetUnitDataClear()
//{
//    TCHAR idx[256]= {0,};
//    TCHAR buf[256]= {0,};
//
//    for(int i=0; i<UNIT_MAX; i++){
//        sprintf_s(idx, "%d", i);
//        sprintf_s(buf, "%d", -1);
//        WritePrivateProfileString("UNIT_INDEX", idx, buf, UNIT_INDEX_PATH);
//    }
//}

CTesterStatus CTesterIF::GetTesterStatus( int stationID, int nMode)
{
	CTesterStatus cTesterStatus;

	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if (nMode == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
	{
		CTester* pTester = m_TesterMgr.GetTester(stationID,eIdentityNo);
		BOOL bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + stationID - 1);

		if (pTester != NULL && bRegister == TRUE) {
			cTesterStatus = pTester->GetTesterStatus();
		}
		
	}
	else if(nMode == eTester_Dual)
	{
		CTester* testerDual[eMaxTester];
		testerDual[eRearTester] = m_TesterMgr.GetTester(m_TesterMgr.DualTesterIndex(stationID, eRearTester)); // Rear Tester [8/30/2021 donghyun.shin]
		testerDual[eFrontTester] = m_TesterMgr.GetTester(m_TesterMgr.DualTesterIndex(stationID, eFrontTester));     // Front Tester
	

		CTesterStatus cTesterStatusDual[eMaxTester];
		BOOL bRegister[eMaxTester];

		bRegister[eRearTester] = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + m_TesterMgr.DualTesterIndex(stationID, eRearTester) - 1);
		bRegister[eFrontTester] = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + m_TesterMgr.DualTesterIndex(stationID, eFrontTester) - 1);

		if (testerDual[eRearTester] != NULL && bRegister[eRearTester] == TRUE) 
			cTesterStatus = testerDual[eRearTester]->GetTesterStatus();

		for (int t = 0; t < eMaxTester; t++)
		{		
			if (testerDual[t] != NULL && bRegister[t] == TRUE) {
				
				cTesterStatusDual[t] = testerDual[t]->GetTesterStatus();
				int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
				int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

				for (int nX = 0; nX < TestSite_Div_X; nX++) {
					if (t <= 0) {
						cTesterStatus.m_sSocketID[nX] = cTesterStatusDual[t].m_sSocketID[nX];
						cTesterStatus.m_sSocketID_Orginal[nX] = cTesterStatusDual[t].m_sSocketID_Orginal[nX];
					}
					else
					{
						cTesterStatus.m_sSocketID[nX + TestSite_Div_X] = cTesterStatusDual[t].m_sSocketID[nX];
						cTesterStatus.m_sSocketID_Orginal[nX + TestSite_Div_X] = cTesterStatusDual[t].m_sSocketID_Orginal[nX];
					}
				}

			}
		}		
	}
	else	// 각각 소켓별 Data 수신 하는 방식.
	{
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		CTester* testerEachSocket[eEachTesterSocket::eMaxTesterForYanmade] = { NULL, };

		int nSocketCnt = 0;

		CTesterStatus cTesterStatusEach[eEachTesterSocket::eMaxTesterForYanmade];
		BOOL bArrRegister[eEachTesterSocket::eMaxTesterForYanmade] = { FALSE, };

		for (nSocketCnt = 0; nSocketCnt < TestSite_Div_X * TestSite_Div_Y; nSocketCnt++)
		{
			int nPhysicalCommSocketNo = m_TesterMgr.EachTesterIndex(stationID, nSocketCnt);
			testerEachSocket[nSocketCnt] = m_TesterMgr.GetTester(nPhysicalCommSocketNo, TRUE);
			bArrRegister[eEachTesterSocket::eSocket1 + nSocketCnt] = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + (nPhysicalCommSocketNo-1));
		}
	
		for (int t = 0; t < TestSite_Div_X * TestSite_Div_Y; t++)
		{
			if (testerEachSocket[eEachTesterSocket::eSocket1 + t] != NULL && bArrRegister[eEachTesterSocket::eSocket1 + t] == TRUE)
			{
				cTesterStatus = testerEachSocket[eEachTesterSocket::eSocket1 + t]->GetTesterStatus();
				break;
			}
		}


		for (int t = 0; t < TestSite_Div_X * TestSite_Div_Y; t++)
		{
			cTesterStatus.m_nRegiDutIdentityNo[t] = -1;
			cTesterStatus.m_nLastTestResult[t]	  = 0;
			cTesterStatus.m_sLastErrString[t].Empty();
			cTesterStatus.m_sLastDutSN[t].Empty();
			cTesterStatus.m_nLastReserved[t] = 0;
		}

		for (int t = 0; t < TestSite_Div_X * TestSite_Div_Y; t++)
		{
			bArrRegister[t] = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + m_TesterMgr.EachTesterIndex(stationID, t) - 1);
			
			if (testerEachSocket[t] != NULL && bArrRegister[t] == TRUE) {

				cTesterStatusEach[t] = testerEachSocket[t]->GetTesterStatus();
				cTesterStatus.m_nRegiDutIdentityNo[t] = cTesterStatusEach[t].m_nRegiDutIdentityNo[0];
				cTesterStatus.m_nLastTestResult[t]	  = cTesterStatusEach[t].m_nLastTestResult[0];
				cTesterStatus.m_sLastErrString[t]	  = cTesterStatusEach[t].m_sLastErrString[0];
				cTesterStatus.m_sLastDutSN[t]		  = cTesterStatusEach[t].m_sLastDutSN[0];
				cTesterStatus.m_nYanMadeStatus[t] = cTesterStatusEach[t].m_nStatus;
				cTesterStatus.m_nLastReserved[t] = cTesterStatusEach[t].m_nLastReserved[0];
				//cTesterStatus.m_sSocketID[t] = cTesterStatusEach[t].m_sSocketID[0];
				//cTesterStatus.m_sSocketID_Orginal[t] = cTesterStatusEach[t].m_sSocketID_Orginal[0];
			}
		}
	}

	return cTesterStatus;
}

CTime CTesterIF::GetTesterTimeStamp(int stationID)
{
	CTester* pTester = m_TesterMgr.GetTester( stationID );
	if( pTester != NULL ){
		CTesterStatus cTesterStatus;
		cTesterStatus = pTester->GetTesterStatus();

		return cTesterStatus.m_cChkTimeStamp;
	}

	return -1;
}

void CTesterIF::MakeLog(LPCTSTR fmt, ...)
{
   TCHAR tmpMsg[4096] = {0,};
   if(fmt)
   {
       va_list argList;
       va_start(argList, fmt);
       _vstprintf_s(tmpMsg, fmt, argList);
       va_end(argList);
   }
   //TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_HeartBeat), Debug, "", __LINE__, NULL, tmpMsg);
}

void CTesterIF::MakeHexLog( BYTE* data, int len, LPCTSTR header_fmt, ... )
{
#ifdef _DEBUG

	CString strLog = "";
	TCHAR headMsg[ 4096 ] = { 0, };
	
	if( header_fmt )
	{
		va_list argList;
		va_start( argList, header_fmt );
		_vstprintf_s( headMsg, header_fmt, argList );
		va_end( argList );
	}

	char szHexLog[ 4096 ] = { 0, };
	for( int i = 0; i<len; i++ ) {
		char szTemp[ 16 ] = { 0, };
		sprintf_s( szTemp, sizeof( szTemp ), "%02X ", data[ i ] );
		strcat_s( szHexLog, szTemp );
	}

	strLog.Format( "%s HEX : %s", headMsg, szHexLog );
	
	TWLOG.MakeLog( static_cast<int>( eLogID_TesterIF ), Debug, "", __LINE__, NULL, strLog );

// 	// Send TCP Log
// 	TWLOG.MakeLog( static_cast<int>( eLogID_TCPIP_HeartBeat ), Debug, "", __LINE__, NULL, strLog );
#endif
}

//void CTesterIF::GetTesterInfoAll()
//{
//	DeleteListAll(m_listUnitIdx);
//	m_TesterMgr.GetTesterInfoAll();
//}