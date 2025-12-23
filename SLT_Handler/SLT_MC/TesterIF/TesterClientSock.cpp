#include "StdAfx.h"
#include "TesterClientSock.h"
#include "TesterIF.h"
#include <shlwapi.h>
#include <afxstr.h>
#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTesterClientSock::CTesterClientSock(void)
{
	m_pTesterIF = NULL;
	//m_pServerSock = NULL;

	//m_nIdx = 0;
	m_bConnect = FALSE;

	m_nSplitCount = 0;
	ZeroMemory(m_szSplit, sizeof(m_szSplit));

	ResetData();
}


CTesterClientSock::~CTesterClientSock(void)
{
}

void CTesterClientSock::OnClose(int nErrorCode)
{
	m_bConnect = FALSE;
	m_pTesterIF->m_TesterMgr.DisconnectSock(this);
}

void CTesterClientSock::OnSend(int nErrorCode)
{

}

void CTesterClientSock::ResetData()
{
	memset(&m_RecvData, 0x00, sizeof(RecvData));
	memset(&m_SendData, 0x00, sizeof(SendData));
}

void CTesterClientSock::OnReceive(int nErrorCode)
{
	char sData[40000]={0,};
	int nRdLength = Receive(sData, 4096);

	SYSTEMTIME st;
	GetLocalTime(&st);

	char t = 't';

	char test[2] = {'a', 'b'};
	char test1 = {};

	if(nRdLength <= 0/*RET_ERROR*/){
		TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Info, "", __LINE__, NULL,"%04d-%02d-%02d,%02d:%02d:%02d.%03d,Tester OnReceive Err= %d", st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, nRdLength);
		return;
	}

    int nRevLength = nRdLength;

    // Rev TCP/IP Log
    char szHexLog[40960]={0,};
    for(int i=0; i<nRdLength; i++){
        char szTemp[16]={0,};
        sprintf_s(szTemp, "%02X ", (unsigned char)sData[i]);
        strcat_s(szHexLog, szTemp);
    }
    // Send TCP Log Hex
    TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL
        ,"%04d-%02d-%02d,%02d:%02d:%02d.%03d,[T->H (fullData)][HEX : %s]"
        ,st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,szHexLog);


	if( m_nSplitCount != 0 ){
		memcpy(&m_szSplit[m_nSplitCount], sData, nRdLength);
		memcpy(sData, m_szSplit, nRdLength+m_nSplitCount);
		nRdLength += m_nSplitCount;
		ZeroMemory(m_szSplit, sizeof(m_szSplit));
		m_nSplitCount = 0;
	}

	int nRemainMsgLen = nRdLength;
	char* pStart = &sData[0];

	while(nRemainMsgLen != 0){
		// Head
		BYTE head[MAX_MSG_ID] = {0,};
		memcpy((void*)head, pStart, MAX_MSG_ID);

		RecvData* pRcvData = new RecvData;
		if (pRcvData != NULL)
			ZeroMemory(pRcvData, sizeof(RecvData));

		
		ParserMsgID(head, *pRcvData);

		int nMsgDataTotalLen = 0;
		nMsgDataTotalLen = *(BYTE*)&pStart[MAX_MSG_ID] << 8;
		nMsgDataTotalLen += *(BYTE*)&pStart[MAX_MSG_ID+1];

		int nPacketSize = MAX_MSG_ID + nMsgDataTotalLen;
		if( nRemainMsgLen < nPacketSize ){
			memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
			m_nSplitCount += nRemainMsgLen;

			if( pRcvData ){
				delete pRcvData; 
				pRcvData = NULL; 
			}
			break;
		}

		BYTE data[MSG_MAX] = {0,};
		memcpy(data, pStart+MAX_MSG_ID, nMsgDataTotalLen);

		
		ParserMsgData(data, *pRcvData, nRevLength);

		if( pRcvData ){
			delete pRcvData;
			pRcvData = NULL;
		}		

		pStart += nPacketSize;
		nRemainMsgLen -= nPacketSize;

		if( nRemainMsgLen < MAX_MSG_ID ){
			memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
			m_nSplitCount += nRemainMsgLen;
			break;
		}
	}

    TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "****");
}

void CTesterClientSock::ParserMsgID(BYTE* data, RecvData& recvData)
{
	memcpy(&(recvData.m_DataHeader.m_nData), data, sizeof(BYTE)*MAX_MSG_ID);
	recvData.m_DataHeader.m_nData = SWAP32(recvData.m_DataHeader.m_nData);

	recvData.m_nReserved = recvData.m_DataHeader.m_bit.m_nReserved;
	recvData.m_nType     = recvData.m_DataHeader.m_bit.m_nType;
	recvData.m_nStationID = recvData.m_DataHeader.m_bit.m_nStationID;
    recvData.m_nSc       = recvData.m_DataHeader.m_bit.m_nSc;
	recvData.m_nSm       = recvData.m_DataHeader.m_bit.m_nSm;
}

void CTesterClientSock::ParserMsgData(BYTE* data,RecvData& recvData, int nRevLength)
{
	const int DATA_LEN = 2;
	const int CRC_LEN = 1;

	recvData.m_nDataLen = data[0] << 8;
	recvData.m_nDataLen += data[1];

	int dLen = recvData.m_nDataLen - DATA_LEN - CRC_LEN;

    int nProcessedMsgDataLen = recvData.m_nDataLen;

    // TCP/IP LOG =================================================================================================================================
	int nSiteIdx = recvData.m_nStationID;// m_pTesterIF->GetStationID(recvData.m_nStationID);
    // Send TCP Log Hex
    //SYSTEMTIME st;
    //GetLocalTime(&st);
    //CString strDateTm = _T("");
    //strDateTm.Format("%04d-%02d-%02d,%02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // Format Ex :: 2016-03-21,20:18:19,Site,REV,01 AE 67 B0 00 ....
	//m_pTesterIF->MakeHexLog( data, nProcessedMsgDataLen, "%s,[T->H (NotImportHeader)][Idx=%d][Rev StationID=%d]", strDateTm, nSiteIdx, recvData.m_nStationID);

	CString strType = _T(""),strType1 = _T(""),strType2 = _T("");    
    switch(recvData.m_nType)
    {
    case TESTER_REG			: { strType="TESTER REG";         }break;
    case ACTION				: { strType="ACTION";             }break;
    case TESTER_STAUS		: { strType="TESTER STAUS";       }break;
    case TEST_RESULT		: { strType="TESTER RESULT";      }break;
    case ACKNOWLEDGEMENT	: { strType="ACKNOWLEDGEMENT";    }break;
    case HEART_BEAT			: { strType="HEART BEAT";         }break;
    case TESTER_INFORMATION	: { strType="TESTER INFORMATION"; }break;
	case TESTER_REG_FOR_AMKOR: { strType = "TESTER REG FOR AMKOR"; }break;
	case TESTER_TEST_SCRIPT: { strType = "TESTER TEST SCRIPT"; }break;
	case SET_TESTER_MODE: { strType = "SET_TESTER_MODE"; }break;
	case TESTER_APL_INFO: { strType = "TESTER_APL_INFO"; }break;
				
    default: { strType="NONE"; } break;
    }

	switch(recvData.m_nSc)  //0: Server to Client; 1: Client to Server
	{
	case 0	: { strType1="Server to Client"; }break;
	case 1	: { strType1="Client to Server"; }break;
	}

	switch(recvData.m_nSm)  //0: Single packet; 1: Multiple packet
	{
	case 0	: { strType2="Single";   }break;
	case 1	: { strType2="Multiple"; }break;
	}

    // Format Ex :: 2016-03-21,20:18:19,Site,4,REM,[Reserved:0],[Type:6, /Heart Beat][StationID:760300][SC:0 / Server to Client][Process Length:17][Receive Length:63]
	TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[T->H][Idx:%d][Reserved:%d][Type:%d/%s][StationID:%d][SC:%d/%s][SM:%d/%s][Process Length:%d][Receive Length:%d]"
		, nSiteIdx, recvData.m_nReserved, recvData.m_nType, strType, recvData.m_nStationID, recvData.m_nSc, strType1,  recvData.m_nSm, strType2, recvData.m_nDataLen,nRevLength );

    // ============================================================================================================================================

	if( recvData.m_nType == TESTER_REG  || recvData.m_nType == TESTER_REG_FOR_AMKOR )
	{
		recvData.m_nRegister = data[DATA_LEN];
		dLen--;

		memcpy(recvData.m_cRegStr, &data[DATA_LEN+1], sizeof(BYTE)*dLen);
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TESTER_REG), Info, "", __LINE__, NULL, "[RCV] [Type:%d (REG:%s)] [StationID:%d] [TesterInfo:%s]", recvData.m_nType, (recvData.m_nRegister == 1 ? _T("Register") : _T("Unregister")), recvData.m_nStationID, recvData.m_cRegStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[RCV] [Type:%d (REG:%s)] [StationID:%d] [TesterInfo:%s]", recvData.m_nType, (recvData.m_nRegister == 1 ? _T("Register") : _T("Unregister")), recvData.m_nStationID, recvData.m_cRegStr);
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (REG:%s)] [StationID:%d] [TesterInfo:%s]", recvData.m_nType, (recvData.m_nRegister == 1 ? _T("Register") : _T("Unregister"), recvData.m_nStationID, recvData.m_cRegStr);

		switch( recvData.m_nRegister ) {
			case 0 : // Ungrgister
				{			
					int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
					int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

					if (nProjectOption == DEF_PROJECT_WATT)
					{
						m_pTesterIF->m_TesterMgr.AllUnRegTester();
					}
					else {
						m_pTesterIF->m_TesterMgr.UnRegTester(recvData);
					}
												
				}break;
			case 1 : // Register
				{ 
					m_pTesterIF->m_TesterMgr.RegTester( recvData, this );
				}break;
			case 2: //Yanmade Unregister-TW
				{
					m_pTesterIF->m_TesterMgr.UnRegTesterIdentity(recvData);
				}break;
		}
		
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (REG:%d)] [StationID:%d] [TesterInfo:%s]", recvData.m_nType, recvData.m_nRegister, recvData.m_nStationID, recvData.m_cRegStr);
	}
	else if(recvData.m_nType == TESTER_STAUS)
	{
		recvData.m_nStatus = data[DATA_LEN];
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

		m_pTesterIF->m_TesterMgr.SetStatus(recvData.m_nStationID, recvData.m_nStatus);

        CString strStatus = _T("");
        switch(recvData.m_nStatus) 
        {
        case STATUS_INIT      :	{ strStatus = "INIT";		} break;
        case STATUS_IDLE      :	{ strStatus = "IDLE";		}
		{
			//m_pTesterIF->m_TesterMgr.SetStatus()

			

			CTester *pTester = m_pTesterIF->m_TesterMgr.GetTester(recvData.m_nStationID);

			if (pTester != NULL)
			{
				CTesterStatus eStatusTester;

				eStatusTester = pTester->GetTesterStatus();

				if (eStatusTester.m_nTesterInformation == _eSetTesterInformation::_TesterInformationNeedResetSend) // rest 보낸 상태.
				{
					pTester->SetTesterInformationStatus(_eSetTesterInformation::_TesterInformationNeedResetOK);
				}
			}

			
		}break;
        case STATUS_TESTING   :	{ strStatus = "TESTING";	} break;
        case STATUS_ERROR     :	{ strStatus = "ERROR";		} break;
        case STATUS_UNREGISTER:	{ strStatus = "UNREGISTER"; } break;
        }

		
// 		if (nTesterOption == eTester_EachSocket)
// 		{
// 			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[RCV] [Type:%d (STATUS)] [StationID:%d] [Socket No:%d][Status:%s]", recvData.m_nType, recvData.m_nStationID, m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID), strStatus);
// 			int nSocketIdx = m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID);
// 
// 			if(nSocketIdx >= 1 && nSocketIdx <= 64)
// 				TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_1 + ((nSocketIdx - 1) /eMaxPressUnitCount) ), Info, "", __LINE__, NULL, "[RCV] [Type:%d (STATUS)] [StationID:%d] [Socket No:%d][Status:%s]", recvData.m_nType, recvData.m_nStationID, m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID), strStatus);
// 		}
// 		else
// 		{ 
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TESTER_STAUS), Info, "", __LINE__, NULL, "[RCV] [Type:%d (STATUS)] [StationID:%d] [Status:%s]", recvData.m_nType, recvData.m_nStationID, strStatus);
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[RCV] [Type:%d (STATUS)] [StationID:%d] [Status:%s]", recvData.m_nType, recvData.m_nStationID, strStatus);
//		}
        //TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Idx=%d],[TesterStatus:%d/%s]",nSiteIdx, recvData.m_nType, strStatus);
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (STATUS)] [StationID:%d] [Status:%s]", recvData.m_nType, recvData.m_nStationID, strStatus);
	}
	else if(recvData.m_nType == TEST_RESULT)
	{
//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
		if (nTesterOption == eTester_EachSocket)
		{
			recvData.m_nTestResult[0] = data[DATA_LEN]; // 테스트 결과

			recvData.m_nReserved1[0] = *(int*)&data[DATA_LEN + 1];	// error code

	// 		memcpy(recvData.m_cErrorCode[0], &data[DATA_LEN + 1], sizeof(BYTE)*ERR_CODE_STR); // byte 7~38
	// 		recvData.m_cErrorCode[0][ERR_CODE_STR - 1] = 0;
	// 		StrTrim(recvData.m_cErrorCode[0], " ");

			memcpy(recvData.m_cErrorStr[0], &data[DATA_LEN + 5], sizeof(BYTE)*ERR_STR); // byte 7~38
			recvData.m_cErrorStr[0][ERR_STR - 1] = 0;
			StrTrim(recvData.m_cErrorStr[0], " ");

			memcpy(recvData.m_cDutSn[0], &data[DATA_LEN + 5 + ERR_STR], sizeof(BYTE)*DUT_SN_STR_EACH);
			recvData.m_cDutSn[0][DUT_SN_STR_EACH - 1] = 0;
			StrTrim(recvData.m_cDutSn[0], " ");

			//memcpy(recvData.m_cSocketID[i], &data[DATA_LEN + 5 + ERR_STR + DUT_SN_STR + (i * RESULT_DATA_STR)], sizeof(BYTE)*SOCKET_ID_STR);
			//recvData.m_cSocketID[i][SOCKET_ID_STR - 1] = 0;
			//StrTrim(recvData.m_cSocketID[i], " ");

			recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

			m_pTesterIF->m_TesterMgr.SetEachTestResult(
				recvData.m_nStationID, recvData.m_nTestResult[0], recvData.m_nReserved1[0], recvData.m_cErrorStr[0], recvData.m_cDutSn[0], recvData.m_cSocketID[0], recvData.m_cErrorCode[0], 0);

			//m_pTesterIF->MakeLog("[RCV] [Type:%d (RESULT)] [StationID:%d] [TesterNum:%d] [Result:%d] [Reserved:%d] [ErrStr:%s] [DutSn:%s] [SocketID:%s]",
			//					  recvData.m_nType, recvData.m_nStationID, i+1, recvData.m_nTestResult[ i ], recvData.m_nReserved1[ i ], recvData.m_cErrorStr[ i ], recvData.m_cDutSn[ i ], recvData.m_cSocketID[ i ] );

			CString strStatus = _T("");
			switch (recvData.m_nTestResult[0])
			{
			case 0: { strStatus = "FAIL";		} break;
			case 1: { strStatus = "PASS";		} break;
			}
			//TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "%d-%d,[TestResult:%d/%s][ErrorString:%s]",nSiteIdx, i+1, recvData.m_nTestResult[ i ], strStatus, recvData.m_cErrorStr[ i ]);
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TEST_RESULT), Info, "", __LINE__, NULL, "[Station:%d] [SocketNo:%d],[TestResult:%d/%s][ErrorString:%s][DutSn:%s][SocketID:%s][Reserved1=%d][ErrorCode=%s]", nSiteIdx, m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID), recvData.m_nTestResult[0], strStatus, recvData.m_cErrorStr[0], recvData.m_cDutSn[0], recvData.m_cSocketID[0], recvData.m_nReserved1[0], recvData.m_cErrorCode[0]);
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Station:%d] [SocketNo:%d],[TestResult:%d/%s][ErrorString:%s][DutSn:%s][SocketID:%s][Reserved1=%d][ErrorCode=%s]", nSiteIdx, m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID), recvData.m_nTestResult[0], strStatus, recvData.m_cErrorStr[0], recvData.m_cDutSn[0], recvData.m_cSocketID[0], recvData.m_nReserved1[0], recvData.m_cErrorCode[0]);

// 			int nSocketIdx = m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID);
// 
// 			if (nSocketIdx >= 1 && nSocketIdx <= 64)
// 				TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_1 + ((nSocketIdx - 1) / eMaxPressUnitCount)), Info, "", __LINE__, NULL, "[Station:%d] [SocketNo:%d],[TestResult:%d/%s][ErrorString:%s][DutSn:%s][SocketID:%s][Reserved1=%d][ErrorCode=%s]", nSiteIdx, m_pTesterIF->m_TesterMgr.GetDutIdentityNo(recvData.m_nStationID), recvData.m_nTestResult[0], strStatus, recvData.m_cErrorStr[0], recvData.m_cDutSn[0], recvData.m_cSocketID[0], recvData.m_nReserved1[0], recvData.m_cErrorCode[0]);
		}
		else
		{
			for (int i = 0; i < STATION_MAX_PARA; i++) {
				recvData.m_nTestResult[i] = data[DATA_LEN + (i * RESULT_DATA_STR)];

				recvData.m_nReserved1[i] = *(int*)&data[DATA_LEN + 1 + (i* RESULT_DATA_STR)];

				memcpy(recvData.m_cErrorStr[i], &data[DATA_LEN + 5 + (i * RESULT_DATA_STR)], sizeof(BYTE)*ERR_STR);
				recvData.m_cErrorStr[i][ERR_STR - 1] = 0;
				StrTrim(recvData.m_cErrorStr[i], " ");

				memcpy(recvData.m_cDutSn[i], &data[DATA_LEN + 5 + ERR_STR + (i * RESULT_DATA_STR)], sizeof(BYTE)*DUT_SN_STR);
				recvData.m_cDutSn[i][DUT_SN_STR - 1] = 0;
				StrTrim(recvData.m_cDutSn[i], " ");

				memcpy(recvData.m_cSocketID[i], &data[DATA_LEN + 5 + ERR_STR + DUT_SN_STR + (i * RESULT_DATA_STR)], sizeof(BYTE)*SOCKET_ID_STR);
				recvData.m_cSocketID[i][SOCKET_ID_STR - 1] = 0;
				StrTrim(recvData.m_cSocketID[i], " ");
			}

			recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

			for (int i = 0; i < STATION_MAX_PARA; i++) {
				m_pTesterIF->m_TesterMgr.SetTestResult(
					recvData.m_nStationID, recvData.m_nTestResult[i], recvData.m_nReserved1[i], recvData.m_cErrorStr[i], recvData.m_cDutSn[i], recvData.m_cSocketID[i], i);

				//m_pTesterIF->MakeLog("[RCV] [Type:%d (RESULT)] [StationID:%d] [TesterNum:%d] [Result:%d] [Reserved:%d] [ErrStr:%s] [DutSn:%s] [SocketID:%s]",
				//					  recvData.m_nType, recvData.m_nStationID, i+1, recvData.m_nTestResult[ i ], recvData.m_nReserved1[ i ], recvData.m_cErrorStr[ i ], recvData.m_cDutSn[ i ], recvData.m_cSocketID[ i ] );

				CString strStatus = _T("");
				switch (recvData.m_nTestResult[i])
				{
				case 0: { strStatus = "FAIL";		} break;
				case 1: { strStatus = "PASS";		} break;
				}
				//TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "%d-%d,[TestResult:%d/%s][ErrorString:%s]",nSiteIdx, i+1, recvData.m_nTestResult[ i ], strStatus, recvData.m_cErrorStr[ i ]);
				TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TEST_RESULT), Info, "", __LINE__, NULL, "[Station:%d Site:%d],[TestResult:%d/%s][ErrorString:%s][DutSn:%s][SocketID:%s]", nSiteIdx, i + 1, recvData.m_nTestResult[i], strStatus, recvData.m_cErrorStr[i], recvData.m_cDutSn[i], recvData.m_cSocketID[i]);
				TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Station:%d Site:%d],[TestResult:%d/%s][ErrorString:%s][DutSn:%s][SocketID:%s]", nSiteIdx, i + 1, recvData.m_nTestResult[i], strStatus, recvData.m_cErrorStr[i], recvData.m_cDutSn[i], recvData.m_cSocketID[i]);
			}
		}
//#endif
	}
	else if(recvData.m_nType == HEART_BEAT)
	{
		//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0 

// 		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
// 		if (nTesterOption == eTester_EachSocket)
// 			return; //yanmade tester heartbeat 사용 안함.

		if( recvData.m_nDataLen <= 0 || recvData.m_nDataLen > SEND_HEART_BEAT_DATA_MAX || dLen <= 0 || nProcessedMsgDataLen <= 0 ){
			TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "==== Rev Zero Lenght [Data=%d, dLen=%d, ProcLen=%d]=== ", recvData.m_nDataLen, dLen, nProcessedMsgDataLen);
			return;
		}

		memcpy(recvData.m_cTimeStamp, &data[DATA_LEN], sizeof(BYTE)*dLen);
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

		m_pTesterIF->m_TesterMgr.SetTimeStamp(recvData.m_nStationID, CString(recvData.m_cTimeStamp));
		m_pTesterIF->m_TesterMgr.SendHeartBeat(recvData.m_nStationID, recvData);

		CTester *pTester = m_pTesterIF->m_TesterMgr.GetTester(recvData.m_nStationID);

		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		if (nProjectOption == DEF_PROJECT_WATT)
		{
			if (pTester != NULL)
			{
				CTesterStatus eStatusTester;

				eStatusTester = pTester->GetTesterStatus();

				if (eStatusTester.m_nTesterInformation == _eSetTesterInformation::_TesterInformationIdle) // rest 보낸 상태.
				{			
					pTester->SetTesterInformationStatus(_eSetTesterInformation::_TesterScriptHeartBeatNeedResetSend);
				}
			}
		}
	
		//#endif
	}
	else if(recvData.m_nType == TESTER_INFORMATION)
	{
		recvData.m_nReserved2 = data[DATA_LEN]; // tester location 사용 안함
		dLen--;

		recvData.m_nReserved3 = data[DATA_LEN+1];
		dLen--;
		
		memcpy(recvData.m_cInfoStr, &data[DATA_LEN+2], sizeof(BYTE)*dLen);
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		{
			sprintf_s(recvData.m_cInfoStr, 256, "%s", "Tester_HW:\"3\"; Tester_PPOFW:\"2.0.526\"; Tester_HIDFW:\"1.49.9\"; TestVersion:\"1.0.0.49 - OQC\"; Testscript:\"22\;");
		}

		m_pTesterIF->m_TesterMgr.SetInformation(recvData.m_nStationID, CString(recvData.m_cInfoStr) );

		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
		if (nTesterOption == eTester_EachSocket)
		{
			m_pTesterIF->m_TesterMgr.SetInformationStatus(recvData.m_nStationID, _eSetTesterInformation::_TesterInformationOK);
		}



		//m_pTesterIF->MakeLog("[RCV] [Type:%d (INFO)] [StationID:%d] [Location:%d] [Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TESTER_INFORMATION), Info, "", __LINE__, NULL, "[Type:%d/Information][StationID:%d][Reserve:%d][Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Type:%d/Information][StationID:%d][Reserve:%d][Info:%s]",recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
	}
	else if (recvData.m_nType == TESTER_TEST_SCRIPT)
	{
		recvData.m_nReserved2 = data[DATA_LEN]; // tester location 사용 안함
		dLen--;

		recvData.m_nReserved3 = data[DATA_LEN + 1];
		dLen--;

		memcpy(recvData.m_cInfoStr, &data[DATA_LEN + 2], sizeof(BYTE)*dLen);
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		{
			//sprintf_s(recvData.m_cInfoStr, 256, "%s", "Tester_HW:\"3\"; Tester_PPOFW:\"2.0.526\"; Tester_HIDFW:\"1.49.9\"; TestVersion:\"1.0.0.49 - OQC\"; Testscript:\"22\;");
		}

		m_pTesterIF->m_TesterMgr.SetTestScriptInfomation(recvData.m_nStationID, CString(recvData.m_cInfoStr));
		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
		if (nTesterOption == eTester_EachSocket)
		{
			m_pTesterIF->m_TesterMgr.SetInformationStatus(recvData.m_nStationID, _eSetTesterInformation::_TesterScriptInformationNeedResetOK);
		}
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (INFO)] [StationID:%d] [Location:%d] [Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION), Info, "", __LINE__, NULL, "[Type:%d/Information][StationID:%d][Reserve:%d][Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Type:%d/Information][StationID:%d][Reserve:%d][Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
	}
	else if (recvData.m_nType == SET_TESTER_MODE)
	{
		recvData.m_nReserved2 = data[DATA_LEN]; // tester location 사용 안함
		dLen--;

		recvData.m_nReserved3 = data[DATA_LEN + 1];
		dLen--;

		memcpy(recvData.m_cInfoStr, &data[DATA_LEN + 2], sizeof(BYTE)*dLen);
		recvData.m_nCRC = data[recvData.m_nDataLen - CRC_LEN];


		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		if (nProjectOption == DEF_PROJECT_WATT) {
			m_pTesterIF->m_TesterMgr.SetTestMode(recvData.m_nStationID, CString(recvData.m_cInfoStr));

			CTester *pTester = m_pTesterIF->m_TesterMgr.GetTester(recvData.m_nStationID);

			if (pTester != NULL)
			{
				CTesterStatus eStatusTester;

				eStatusTester = pTester->GetTesterStatus();

				if (eStatusTester.m_nTesterInformation == _eSetTesterInformation::_TesterScriptHeartBeatNeedResetOK) // rest 보낸 상태.
				{
					pTester->SetTesterInformationStatus(_eSetTesterInformation::_TesterAPLInfoSend);
				}
			}		
		}
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (INFO)] [StationID:%d] [Location:%d] [Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_SET_TEST_MODE), Info, "", __LINE__, NULL, "[Type:%d/SetTestMode][StationID:%d][Reserve:%d][Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Type:%d/SetTestMode][StationID:%d][Reserve:%d][Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
	}
	else if (recvData.m_nType == TESTER_APL_INFO)
	{
		char szTestScriptInfo[TEST_APL_SCRIPT_INFO_STR] = { 0, };
		memcpy(szTestScriptInfo, &data[DATA_LEN], sizeof(BYTE)*TEST_APL_SCRIPT_INFO_STR);
		szTestScriptInfo[TEST_APL_SCRIPT_INFO_STR - 1] = 0;
		StrTrim(szTestScriptInfo, " ");

		char szTestSWVersion[TEST_APL_SW_VERSION_INFO_STR] = { 0, };
		memcpy(szTestSWVersion, &data[DATA_LEN + TEST_APL_SCRIPT_INFO_STR], sizeof(BYTE)*TEST_APL_SW_VERSION_INFO_STR);
		szTestSWVersion[TEST_APL_SW_VERSION_INFO_STR - 1] = 0;
		StrTrim(szTestSWVersion, " ");

		char szTestRetest[TEST_APL_RETEST_STR] = { 0, };
		memcpy(szTestRetest, &data[DATA_LEN + TEST_APL_SCRIPT_INFO_STR + TEST_APL_RETEST_STR], sizeof(BYTE)*TEST_APL_RETEST_STR);
		szTestRetest[TEST_APL_RETEST_STR - 1] = 0;
		StrTrim(szTestRetest, " ");

		char szHandlerID[TEST_APL_HANDLER_ID_STR] = { 0, };
		memcpy(szHandlerID, &data[DATA_LEN + TEST_APL_SCRIPT_INFO_STR + TEST_APL_RETEST_STR + TEST_APL_HANDLER_ID_STR], sizeof(BYTE)*TEST_APL_HANDLER_ID_STR);
		szHandlerID[TEST_APL_HANDLER_ID_STR - 1] = 0;
		StrTrim(szHandlerID, " ");

		char szDevice[TEST_APL_DEVICE_STR] = { 0, };
		memcpy(szDevice, &data[DATA_LEN + TEST_APL_SCRIPT_INFO_STR + TEST_APL_RETEST_STR + TEST_APL_HANDLER_ID_STR + TEST_APL_DEVICE_STR], sizeof(BYTE)*TEST_APL_DEVICE_STR);
		szDevice[TEST_APL_DEVICE_STR - 1] = 0;
		StrTrim(szDevice, " ");

		m_pTesterIF->m_TesterMgr.SetAPLInfo(recvData.m_nStationID, CString(szTestScriptInfo), CString(szTestSWVersion), CString(szTestRetest), CString(szHandlerID), CString(szDevice));
		
		//m_pTesterIF->MakeLog("[RCV] [Type:%d (INFO)] [StationID:%d] [Location:%d] [Info:%s]", recvData.m_nType, recvData.m_nStationID, recvData.m_nReserved2, recvData.m_cInfoStr);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP_APL_INFO), Info, "", __LINE__, NULL, "[Type:%d/TESTER_APL_INFO][StationID:%d][szTestScriptInfo:%s][szTestSWVersion:%s][szTestRetest=%s][szHandlerID=%s][szDevice=%s]", recvData.m_nType, recvData.m_nStationID, szTestScriptInfo, szTestSWVersion, szTestRetest, szHandlerID, szDevice);
		TWLOG.MakeLog(static_cast<int>(eLogID_TCPIP), Info, "", __LINE__, NULL, "[Type:%d/TESTER_APL_INFO][StationID:%d][szTestScriptInfo:%s][szTestSWVersion:%s][szTestRetest=%s][szHandlerID=%s][szDevice=%s]", recvData.m_nType, recvData.m_nStationID, szTestScriptInfo, szTestSWVersion, szTestRetest, szHandlerID, szDevice);
	}
}


