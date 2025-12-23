// ConectSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConectSocket.h"
#include "ParamList.h"
// CConectSocket

CIocpCore g_iocp;
void OnReceive(CClientConnection* client)
{
	CConectSocket* ptr = (CConectSocket*)client->GetParentPtr();
	if (!ptr)
		return;
	ptr->OnReceive(client);
}


void OnClose(CClientConnection* client)
{
	CConectSocket* ptr = (CConectSocket*)client->GetParentPtr();
	if (!ptr)
		return;
	ptr->OnClose(client);
}


CConectSocket::CConectSocket()
{
	qDvcData.clear();
	m_nSplitCount = 0;
	ZeroMemory(m_szSplit, sizeof(m_szSplit));
	m_nActionMode = 0; //NORMAL
	m_bConnect = FALSE;
	m_pClient = nullptr;
	m_nPort = 0;
}

CConectSocket::~CConectSocket()
{

}


// CConectSocket 멤버 함수


void CConectSocket::OnClose(CClientConnection* _client)
{
	m_bConnect = FALSE; //추가
//	AfxMessageBox(_T("ERROR:Disconnected from server!"));
//	::PostQuitMessage(0);
}

void CConectSocket::CloseSocket()
{
	g_iocp.StopComm();
}

void CConectSocket::Send(const void * lpBuf, int nBufLen)
{
	if (m_pClient)
		m_pClient->Send((char*)lpBuf, nBufLen);
}

int CConectSocket::Connect(CString ip, int port)
{
	m_strIp = ip.GetString();
	m_nPort = port;
	g_iocp.Initialize();
	m_pClient = g_iocp.StartClientComm(ip, port);
	if (m_pClient)
	{
		m_pClient->RegisterCallback((CWnd*)this, ::OnReceive, nullptr, ::OnClose);
	}
	m_bConnect = (m_pClient) ? TRUE : FALSE;
	return m_bConnect;
}

int CConectSocket::Reconnect()
{
	if (getIsConnect())
		return 1;
	return Connect(m_strIp, m_nPort);
}

void CConectSocket::ParserMsgID(BYTE* data, RecvData& recvData)
{
	memcpy(&(recvData.m_DataHeader.m_nData), data, sizeof(BYTE)*MAX_MSG_ID);
	recvData.m_DataHeader.m_nData = SWAP32(recvData.m_DataHeader.m_nData);

	recvData.m_nReserved = recvData.m_DataHeader.m_bit.m_nReserved;
	recvData.m_nType = recvData.m_DataHeader.m_bit.m_nType;
	recvData.m_nStationID = recvData.m_DataHeader.m_bit.m_nStationID;
	recvData.m_nSc = recvData.m_DataHeader.m_bit.m_nSc;
	recvData.m_nSm = recvData.m_DataHeader.m_bit.m_nSm;
}


int CConectSocket::GetAction()
{
	int nAction = -1;
	m_cs.Lock();
	if (qDvcData.size() > 0)
	{
		nAction = qDvcData.front();
		qDvcData.pop_front();		
	}
	m_cs.Unlock();

	return nAction;
}

void  CConectSocket::ParserMsgData(BYTE* data, RecvData& recvData, int nRevLength)
{
	m_cs.Lock();

	const int DATA_LEN = 2;
	const int CRC_LEN = 1;
	//추가
	const int DATA_MODE_IDX = 8;
	const int CHAR_TO_INT = 48;
	//
	 
	recvData.m_nDataLen = data[0] << 8;
	recvData.m_nDataLen += data[1];

	int dLen = recvData.m_nDataLen - DATA_LEN - CRC_LEN;
	int nProcessedMsgDataLen = recvData.m_nDataLen;

	if (recvData.m_nType == ACTION)
	{
		recvData.m_nStatus = data[DATA_LEN];
		
		switch (recvData.m_nStatus)
		{
		case  ACTION_START:
			{
			//added
			if (m_nState != STATUS_IDLE)
				break;

				char szForParsing[SERIAL_STR_MAX] = { 0, };
				char szForTPRev[FULLSITES_STR_MAX] = { 0, };
				memcpy(szForParsing, &data[35], sizeof(BYTE[FULLSITES_STR_MAX]));
				memcpy(szForParsing, &data[67], sizeof(BYTE[SERIAL_STR_MAX]));
				std::vector<std::pair<std::string, std::string>> listTesterInfo;

				CParamList PrmList;
				PrmList.m_fnSetDelimiter(";", 1);
				PrmList.m_fnSetDelimitedText(szForParsing, strlen(szForParsing));
				int nCount = PrmList.m_fnGetCount();

				for (int i = 0; i < nCount; ++i) {

					char szName_Version[256] = { 0, };
					PrmList.m_fnGetItemString(i, szName_Version);

					std::pair<std::string, std::string> p;
					p.first = szName_Version;

					listTesterInfo.push_back(p);
				}
				
				memset(m_SendData.m_cSerialNum[0], 0, sizeof(m_SendData.m_cSerialNum[0]));
			    CString strSerialNo =listTesterInfo[0].first.c_str();
				unsigned int nLenth = strSerialNo.GetLength();
				LPCTSTR lpctstr = static_cast<LPCTSTR>(strSerialNo);
				strncpy(m_SendData.m_cSerialNum[0], lpctstr, nLenth);
								
				qDvcData.push_back(ACTION_START);				
			}break;
		case  ACTION_STATUS:
			{
				qDvcData.push_back(ACTION_STATUS);
			}break;
		case ACTION_RESET:
			{
				qDvcData.push_back(ACTION_RESET);
			}break;
			//// added by JJH.
		case ACTION_INFO:    
			{
				qDvcData.push_back(ACTION_INFO);
			}break;
		case ACTION_STOP:
			{
				qDvcData.push_back(ACTION_STOP);
			}break;
		case ACTION_SET_TEST_MODE:
			{
			//added
			if (m_nState != STATUS_IDLE)
				break;

			qDvcData.push_back(ACTION_SET_TEST_MODE);
			 m_nActionMode = data[DATA_MODE_IDX] - CHAR_TO_INT;	// ASKII Unsigned Char -> int형으로 변환키 위함

			}break;
		case ACTION_TEST_SCRIPT:
		{ 
			char szForParsing[SERIAL_STR_MAX] = { 0, };
			memcpy(szForParsing, &data[35], sizeof(recvData.m_cInfoStr));

			std::vector<std::pair<std::string, std::string>> listTesterInfo;

			CParamList PrmList;
			PrmList.m_fnSetDelimiter(";", 1);
			PrmList.m_fnSetDelimitedText(szForParsing, strlen(szForParsing));
			int nCount = PrmList.m_fnGetCount();

			for (int i = 0; i < nCount; ++i) {

				char szName_Version[256] = { 0, };
				PrmList.m_fnGetItemString(i, szName_Version);

				std::pair<std::string, std::string> p;
				p.first = szName_Version;

				listTesterInfo.push_back(p);
			}

// 			memset(m_SendData.m_cSerialNum[0], 0, sizeof(m_SendData.m_cSerialNum[0]));
// 			CString strSerialNo = listTesterInfo[0].first.c_str();
// 			unsigned int nLenth = strSerialNo.GetLength();
// 			LPCTSTR lpctstr = static_cast<LPCTSTR>(strSerialNo);
// 			strncpy(m_SendData.m_cSerialNum[0], lpctstr, nLenth);

			qDvcData.push_back(ACTION_TEST_SCRIPT);
		}break;
			////
		default: break;
		}

		//qDvcData.push_back(ACTION);
	}
	else if (recvData.m_nType == HEART_BEAT)
	{
		qDvcData.push_back(HEART_BEAT);
	}

	m_cs.Unlock();
}

// char* buf = NULL;
// int bytes = _client->GetRcvData(&buf);
// CString str;
// memcpy(str.GetBuffer(bytes / 2), buf, bytes);
// str.ReleaseBuffer();
// _client->ReleaseRcvData(buf);
void CConectSocket::OnReceive(CClientConnection* client)
{
	char sData[40000];
	ZeroMemory(sData, sizeof(sData));

	char* buf;

	int nRdLength = client->GetRcvData(&buf);
	memcpy(sData, buf, nRdLength);
	client->ReleaseRcvData(buf);

	int nRevLength = nRdLength;

	if (m_nSplitCount != 0) {
		memcpy(&m_szSplit[m_nSplitCount], sData, nRdLength);
		memcpy(sData, m_szSplit, nRdLength + m_nSplitCount);
		nRdLength += m_nSplitCount;
		ZeroMemory(m_szSplit, sizeof(m_szSplit));
		m_nSplitCount = 0;
	}

	int nRemainMsgLen = nRdLength;
	char* pStart = &sData[0];

	while (nRemainMsgLen != 0) {
		// Head
		BYTE head[MAX_MSG_ID] = { 0, };
		memcpy((void*)head, pStart, MAX_MSG_ID);

		RecvData* pRcvData = new RecvData;
		ZeroMemory(pRcvData, sizeof(RecvData));
		ParserMsgID(head, *pRcvData);

		int nMsgDataTotalLen = 0;
		nMsgDataTotalLen = *(BYTE*)&pStart[MAX_MSG_ID] << 8;
		nMsgDataTotalLen += *(BYTE*)&pStart[MAX_MSG_ID + 1];

		int nPacketSize = MAX_MSG_ID + nMsgDataTotalLen;
		if (nRemainMsgLen < nPacketSize) {
			memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
			m_nSplitCount += nRemainMsgLen;

			if (pRcvData) { delete pRcvData; pRcvData = NULL; }
			break;
		}

		BYTE data[MSG_MAX] = { 0, };
		memcpy(data, pStart + MAX_MSG_ID, nMsgDataTotalLen);
		int nStatus = data[2];
		
		if (nStatus == 9)
		{
			//memset(data, 0, sizeof(data));
		}

		ParserMsgData(data, *pRcvData, nRevLength);

		if (pRcvData) {
			delete pRcvData;
			pRcvData = NULL;
		}

		pStart += nPacketSize;
		nRemainMsgLen -= nPacketSize;

		if (nRemainMsgLen < MAX_MSG_ID) {
			memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
			m_nSplitCount += nRemainMsgLen;
			break;
		}
	}
}

void CConectSocket::Initialize(int nTesterID)
{
	m_nTesterID = nTesterID;
	m_nState = STATUS_INIT;
	m_strVersion = "1.0.0"; //임의삽입
}

void CConectSocket::MakeSendData(BYTE* data, int action, int SubAction)
{
	SendData SendData;

	if (action == TESTER_REG)
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = action;

		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = REG_STR + 4;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

		// Action   Unregister 경우를 위해 수정. by JJH
		if (SubAction == UNREG_TESTER) 
		{
			data[MAX_MSG_ID + 2] = (BYTE)UNREG_TESTER;
		}
		else  //Register 라면
		{
			data[MAX_MSG_ID + 2] = (BYTE)action;
		}

		CString strData = _T("");
	   //strData.Format("TesterID:FCT%06d;SocketID:SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d;SW_Version:%s;IP:%s;"
	   //	, m_nTesterID
	   //	, m_nTesterID * 100 + 1, m_nTesterID * 100 + 2, m_nTesterID * 100 + 3, m_nTesterID * 100 + 4
	   //	, m_nTesterID * 100 + 5, m_nTesterID * 100 + 6, m_nTesterID * 100 + 7, m_nTesterID * 100 + 8
	   //	, m_strVersion , m_strIp); //Modified by JJH
	   //

		strData.Format(_T("%d;"), m_nTesterID);
		memcpy(&data[MAX_MSG_ID + 3], strData, strData.GetLength());

		// CRC
		data[MAX_MSG_ID + 3 + REG_STR] = CRC;//m_SendData.m_nCRC;
	}
	else if (action == TESTER_STAUS)
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = action;

		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = 4;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

		// Action
		data[MAX_MSG_ID + 2] = (BYTE)SubAction;
		data[MAX_MSG_ID + 3] = CRC;

		m_nState = SubAction;
	}
	else if (action == HEART_BEAT)
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = action;

		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = SEND_HEART_BEAT_DATA_MAX;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

		// TimeStamp
		CTime CurTime = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format("%04d%02d%02d%02d%02d%02d", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());

		int nLen = strTime.GetLength();
		memcpy(&data[MAX_MSG_ID + 2], strTime.GetString(), nLen);

		data[MAX_MSG_ID + 2 + TIME_STAMP_STR] = CRC;
	}
	else if (action == TEST_RESULT)
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = TEST_RESULT;

		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = 3 + RESULT_DATA_STR;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));
		
		//for (int i = 0; i < STATION_MAX_PARA; i++) 소켓 1개당 Packet 1개 
		{
			if (SubAction == FALSE)
			{
				sprintf(m_RecvData.m_cErrorStr[0], "FAIL");
				m_RecvData.m_nReserved1[0] = 2;
			}
			else
			{
				sprintf(m_RecvData.m_cErrorStr[0], "FAIL");
				m_RecvData.m_nReserved1[0] = 2;
// 				sprintf(m_RecvData.m_cErrorStr[0], "PASS");
// 				m_RecvData.m_nReserved1[0] = 1;
			}

				
			sprintf(m_RecvData.m_cDutSn[0], "%s", m_SendData.m_cSerialNum[0]);

	
			//char szData1('1');
			//char szData2('0');

			BYTE bResult[4] = { 0, };
			bResult[0] = (byte)((SubAction >> 24) & 0xFF);
			bResult[1] = (byte)((SubAction >> 16) & 0xFF);
			bResult[2] = (byte)((SubAction >> 8) & 0xFF);
			bResult[3] = (byte)(SubAction & 0xFF);

			bResult[0] = 0;//bResult[3];
						

			memcpy(&data[MAX_MSG_ID + 2], bResult, sizeof(BYTE));
			memcpy(&data[MAX_MSG_ID + 3 ]					     ,&m_RecvData.m_nReserved1[0], sizeof(BYTE)*4);
			int nLenth = strlen(m_RecvData.m_cErrorStr[0]);
			memcpy(&data[MAX_MSG_ID + 7 ]						 ,m_RecvData.m_cErrorStr[0]  , sizeof(BYTE)* nLenth);
			nLenth = strlen(m_RecvData.m_cDutSn[0]);
			memcpy(&data[MAX_MSG_ID + 2  + 5 + ERR_STR]          ,m_RecvData.m_cDutSn[0] 	 , sizeof(BYTE)* nLenth );
		}
		// CRC
		data[MAX_MSG_ID + 2 + RESULT_DATA_STR] = CRC;
	}

	// Added By JJH
	else if(action == TESTER_INFORMATION) 
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = action;
		
		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = TESTER_INFO_STR + 5;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

		CString strData = _T("");
		//strData.Format("TesterID:FCT%06d;SocketID:SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d;SW_Version:1.0.0;IP:192.168.1.1%02d;"
		//	, m_nTesterID
		//	, m_nTesterID * 100 + 1, m_nTesterID * 100 + 2, m_nTesterID * 100 + 3, m_nTesterID * 100 + 4
		//	, m_nTesterID * 100 + 5, m_nTesterID * 100 + 6, m_nTesterID * 100 + 7, m_nTesterID * 100 + 8
		//	, m_nTesterID);

		strData = _T("0;0;1.1.1.49-QQC;Script;"); //HW, FW, SW 버전명을 넘긴다. ; 구분 한다.

		memcpy(&data[MAX_MSG_ID + 4], strData, strData.GetLength());
	
		// CRC
		data[MAX_MSG_ID + 4 + TESTER_INFO_STR] = CRC;
	}
	else if (action == TESTER_SCRIPT)
	{
		SendData.m_DataHeader.m_bit.m_nSc = 1;
		SendData.m_DataHeader.m_bit.m_nSm = 0;
		SendData.m_DataHeader.m_bit.m_nStationID = m_nTesterID;
		SendData.m_DataHeader.m_bit.m_nType = action;

		unsigned int head = SWAP32(SendData.m_DataHeader.m_nData);
		memcpy(data, &head, sizeof(BYTE)*MAX_MSG_ID);

		// Len
		SendData.m_nDataLen = TESTER_INFO_STR + 5;
		unsigned short len = SWAP16(SendData.m_nDataLen);
		memcpy(&data[MAX_MSG_ID], &len, sizeof(unsigned short));

		CString strData = _T("");
		//strData.Format("TesterID:FCT%06d;SocketID:SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d,SID%03d;SW_Version:1.0.0;IP:192.168.1.1%02d;"
		//	, m_nTesterID
		//	, m_nTesterID * 100 + 1, m_nTesterID * 100 + 2, m_nTesterID * 100 + 3, m_nTesterID * 100 + 4
		//	, m_nTesterID * 100 + 5, m_nTesterID * 100 + 6, m_nTesterID * 100 + 7, m_nTesterID * 100 + 8
		//	, m_nTesterID);

		strData = _T("APL_INFO;"); //HW, FW, SW 버전명을 넘긴다. ; 구분 한다.

		memcpy(&data[MAX_MSG_ID + 4], strData, strData.GetLength());

		// CRC
		data[MAX_MSG_ID + 4 + TESTER_INFO_STR] = CRC;
	}
}
