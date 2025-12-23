#include "StdAfx.h"
#include "Tester.h"
#include <memory.h>
#include <afxstr.h>
#include "TesterIF.h"
#include "LBaseSock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================
//  ::CTesterStatus
//===================================================================
CTesterStatus::CTesterStatus()
{
	m_nTest_Mode = 0;
	m_nStationID = 0;
	m_nRegistration = 0;
	m_nStatus = 0;
	m_nReserved2 = -1;
	m_nReserved3 = 0;
	m_bHeartBit = FALSE;
	m_bBeforeHeatBit = FALSE;

	for(int i=0; i<STATION_MAX_PARA; i++){
		m_nLastTestResult[i] = -1;
		m_nLastReserved[i] = 0;
		m_nYanMadeStatus[i] = 0;
	}

	m_cChkTimeStamp = 0;
	m_nTesterInformation = _eSetTesterInformation::_TesterInformationIdle;
	ZeroMemory(m_nRegiDutIdentityNo, sizeof(m_nRegiDutIdentityNo));
	
}

CTesterStatus::CTesterStatus(const CTesterStatus& ref)
{
	m_nStationID			= ref.m_nStationID;			
	m_nRegistration		= ref.m_nRegistration;	
	memcpy(m_nRegiDutIdentityNo, ref.m_nRegiDutIdentityNo, sizeof(m_nRegiDutIdentityNo));
	m_nStatus			= ref.m_nStatus;				
	m_nReserved2			= ref.m_nReserved2;			
	m_nReserved3			= ref.m_nReserved3;		
	m_sTimeStamp		= ref.m_sTimeStamp.GetString();		
	for(int i=0; i<STATION_MAX_PARA; i++){
		m_nLastTestResult[i] = ref.m_nLastTestResult[i];
		m_nLastReserved[i]	 = ref.m_nLastReserved[i];		
		m_sLastErrString[i]	 = ref.m_sLastErrString[i].GetString();	
		m_sLastDutSN[i]      = ref.m_sLastDutSN[i].GetString();

		m_sSocketID[ i ] = ref.m_sSocketID[ i ].GetString();
		m_sSocketID_Orginal[ i ] = ref.m_sSocketID_Orginal[ i ].GetString();
		m_nYanMadeStatus[i] = ref.m_nYanMadeStatus[i];
	
	}
	m_bHeartBit			= ref.m_bHeartBit;
	m_bBeforeHeatBit    = ref.m_bBeforeHeatBit;
	m_cChkTimeStamp = ref.m_cChkTimeStamp;

	m_sTesterID = ref.m_sTesterID;
	m_sTesterID_Orginal = ref.m_sTesterID_Orginal;
	m_sHW_version = ref.m_sHW_version;
	m_sPPO_FW_version = ref.m_sPPO_FW_version;
	m_sHID_FW_version = ref.m_sHID_FW_version;
	m_sSW_Version = ref.m_sSW_Version;
	m_sIP = ref.m_sIP;
	m_nTesterInformation = ref.m_nTesterInformation;
	m_sTest_Script_Info = ref.m_sTest_Script_Info;
	m_sTest_Script_Apl = ref.m_sTest_Script_Apl;
	m_nTest_Mode = ref.m_nTest_Mode;

	m_sTestScriptInfo = ref.m_sTestScriptInfo;
	m_sTestSWVersion = ref.m_sTestSWVersion;
	m_sTestRetest = ref.m_sTestRetest;
	m_sTestHandlerID = ref.m_sTestHandlerID;
	m_sTestDevice = ref.m_sTestDevice;

	int nSearch = 0;
	for (nSearch = 0; nSearch < STATION_MAX_PARA; nSearch++)
	{
		SetVersionInfo(ref.m_sInfo[nSearch], nSearch);
	}
}

CTesterStatus::~CTesterStatus()
{
}


CTesterStatus& CTesterStatus::operator=(const CTesterStatus& ref)
{
	if( this == &ref ){ return *this; }
	m_nStationID			= ref.m_nStationID;			
	m_nRegistration		= ref.m_nRegistration;		
	memcpy(m_nRegiDutIdentityNo, ref.m_nRegiDutIdentityNo, sizeof(m_nRegiDutIdentityNo));
	m_nStatus			= ref.m_nStatus;				
	m_nReserved2			= ref.m_nReserved2;			
	m_nReserved3			= ref.m_nReserved3;		
	m_sTimeStamp		= ref.m_sTimeStamp.GetString();		
	for(int i =0; i<STATION_MAX_PARA; i++){
		m_nLastTestResult[i]= ref.m_nLastTestResult[i];
		m_nLastReserved[i]	= ref.m_nLastReserved[i];		
		m_sLastErrString[i]	= ref.m_sLastErrString[i].GetString();	
		m_sLastDutSN[i]     = ref.m_sLastDutSN[i].GetString();


		m_sSocketID[ i ] = ref.m_sSocketID[ i ].GetString();
		m_sSocketID_Orginal[ i ] = ref.m_sSocketID_Orginal[ i ].GetString();
		m_nYanMadeStatus[i] = ref.m_nYanMadeStatus[i];
	}
	m_bHeartBit			= ref.m_bHeartBit;
	m_bBeforeHeatBit    = ref.m_bBeforeHeatBit;
	m_cChkTimeStamp = ref.m_cChkTimeStamp;

	m_sTesterID = ref.m_sTesterID;
	m_sHW_version = ref.m_sHW_version;
	m_sPPO_FW_version = ref.m_sPPO_FW_version;
	m_sHID_FW_version = ref.m_sHID_FW_version;
	m_sSW_Version = ref.m_sSW_Version;
	m_sIP = ref.m_sIP;
	m_sTesterID_Orginal = ref.m_sTesterID_Orginal;
	m_nTesterInformation = ref.m_nTesterInformation;
	m_sTest_Script_Info = ref.m_sTest_Script_Info;
	m_sTest_Script_Apl = ref.m_sTest_Script_Apl;

	m_nTest_Mode = ref.m_nTest_Mode;
	m_sTestScriptInfo = ref.m_sTestScriptInfo;
	m_sTestSWVersion = ref.m_sTestSWVersion;
	m_sTestRetest = ref.m_sTestRetest;
	m_sTestHandlerID = ref.m_sTestHandlerID;
	m_sTestDevice = ref.m_sTestDevice;

	int nSearch = 0;

	for (nSearch = 0; nSearch < STATION_MAX_PARA; nSearch++)
	{
		SetVersionInfo(ref.m_sInfo[nSearch], nSearch);
	}
		
	return *this;
}

void CTesterStatus::SetVersionInfo(CString sInfo, int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < STATION_MAX_PARA);

	m_sInfo[nIdx] = sInfo;
}

CString CTesterStatus::GetStatus()
{
	CString sStatus;
	switch(m_nStatus) 
	{
	case STATUS_INIT:		sStatus = "INIT";		break;
	case STATUS_IDLE:		sStatus = "ILDE";		break;
	case STATUS_TESTING:	sStatus = "TESTING";	break;
	case STATUS_ERROR:		sStatus = "ERROR";		break;
	case STATUS_UNREGISTER:	sStatus = "UNREGISTER"; break;
	}
	return sStatus;
};




// "version1:0.0.1 version2:0.0.2 version3:0.0.3" 이런식으로 되어 있음. 첫번째 구분자 ";", 두번째 구분자 ":"
void CTesterStatus::PasingMsgType(CString sInfo, std::vector<std::pair<std::string,std::string>>& data_list, int MsgType )
{
	int nLen = sInfo.GetLength()+1;
	char* szBuff = new char[nLen];
	ZeroMemory(szBuff, nLen);

	sprintf_s(szBuff, nLen, "%s", sInfo.GetString() );

	CParamList PrmList;
	PrmList.m_fnSetDelimiter( ";", 1 );
	PrmList.m_fnSetDelimitedText(szBuff, strlen(szBuff));

	int nCount = PrmList.m_fnGetCount();
//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{
		for (int i = 0; i < nCount; ++i) {

			char szName_Version[256] = { 0, };
			PrmList.m_fnGetItemString(i, szName_Version);
			std::pair<std::string, std::string> p;

			if (MsgType == DEF_MSG_TYPE_REGIST || MsgType == DEF_MSG_TYPE_TEST_SCRIPT)
			{
				p.first = szName_Version;
			}
			else if (MsgType == DEF_MSG_TYPE_INFORMATION)
			{
				p = PrmList.m_fnGetPasedItemString(szName_Version);
			}

			data_list.push_back(p);
		}
	}
// #else
	else
	{
		for(int i=0; i<nCount; ++i){
		
			if (MsgType == DEF_MSG_TYPE_TEST_MODE)
			{
				char szName_Version[256] = { 0, };
				PrmList.m_fnGetItemString(i, szName_Version);
				std::pair<std::string, std::string> p;
				p.first = szName_Version;
				data_list.push_back(p);
			}
			else {
				char szName_Version[256] = { 0, };
				PrmList.m_fnGetItemString(i, szName_Version);

				CParamList PrmList2;
				PrmList2.m_fnSetDelimiter(":", 1);
				PrmList2.m_fnSetDelimitedText(szName_Version, strlen(szName_Version));

				std::pair<std::string, std::string> p;
				char szName[128] = { 0, };
				PrmList2.m_fnGetItemString(0, szName);
				p.first = szName;

				char szData[128] = { 0, };
				PrmList2.m_fnGetItemString(1, szData);
				p.second = szData;

				data_list.push_back(p);
			}
		}
		
// 		if (nCount < 4) // 4보다 적은 경우 실패  TWSL421 통신 프로토콜 확인 하여 수정 필요.
// 		{
// 			std::pair<std::string, std::string> p;
// 			p.first = "";
// 			p.second = "";
// 		
// 			for (int i = 0; i < 4 - nCount; ++i) {
// 				data_list.push_back(p);
// 			}
// 		}
	}
// #endif



	delete [] szBuff;
	szBuff = NULL;
}

// "version1:0.0.1 version2:0.0.2 version3:0.0.3" 이런식으로 되어 있음. 첫번째 구분자 ";", 두번째 구분자 ":"
void CTesterStatus::PasingInformation(CString sInfo, std::vector<std::pair<std::string, std::string>>& data_list)
{
	int nLen = sInfo.GetLength() + 1;
	char* szBuff = new char[nLen];
	ZeroMemory(szBuff, nLen);

	sprintf_s(szBuff, nLen, "%s", sInfo.GetString());

	CParamList PrmList;
	PrmList.m_fnSetDelimiter(";", 1);
	PrmList.m_fnSetDelimitedText(szBuff, strlen(szBuff));

	int nCount = PrmList.m_fnGetCount();

	for (int i = 0; i < nCount; ++i) {
		char szName_Version[256] = { 0, };
		PrmList.m_fnGetItemString(i, szName_Version);

		CParamList PrmList2;
		PrmList2.m_fnSetDelimiter(":", 1);
		PrmList2.m_fnSetDelimitedText(szName_Version, strlen(szName_Version));

		std::pair<std::string, std::string> p;
		char szName[128] = { 0, };
		PrmList2.m_fnGetItemString(0, szName);
		p.first = szName;

		char szData[128] = { 0, };
		PrmList2.m_fnGetItemString(1, szData);
		p.second = szData;

		data_list.push_back(p);
	}

	if (nCount < 4)
	{
		std::pair<std::string, std::string> p;
		p.first = "";
		p.second = "";

		for (int i = 0; i < 4 - nCount; ++i) {
			data_list.push_back(p);
		}
	}

	delete[] szBuff;
	szBuff = NULL;
}



//===================================================================
//  ::CTester
//===================================================================
CTester::CTester(void)
{
	m_pSock = NULL;
	memset(&m_SendData, 0x00, sizeof(SendData));
}


CTester::~CTester(void)
{
}


BOOL CTester::SendAction(int action, WPARAM wParam/*=NULL*/, LPARAM lParam/*=NULL*/)
{
	//BYTE sendData[MAX_MSG_ID + SEND_MSG_DATA_MAX + 1] = { 0, };
	m_SendData.m_nAction = action;
	int nStation_Max_Para = 0;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	if (nTesterOption == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
		nStation_Max_Para = STATION_MAX_PARA/2;
	else if (nTesterOption == eTester_Dual)
		nStation_Max_Para = STATION_MAX_PARA / 2;
	else
		nStation_Max_Para = 1;

//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET

	if (nTesterOption == eTester_EachSocket)
	{
		BYTE sendData[MAX_MSG_ID + SEND_MSG_DATA_EACH_MAX + 1] = { 0, };

		switch (action) {
			case ACTION_START:
			{
				for (int i = 0; i < STATION_MAX_PARA; i++) {
					m_TesterStatus.m_nLastTestResult[i] = -1;
				}

				ASSERT(wParam != NULL);
				ST_TESTER_ACTION_PARAM* ap = (ST_TESTER_ACTION_PARAM*)wParam;

				char szHostName[SEND_HOST_NAME] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szHostName, sizeof(szHostName));

				// processID 받아오는 곳 확인 필요.
				char szProcessID[32] = { 0, };
				_tcscpy(szProcessID, _T("ProcessID"));
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szProcessID, sizeof(szProcessID));
				// operatorID 받아오는 곳 확인 필요.
				char szOperatorID[32] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_OperatorID, szOperatorID, sizeof(szOperatorID));

				if (nProjectOption == DEF_PROJECT_CARPENTERS)
				{
					char szTP_Rev[32] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, sizeof(szTP_Rev));
					memset(m_SendData.m_cTpRev[0], 0, sizeof(m_SendData.m_cTpRev[0]));
					sprintf(m_SendData.m_cTpRev[0], "%s;", szTP_Rev);
				}

				memset(m_SendData.m_cSerialNum[0], 0, sizeof(m_SendData.m_cSerialNum[0]));
				if (ap[0].m_bSiteOn == true)
				{
					sprintf(m_SendData.m_cSerialNum[0], "%s;%s;%s;%s;%s;",
						ap[0].m_c2DID, szHostName, ap[0].m_cLotNum, szProcessID, szOperatorID);
				}

				memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));

			}break;
			case ACTION_SET_TEST_MODE:
			{
				int mode = lParam;
				sprintf(m_SendData.m_cMode, "mode:%d", mode);

				memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
				for (int i = 0; i < nStation_Max_Para; i++)
					memset(m_SendData.m_cSerialNum[i], 0, sizeof(m_SendData.m_cSerialNum[i]));


			}break;
			case ACTION_TEST_SCRIPT_INFO:
			{
				char szTP_Rev[32] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, sizeof(szTP_Rev));
				memset(m_SendData.m_cTpRev[0], 0, sizeof(m_SendData.m_cTpRev[0]));

				sprintf(m_SendData.m_cTpRev[0], "%s;", szTP_Rev);
				
				memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
			}break;
			default:
			{

				if (action == ACTION_STATUS || action == ACTION_RESET)
				{
					SetStatus(STATUS_INIT);	// 테스터한테 수신 받기전 해당 falg init으로 바꿔 놓는다.
				}

				memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
				for (int i = 0; i < nStation_Max_Para; i++)
					memset(m_SendData.m_cSerialNum[i], 0, sizeof(m_SendData.m_cSerialNum[i]));
				// 				g_AppleTestIF.MakeLog( "%s action:%d is invaild value", __FUNCSIG__, action );
				// 				return FALSE;
			}break;
		}

		MakeSendData(sendData, action);


		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] TesterID:%d, Type:%s, Action:%d, Mode:%s,%s, ",
			m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);

		//g_AppleTestIF.MakeLog("[SEND H->T] TesterID:%d, Type:%d, Action:%d, Mode:%s,%s, ",
		//	m_SendData.m_DataHeader.m_bit.m_nStationID, m_SendData.m_DataHeader.m_bit.m_nType, m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
		if (action == ACTION_START || action == ACTION_TEST_SCRIPT_INFO) {
			//for (int i = 0; i < STATION_MAX_PARA; i++) {
			//g_AppleTestIF.MakeLog("[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[i]);
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[0]);
			//}
		}

		int nSendMsgLen = MAX_MSG_ID + SEND_MSG_DATA_MAX;
		g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "[SND ACTION H->T] TesterID:%d, Type:%s, SendLen:%d", m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), nSendMsgLen);

		if (TesterNullPointerCheck(m_pSock) != NULL) {
			int nResult = m_pSock->Send(sendData, MAX_MSG_ID + SEND_MSG_DATA_MAX + 1);
		}
		else {
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] Send Fail!!!!!!!!!!!!     TesterID:%d, Type:%s, Action:%d, Mode:%s,%s, ",
			m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
		}
			
	}
	else //Single, Dual
	{ 
		BYTE sendData[MAX_MSG_ID + SEND_MSG_DATA_MAX + 1] = { 0, };

		switch( action ) {
			case ACTION_START:
				{
					for( int i = 0; i<STATION_MAX_PARA; i++ ) {
						m_TesterStatus.m_nLastTestResult[ i ] = -1;
					}

					ASSERT( wParam != NULL );
					ST_TESTER_ACTION_PARAM* ap = ( ST_TESTER_ACTION_PARAM* )wParam;

					char szHostName[SEND_HOST_NAME] = { 0, };
					
					if (nProjectOption == DEF_PROJECT_WATT) {
						g_DMCont.m_dmHandlerOpt.GS(SDM11_HANDLER, szHostName, sizeof(szHostName));
					}
					else {
						g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szHostName, sizeof(szHostName));
					}
					

					for( int i = 0; i<nStation_Max_Para; i++ ) {
						memset( m_SendData.m_cSingleSerialNum[ i ], 0, sizeof(m_SendData.m_cSingleSerialNum[ i ]));
						if( ap[ i ].m_bSiteOn == true )
						{	
// 							if (nProjectOption == DEF_PROJECT_WATT)
// 							{							
// 								sprintf(m_SendData.m_cSingleSerialNum[i], "%s,%s,%s,%s,%s,%s,%s",
// 									ap[i].m_cLotNum, ap[i].m_cTesterID, ap[i].m_cSocketID, ap[i].m_c2DID, szHostName, ap[i].m_cRTCode, ap[i].m_cFTRT);
// 							}
// 							else 
//							{
								sprintf(m_SendData.m_cSingleSerialNum[i], "%s,%s,%s,%s,%s",
									ap[i].m_cLotNum, ap[i].m_cTesterID, ap[i].m_cSocketID, ap[i].m_c2DID, szHostName);
//							}												
						}
					}

					CString strFullsites = "";
					for( int i = 0; i < nStation_Max_Para; i++ ) {
						strFullsites += ap[ i ].m_bSiteOn ? "1" : "0";
						if( i < nStation_Max_Para -1 )
							strFullsites += ",";
					}

					memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
					sprintf( m_SendData.m_cFullsites, "Fullsites:%s", strFullsites.GetString() );

				}break;
			case ACTION_SET_TEST_MODE:
				{
					int mode = lParam;
					sprintf( m_SendData.m_cMode, "mode:%d", mode );

					memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
					for( int i = 0; i<nStation_Max_Para; i++ )
						memset( m_SendData.m_cSingleSerialNum[ i ], 0, sizeof(m_SendData.m_cSingleSerialNum[ i ]));
					

			}break;
			default :
			{

				if (action == ACTION_STATUS)
				{
					SetStatus(STATUS_INIT);
				}

				memset(m_SendData.m_cFullsites, 0, sizeof(m_SendData.m_cFullsites));
				for( int i = 0; i<nStation_Max_Para; i++ )
					memset( m_SendData.m_cSingleSerialNum[ i ], 0, sizeof(m_SendData.m_cSingleSerialNum[ i ]));
// 				g_AppleTestIF.MakeLog( "%s action:%d is invaild value", __FUNCSIG__, action );
// 				return FALSE;
			}break;
		}

		MakeSendData(sendData, action);


		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] TesterID:%d, Type:%s, Action:%d, Mode:%s,%s, ",
			m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);

		//g_AppleTestIF.MakeLog("[SEND H->T] TesterID:%d, Type:%d, Action:%d, Mode:%s,%s, ",
		//	m_SendData.m_DataHeader.m_bit.m_nStationID, m_SendData.m_DataHeader.m_bit.m_nType, m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
		if (action == ACTION_START) {
			//for (int i = 0; i < STATION_MAX_PARA; i++) {
			//g_AppleTestIF.MakeLog("[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[i]);
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[0]);
			//}
		}

		int nSendMsgLen = MAX_MSG_ID + SEND_MSG_DATA_MAX;
		g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "[SND ACTION H->T] TesterID:%d, Type:%s, SendLen:%d", m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), nSendMsgLen); 

		if (TesterNullPointerCheck(m_pSock) != NULL) {
			int nResult = m_pSock->Send(sendData, MAX_MSG_ID + SEND_MSG_DATA_MAX + 1);
		}			
		else {
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] Send Fail!!!!!!!!!!!!     TesterID:%d, Type:%s, Action:%d, Mode:%s,%s, ",
			m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
		}
	}
//#endif

// 	MakeSendData(sendData, action);
// 
// 	
// 	TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] TesterID:%d, Type:%s, Action:%d, Mode:%s,%s, ",
// 		m_SendData.m_DataHeader.m_bit.m_nStationID, GetStringActionType(m_SendData.m_nAction), m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
// 
// 	//g_AppleTestIF.MakeLog("[SEND H->T] TesterID:%d, Type:%d, Action:%d, Mode:%s,%s, ",
// 	//	m_SendData.m_DataHeader.m_bit.m_nStationID, m_SendData.m_DataHeader.m_bit.m_nType, m_SendData.m_nAction, m_SendData.m_cMode, m_SendData.m_cFullsites);
// 	if (action == ACTION_START) {
// 		//for (int i = 0; i < STATION_MAX_PARA; i++) {
// 			//g_AppleTestIF.MakeLog("[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[i]);
// 			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[SEND H->T] Serial:%s", m_SendData.m_cSerialNum[0]);
// 		//}
// 	}
// 
// 	int nSendMsgLen = MAX_MSG_ID + SEND_MSG_DATA_MAX;
// 	g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "[SND ACTION H->T] TesterID:%d, SendLen:%d", m_SendData.m_DataHeader.m_bit.m_nStationID, nSendMsgLen);
// 
// 
// 	int nResult = m_pSock->Send(sendData, MAX_MSG_ID + SEND_MSG_DATA_MAX + 1);
	

	return TRUE;
}

void CTester::MakeSendData(BYTE* data, int action)
{
	// Head
	m_SendData.m_DataHeader.m_bit.m_nReserved = 0;
	m_SendData.m_DataHeader.m_bit.m_nType = 2;
	m_SendData.m_DataHeader.m_bit.m_nStationID = GetStationID();
	m_SendData.m_DataHeader.m_bit.m_nSc = 0;
	m_SendData.m_DataHeader.m_bit.m_nSm = 0;
	
	unsigned int head = SWAP32(m_SendData.m_DataHeader.m_nData);
	memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

	// Len
	m_SendData.m_nDataLen = SEND_MSG_DATA_MAX + 1; // CRC size 넣는다.
	unsigned short len = SWAP16(m_SendData.m_nDataLen);
	memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

	// Action
	data[MAX_MSG_ID+2] = (BYTE)action;


//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if (nTesterOption == eTester_EachSocket)
	{
		if (nProjectOption == DEF_PROJECT_CARPENTERS)
		{
			if (action == ACTION_TEST_SCRIPT_INFO || action == ACTION_START)
			{
				memcpy(&data[MAX_MSG_ID + 3 + MODE_STR_MAX], m_SendData.m_cTpRev[0], sizeof(m_SendData.m_cTpRev[0]));					
			}
		}
		if(action != ACTION_TEST_SCRIPT_INFO)
			memcpy(&data[MAX_MSG_ID + 3 + MODE_STR_MAX + FULLSITES_STR_MAX], m_SendData.m_cSerialNum[0], sizeof(m_SendData.m_cSerialNum[0]));

		data[MAX_MSG_ID + SEND_MSG_DATA_MAX] = SEND_DATA_CRC;//m_SendData.m_nCRC;
	}
	// CRC
	else
	{
		// Mode
		memcpy(&data[MAX_MSG_ID + 3], m_SendData.m_cMode, sizeof(m_SendData.m_cMode));
		// Fullsites
		memcpy(&data[MAX_MSG_ID + 3 + MODE_STR_MAX], m_SendData.m_cFullsites, sizeof(m_SendData.m_cFullsites));

		// Serial 
		// Inline 4Para 대응
		for (int i = 0; i < STATION_MAX_PARA; i++) {
			memcpy(&data[MAX_MSG_ID + 3 + MODE_STR_MAX + FULLSITES_STR_MAX + (SERIAL_SINGLE_STR_MAX*i)], m_SendData.m_cSingleSerialNum[i], sizeof(m_SendData.m_cSingleSerialNum[i]));
		}

		// CRC
		data[MAX_MSG_ID + 3 + MODE_STR_MAX + FULLSITES_STR_MAX + (SERIAL_SINGLE_STR_MAX*STATION_MAX_PARA)] = SEND_DATA_CRC;//m_SendData.m_nCRC;
	}
//#endif




}

void CTester::SetStatus( int a_nStatus )
{
	m_TesterStatus.m_nStatus = a_nStatus;
}

void CTester::SetHeartBit( CString sTimeStamp )
{
	CTime cTime = CTime::GetCurrentTime();
	m_TesterStatus.m_cChkTimeStamp = cTime;

	if(sTimeStamp.IsEmpty() == false)
		m_TesterStatus.m_sTimeStamp = sTimeStamp.GetString();
	m_TesterStatus.m_bHeartBit = TRUE;// !m_TesterStatus.m_bHeartBit;
}

void CTester::SetRegiDutIdentityNo(int nDutIdentityNo)
{
	m_TesterStatus.m_nRegiDutIdentityNo[0] = nDutIdentityNo;
}
void CTester::SetRegistration(int nReg)
{
	m_TesterStatus.m_nRegistration = nReg;
}

void CTester::SetInformation( CString sTesterID, CString sSocketIDs, CString sSW_Version, CString sIP )
{
	m_TesterStatus.m_sTesterID_Orginal = sTesterID;
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int socket_idx = 0;
	CString strTmp = _T("");

	for( int iStart = 0; socket_idx < STATION_MAX_PARA; ) {
		if (iStart >= 0) {
			strTmp = sSocketIDs.Tokenize(_T(","), iStart);

			if (strTmp.IsEmpty() != TRUE) {
				m_TesterStatus.m_sSocketID_Orginal[socket_idx++] = strTmp;
			}
			else {				
				m_TesterStatus.m_sSocketID_Orginal[socket_idx++] = REG_SOCKET_ID_ERROR;
			}
		}
		else {
				m_TesterStatus.m_sSocketID_Orginal[socket_idx++] = REG_SOCKET_ID_ERROR;
		}
	}

	m_TesterStatus.m_sSW_Version = sSW_Version;
	m_TesterStatus.m_sIP = sIP;
}
void CTester::SetInformation(std::vector<std::pair<std::string, std::string>> tester_data)
{
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	m_TesterStatus.m_sHW_version	 = tester_data[eTesterInfoEach_HW_Version].second.c_str();
	m_TesterStatus.m_sPPO_FW_version = tester_data[eTesterInfoEach_PPO_FW_Version].second.c_str();
	m_TesterStatus.m_sHID_FW_version = tester_data[eTesterInfoEach_HID_FW_Version].second.c_str();
	m_TesterStatus.m_sSW_Version = tester_data[eTesterInfoEach_SW_Version].second.c_str();
	if (nProjectOption != DEF_PROJECT_CARPENTERS) // Carpenter는 미사용, Handler APL 기반으로 Script info 생성
	{
		m_TesterStatus.m_sTest_Script_Info = tester_data[eTesterInfoEach_Test_Script].second.c_str();
	}
	
}

void CTester::SetTestScript(CString sTestScript)
{
	m_TesterStatus.m_sTest_Script_Apl = sTestScript;
}

void CTester::SetTestMode(int nTest_Mode)
{
	m_TesterStatus.m_nTest_Mode= nTest_Mode;
}


void CTester::SetStationID(int nID)
{
	m_TesterStatus.m_nStationID = nID;
}

void CTester::SetInfo( CString sInfo )
{
	m_TesterStatus.SetVersionInfo( sInfo, 0 );
}

void CTester::SetLocation( int nLocation )
{
	m_TesterStatus.m_nReserved2 = nLocation;
}

void CTester::SetOccupied( int nOccupied )
{
	m_TesterStatus.m_nReserved3 = nOccupied;
}

void CTester::SetEachTestResult(int nResult, int nReserved, const char* szErrStr, const char* szDutSN, const char* szSocketID, const char* szErrCode, int nIdx)
{
	if (nIdx >= STATION_MAX_PARA)
		return;

	m_TesterStatus.m_nLastTestResult[ nIdx ] = nResult;
	m_TesterStatus.m_nLastReserved[ nIdx ] = nReserved;
	m_TesterStatus.m_sLastErrString[ nIdx ] = szErrStr;
	m_TesterStatus.m_sLastDutSN[ nIdx ] = szDutSN;
	m_TesterStatus.m_sSocketID[ nIdx ] = szSocketID;
}

void CTester::SetTestResult(int nResult, int nReserved, const char* szErrStr, const char* szDutSN, const char* szSocketID, int nIdx)
{
	if (nIdx >= STATION_MAX_PARA)
		return;

	m_TesterStatus.m_nLastTestResult[nIdx] = nResult;
	m_TesterStatus.m_nLastReserved[nIdx] = nReserved;
	m_TesterStatus.m_sLastErrString[nIdx] = szErrStr;
	m_TesterStatus.m_sLastDutSN[nIdx] = szDutSN;
	m_TesterStatus.m_sSocketID[nIdx] = szSocketID;
}


void CTester::SendHeartBeatAction(RecvData recvData, WPARAM wParam/*=NULL*/, LPARAM lParam/*=NULL*/)
{
	BYTE sendData[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX] = { 0, };
	//char szTimeStamp[TIME_STAMP_STR] = {0,};

	recvData.m_DataHeader.m_bit.m_nStationID = 0;//server

	// Head
	unsigned int head = SWAP32(recvData.m_DataHeader.m_nData);
	memcpy(sendData, &head, sizeof(BYTE)*MAX_MSG_ID);

	// Len
	recvData.m_nDataLen = SEND_HEART_BEAT_DATA_MAX;
	unsigned short len = SWAP16(recvData.m_nDataLen);
	memcpy(&sendData[MAX_MSG_ID], &len, sizeof(unsigned short));

	// TimeStamp
	CTime CurTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());

	memcpy(&sendData[MAX_MSG_ID + 2], strTime.GetString(), sizeof(BYTE)*TIME_STAMP_STR);

	// CRC
	sendData[MAX_MSG_ID + (SEND_HEART_BEAT_DATA_MAX - 1)] = SEND_DATA_CRC;//m_SendData.m_nCRC;


	int nSendMsgLen = MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX;
	if (TesterNullPointerCheck(m_pSock) != NULL) {
		int nResult = m_pSock->Send(sendData, nSendMsgLen);
	}
	else {
		g_AppleTestIF.MakeLog("[SND HEART BEAT][H->T] Send Fail !!!!!!!!!!!   TesterID:%d, Type:%d", recvData.m_DataHeader.m_bit.m_nStationID, recvData.m_DataHeader.m_bit.m_nType);
	}
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString strDateTm = _T("");
		strDateTm.Format("%04d-%02d-%02d,%02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		g_AppleTestIF.MakeLog("%s,[SND HEART BEAT][H->T] TesterID:%d, Type:%d", strDateTm, recvData.m_DataHeader.m_bit.m_nStationID, recvData.m_DataHeader.m_bit.m_nType);
		g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "%s,[SND HEART BEAT][H->T] TesterID:%d, SendLen:%d ", strDateTm, m_SendData.m_DataHeader.m_bit.m_nStationID, nSendMsgLen);
	}
}

void CTester::SendTestAPLInfoAction()
{
	SendData sendData;
	BYTE data[MAX_MSG_ID + APL_INFO_DATA_MAX] = { 0, };
	
	sendData.m_DataHeader.m_bit.m_nReserved = 0;
	sendData.m_DataHeader.m_bit.m_nType = TESTER_APL_INFO;
	sendData.m_DataHeader.m_bit.m_nStationID = GetStationID();
	sendData.m_DataHeader.m_bit.m_nSc = 0;
	sendData.m_DataHeader.m_bit.m_nSm = 0;

	// Head
	unsigned int head = SWAP32(sendData.m_DataHeader.m_nData);
	memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

	// Len
	sendData.m_nDataLen = APL_INFO_DATA_MAX;
	unsigned short len = SWAP16(sendData.m_nDataLen);
	memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));


	CTime CurTime = CTime::GetCurrentTime();
	CString strTPRev;
	char szTP_Rev[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, MAX_PATH);
	strTPRev.Format("%s", szTP_Rev);

	CString strTPProg;
	char szTP_Prog[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szTP_Prog, MAX_PATH);
	strTPProg.Format("%s", szTP_Prog);


	CString strProcess;
	char szProcess[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szProcess, MAX_PATH);
	strProcess.Format("%s", szProcess);

	CString strHostName;
	char szHostName[MAX_PATH] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szHostName, sizeof(szHostName));
	strHostName.Format("%s", szHostName);

	CString strDevice;
	char szDevice[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szDevice, sizeof(szDevice));
	strDevice.Format("%s", szDevice);

	memcpy(&data[MAX_MSG_ID + 2], strTPRev.GetString(), sizeof(BYTE)*TEST_SCRIPT_STR_MAX);
	memcpy(&data[MAX_MSG_ID + 2 + TEST_SCRIPT_STR_MAX], strTPProg.GetString(), sizeof(BYTE)*TEST_SW_VERSION_STR_MAX);
	memcpy(&data[MAX_MSG_ID + 2 + TEST_SCRIPT_STR_MAX + TEST_RETEST_STR_MAX], strProcess.GetString(), sizeof(BYTE)*TEST_RETEST_STR_MAX);
	memcpy(&data[MAX_MSG_ID + 2 + TEST_SCRIPT_STR_MAX + TEST_RETEST_STR_MAX + TEST_HANDLER_ID_STR_MAX], strHostName.GetString(), sizeof(BYTE)*TEST_HANDLER_ID_STR_MAX);
	memcpy(&data[MAX_MSG_ID + 2 + TEST_SCRIPT_STR_MAX + TEST_RETEST_STR_MAX + TEST_HANDLER_ID_STR_MAX + TEST_DEVICE_STR_MAX], strDevice.GetString(), sizeof(BYTE)*TEST_DEVICE_STR_MAX);
	// CRC
	data[MAX_MSG_ID + (APL_INFO_DATA_MAX - 1)] = SEND_DATA_CRC;//m_SendData.m_nCRC;


	int nSendMsgLen = MAX_MSG_ID + APL_INFO_DATA_MAX;
	if (TesterNullPointerCheck(m_pSock) != NULL) {
		int nResult = m_pSock->Send(data, nSendMsgLen);
	}
	else {
		g_AppleTestIF.MakeLog("[SND TP REV][H->T] Send Fail!!!!!!!!!!!!!!!");
	}
//	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
// 	if (nTesterOption == eTester_Single) {
// 		SYSTEMTIME st;
// 		GetLocalTime(&st);
// 		CString strDateTm = _T("");
// 		strDateTm.Format("%04d-%02d-%02d,%02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
// 
// 		g_AppleTestIF.MakeLog("%s,[SND HEART BEAT][H->T] TesterID:%d, Type:%d", strDateTm, recvData.m_DataHeader.m_bit.m_nStationID, recvData.m_DataHeader.m_bit.m_nType);
// 		g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "%s,[SND HEART BEAT][H->T] TesterID:%d, SendLen:%d ", strDateTm, m_SendData.m_DataHeader.m_bit.m_nStationID, nSendMsgLen);
// 	}
}

void CTester::SendTestLotStatusAction(eTesterLotStatus _eTesterLotStatus)
{
	SendData sendData;
	BYTE data[MAX_MSG_ID + TEST_LOT_STATUS_STR_MAX] = { 0, };

	sendData.m_DataHeader.m_bit.m_nReserved = 0;
	sendData.m_DataHeader.m_bit.m_nType = TESTER_LOT_STATUS;
	sendData.m_DataHeader.m_bit.m_nStationID = GetStationID();
	sendData.m_DataHeader.m_bit.m_nSc = 0;
	sendData.m_DataHeader.m_bit.m_nSm = 0;

	// Head
	unsigned int head = SWAP32(sendData.m_DataHeader.m_nData);
	memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

	// Len
	sendData.m_nDataLen = TEST_LOT_STATUS_STR_MAX;
	unsigned short len = SWAP16(sendData.m_nDataLen);
	memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

	data[MAX_MSG_ID + 2] = static_cast<BYTE>(_eTesterLotStatus);

	// CRC
	data[MAX_MSG_ID + (TEST_LOT_STATUS_STR_MAX - 1)] = SEND_DATA_CRC;//m_SendData.m_nCRC;


	int nSendMsgLen = MAX_MSG_ID + TEST_LOT_STATUS_STR_MAX;
	if (TesterNullPointerCheck(m_pSock) != NULL) {
		int nResult = m_pSock->Send(data, nSendMsgLen);
	}
	else {
		g_AppleTestIF.MakeLog("[SND TP REV][H->T] Send Fail!!!!!!!!!!!!!!!");
	}
	//	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
	// 	if (nTesterOption == eTester_Single) {
	// 		SYSTEMTIME st;
	// 		GetLocalTime(&st);
	// 		CString strDateTm = _T("");
	// 		strDateTm.Format("%04d-%02d-%02d,%02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	// 
	// 		g_AppleTestIF.MakeLog("%s,[SND HEART BEAT][H->T] TesterID:%d, Type:%d", strDateTm, recvData.m_DataHeader.m_bit.m_nStationID, recvData.m_DataHeader.m_bit.m_nType);
	// 		g_AppleTestIF.MakeHexLog(sendData, nSendMsgLen, "%s,[SND HEART BEAT][H->T] TesterID:%d, SendLen:%d ", strDateTm, m_SendData.m_DataHeader.m_bit.m_nStationID, nSendMsgLen);
	// 	}
}

void CTester::Disconnection()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
		g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (m_TesterStatus.m_nRegiDutIdentityNo[0] - 1), FALSE);
	else
		g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (m_TesterStatus.m_nStationID - 1), FALSE);


	m_pSock->Close();
	m_pSock = NULL;
}

void CTester::Connection()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
		g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (m_TesterStatus.m_nRegiDutIdentityNo[0] - 1), TRUE);
	else
		g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (m_TesterStatus.m_nStationID - 1), TRUE);

}

void CTester::LastResultReset()
{
	int nStation_Max_Para;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single) // Single Tester [8/27/2021 donghyun.shin]
		nStation_Max_Para = STATION_MAX_PARA;
	else if(nTesterOption == eTester_Dual)
		nStation_Max_Para = STATION_MAX_PARA / 2;
	else
	{
		nStation_Max_Para = 1;
	}
	

	for (int i = 0; i < nStation_Max_Para; i++) {
		m_TesterStatus.m_nLastTestResult[i] = -1;
		m_TesterStatus.m_sLastErrString[i] = _T("");
	}
}

CString CTester::GetStringActionType(int nAction)
{
	CString sAction = _T("");
	switch (nAction)
	{
	case ACTION_START:		sAction = "START_OF_TEST";		break;
	case ACTION_STOP:		sAction = "STOP";		break;
	case ACTION_RESET:		sAction = "RESET";		break;
	case ACTION_STATUS:	sAction = "STATUS"; break;
	case ACTION_INFO:		sAction = "INFOMATION";		break;
	case ACTION_TEST_SCRIPT_INFO: sAction = "SCRIPT_INFOMATION";		break;
	case ACTION_SET_TEST_MODE:	sAction = "SET_TEST_MODE"; break;
	}
	return sAction;
};

int CTester::TesterNullPointerCheck(CTesterClientSock* pSock)
{
	if (pSock == NULL || pSock == DEF_TESTER_HEAP_POINTER_DELETE || pSock == DEF_TESTER_POINTER_DELETE)
		return NULL;


	return TRUE;
}

CString CTester::GetTestMode()
{
	CString strTestMode = _T("");
	switch (m_TesterStatus.m_nTest_Mode)
	{
	case _eTestModeSLT1: { strTestMode = "SLT1";}break;
	case _eTestModeSLTQA1: { strTestMode = "SLTQA1";}break;
	default:
		break;
	}
	return strTestMode;
}

void CTester::SetAPLInfo(const char* szTestScriptInfo, const char* szTestSWVersion, const char* szTestRetest, const char* szTestHandlerID, const char* szTestDevice)
{
	m_TesterStatus.m_sTestScriptInfo = szTestScriptInfo;
	m_TesterStatus.m_sTestSWVersion = szTestSWVersion;
	m_TesterStatus.m_sTestRetest = szTestRetest;
	m_TesterStatus.m_sTestHandlerID = szTestHandlerID;
	m_TesterStatus.m_sTestDevice = szTestDevice;
}
