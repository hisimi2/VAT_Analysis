#include "StdAfx.h"
#include "SankyoDrv_Imp.h"
#include "DlgSankyoManual.h"
#include "lib_crc.h"
#include <algorithm>

IMPLEMENT_DYNAMIC(CSankyoDrv_Imp, CWnd)


CSankyoDrv_Imp::CSankyoDrv_Imp(void)
{
	m_pWinRs232c = NULL;
	m_nSelectedOperatorAction = Sankyo::eOpActionNoAction;
	m_nStateMain = 0;
	::InitializeCriticalSection(&m_csRcvMsg);
	::InitializeCriticalSection(&m_csLogObserver);
	// 통신과 연결이 되지 않더라도 32개의 station data 저장소를 만들어 둔다.
	for(int i=0; i<Sankyo::MAX_STATION_COUNT; i++){
		m_vData.push_back(Sankyo::ST_SANKYO_DATA());
	}
	m_pDlgSimul = NULL;
	ZeroMemory(m_szSplit, sizeof(m_szSplit));
	m_nSplitCount = 0;
	ZeroMemory(m_szRcvBuff, sizeof(m_szRcvBuff));
	m_nRunMode = 0;
	m_nSendRetryCount = 0;
	m_nMaxSendRetryCount = 2;
	m_dStartTime = (double)::GetTickCount64();
	m_nReplyTimeoutRetryCount = 0;
	m_nMaxReplyTimeoutRetryCount = 2;
	m_nSelectedOperatorAction = -1;
	m_nTagNo = 0;
	m_bRunningHeartBit = FALSE;
	m_dwRcvLen = 0;
	m_bReceiveReply = false;

}


CSankyoDrv_Imp::~CSankyoDrv_Imp(void)
{
	CloseThread();
	::DeleteCriticalSection(&m_csRcvMsg);
	::DeleteCriticalSection(&m_csLogObserver);
	if( m_pDlgSimul ){
		m_pDlgSimul->DestroyWindow();
		delete m_pDlgSimul;
		m_pDlgSimul = NULL;
	}
	if( m_pWinRs232c ){
		m_pWinRs232c->Close();
		delete m_pWinRs232c;
		m_pWinRs232c = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSankyoDrv_Imp, CWnd)
END_MESSAGE_MAP()



int CSankyoDrv_Imp::Initialize(Sankyo::ST_SANKYO_INIT_INFO stInitInfo)
{
	m_stInitInfo = stInitInfo;
	if( !stInitInfo.bSimulMode )
	{
		if( m_pWinRs232c ){
			delete m_pWinRs232c;
			m_pWinRs232c = NULL;
		}
		m_pWinRs232c = new CWinRs232c;

		if( !m_pWinRs232c->Create(this) ){
			AfxMessageBox("Sankyo Motor Comport open fail!!!");
			return sankyo_err_port_open_fail;
		}
		BOOL bOpenSuccess = m_pWinRs232c->m_fnInitial(stInitInfo.nPort, 
			stInitInfo.nRate, 
			stInitInfo.nParity, 
			stInitInfo.nData, 
			stInitInfo.nStop, 
			stInitInfo.szLogFileName, 
			stInitInfo.nLogKeepPeriod, 
			cbReceiveEvent);
		if( !bOpenSuccess ){
			AfxMessageBox("Sankyo Motor Comport open fail!!!");
			return sankyo_err_port_open_fail;
		}
	}
	else
	{
		m_pDlgSimul = new CDlgSankyoSimul(this);
		m_pDlgSimul->Create(IDD_SANKYO_SIMUL, this);
		m_pDlgSimul->ShowWindow(SW_SHOW);
	}
	CreateThread(0);
	return sankyo_err_no_error;
}

void CSankyoDrv_Imp::Finalize()
{
	CloseThread();
	if( m_pWinRs232c ){
		m_pWinRs232c->Close();
		delete m_pWinRs232c;
		m_pWinRs232c = NULL;
	}
}

BOOL CSankyoDrv_Imp::IsConnected()
{
	if( m_pWinRs232c == NULL ){
		return FALSE;
	}
	return m_pWinRs232c->IsConnected();
}

BOOL CSankyoDrv_Imp::IsAutoRunning()
{
	return (m_nStateMain != eComm_Manual_Idle);
}

// nStation : 1~32
double CSankyoDrv_Imp::GetTorque(int nStation)
{
	ASSERT(nStation>0 && nStation<=Sankyo::MAX_STATION_COUNT);
	return m_vData[nStation-1].dTorque;
}

// 32개의 motor driver에 대한
void CSankyoDrv_Imp::GetTorqueAll(std::vector<double>& vCont)
{
	for( std::vector<Sankyo::ST_SANKYO_DATA>::size_type i=0; i<m_vData.size(); i++){
		vCont.push_back(m_vData[i].dTorque);
	}
}

// set-up
// nStation : 1~32
int CSankyoDrv_Imp::Setup_Subscription(int nStation, bool bEnable)
{
	ASSERT(nStation>0 && nStation<=Sankyo::MAX_STATION_COUNT);

	if( m_nStateMain != eComm_Manual_Idle ){
		return sankyo_err_op_mode;
	}
	std::list<int>::iterator it = std::find(std::begin(m_vSubscriptedStationNumber), std::end(m_vSubscriptedStationNumber), nStation);
	// list에 요소가 있다면
	if( it != std::end(m_vSubscriptedStationNumber)){
		if( !bEnable ){
			m_vSubscriptedStationNumber.remove(nStation);
			m_vData[nStation-1].dTorque = 999.0;
		}
	}else{
		if( bEnable ){
			m_vSubscriptedStationNumber.push_back(nStation);
		}
	}
	return sankyo_err_no_error;
}

int CSankyoDrv_Imp::Setup_Subscription(std::list<int> vSubscriptionList)
{
	if( m_nStateMain != eComm_Manual_Idle ){
		return sankyo_err_op_mode;
	}
	m_vSubscriptedStationNumber = vSubscriptionList;
	for(int i=0; i<Sankyo::MAX_STATION_COUNT; i++){
		std::list<int>::iterator it = std::find(std::begin(m_vSubscriptedStationNumber), std::end(m_vSubscriptedStationNumber), i+1);
		if( it == std::end(m_vSubscriptedStationNumber)){
			m_vData[i].dTorque = 999.0;
		}
	}
	return sankyo_err_no_error;
}

bool CSankyoDrv_Imp::Setup_GetSubscription(int nStation)
{
	std::list<int>::iterator it = std::find(std::begin(m_vSubscriptedStationNumber), std::end(m_vSubscriptedStationNumber), nStation);
	if( it != std::end(m_vSubscriptedStationNumber)){
		return true;
	}
	return false;
}

void CSankyoDrv_Imp::Setup_GetSubscriptionAll(std::list<int>& vSubscriptedStation)
{
	vSubscriptedStation = m_vSubscriptedStationNumber;
}

// Work Procedure
void CSankyoDrv_Imp::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while(GetThreadAliveFlag(0))
	{
		Sleep(1);
		if( GetPauseFlag(0) ) continue;

		if( m_nRunMode == Sankyo::eRunMode_Manual ){
			for( std::vector<Sankyo::ST_SANKYO_DATA>::size_type i=0; i<m_vData.size(); i++){
				m_vData[i].dTorque = 999.0;
			}
			m_nRunMode = Sankyo::eRunMode_None;
			m_nStateMain = eComm_Manual_Idle;
		}

		switch( m_nStateMain )
		{
		case eComm_Manual_Idle:
			{
				if( m_nRunMode == Sankyo::eRunMode_Auto){
					m_nRunMode = Sankyo::eRunMode_None;
					m_nStateMain = ePreSend_1000;
					break;
				}
				Sleep(500);
			}break;

		case ePreSend_1000:
			{
				m_vStationNumTemp = m_vSubscriptedStationNumber;
				m_nStateMain = eSend_0000;
			}break;

		case eSend_0000:
			{
				m_nSendRetryCount = 0;
				m_nReplyTimeoutRetryCount = 0;
				m_nStateMain = eSend_1000;
			}break;

		case eSend_1000:
			{
				m_bRunningHeartBit = !m_bRunningHeartBit;
				if( m_vStationNumTemp.empty() ){
					m_nStateMain = ePreSend_1000;
					break;
				}
				std::vector<unsigned short> sParam;
				sParam.push_back(Sankyo::_eStsCode_CommandTorque);
				int nSendDataLen = SendPacket(m_vStationNumTemp.back(), Sankyo::_eCmd_Code_GetStateValue2, sParam);
				if(0)// nSendDataLen == 0 ){
				{
					if(m_nSendRetryCount < m_nMaxSendRetryCount){
						m_nSendRetryCount++;
					}else{
						m_nSendRetryCount = 0;
						m_nStateMain = eCommSendFail_1000;
					}
				}else{
					m_dStartTime = (double)::GetTickCount64();
					m_nSendRetryCount = 0;
					m_nStateMain = eWaitReply;
				}
			}break;

		case eWaitReply:
			{
				double dSpanTime = (double)::GetTickCount64() - m_dStartTime;
				if( dSpanTime > 1000 ){
					m_nStateMain = eReplyTimeout_1000;
					break;
				}
				if( CheckReceiveReply() ){
					m_vStationNumTemp.pop_back();
					// Sankyo 드라이버에서 처리 할 수 있는 최소 통신 주기 만큼 Sleep을 줘야 할 수도 있음.
					// 테스트 후 결정
					//Sleep(5);
					m_nStateMain = eSend_1000;
				}
			}break;

		case eReplyTimeout_1000:
			{
				if( m_nReplyTimeoutRetryCount >= m_nMaxReplyTimeoutRetryCount ){
					m_nStateMain = eReplyTimeout_Wait_Retry_Skip;
					m_nReplyTimeoutRetryCount = 0;
					break;
				}
				m_nReplyTimeoutRetryCount++;
				m_nStateMain = eSend_1000;
				break;
			}break;

		case eReplyTimeout_Wait_Retry_Skip:
			{
				int nSelectedOpAction = CheckOpAction();
				if( nSelectedOpAction == Sankyo::eOpActionRetry ){
					m_nReplyTimeoutRetryCount = 0;
					m_nStateMain = eSend_1000;
				}else if( nSelectedOpAction == Sankyo::eOpActionSkip ){
					m_nReplyTimeoutRetryCount = 0;
					m_vStationNumTemp.pop_back();
					m_nStateMain = eSend_1000;
				}else{ // no action
					// wait operator
				}
			}break;

		case eCommSendFail_1000:
			{
				// Report Alarm
				m_nStateMain = eCommSendFail_1100;
			}break;

			// Delay를 주었다가 다음 패킷 보낸다.
		case eCommSendFail_1100:
			{
				Sleep(500);
				m_vStationNumTemp.pop_back();
				m_nStateMain = eSend_1000;
			}break;
		}
	}
}

void CSankyoDrv_Imp::cbReceiveEvent(CWnd* pWnd, LPARAM lParam, WPARAM wParam)
{
	((CSankyoDrv_Imp*)pWnd)->OnReceivePacket(lParam, wParam);
}

/*=====================================================================================
*
*  RETURN :Send Byte
=====================================================================================*/
int CSankyoDrv_Imp::SendPacket(int nStation, int nCommand, std::vector<unsigned short> sParam)
{
	ASSERT(nStation>0 && nStation<=Sankyo::MAX_STATION_COUNT);

	if( m_stInitInfo.bSimulMode ){
		m_vData[nStation-1].dTorque = m_pDlgSimul->GetTorque(nStation);
		m_bReceiveReply = true;
		return 1;
	}

	if( m_pWinRs232c == NULL ){
		return 0;
	}

	if( !m_pWinRs232c->IsConnected() ){
		return 0;
	}

	//------------------------------- 
	// make packet
	//------------------------------- 
	struct Sankyo::_ST_PACKET packet;
	int nDataLen = 0;	
	//static bool bToggle = false;

	// data
	packet.data.union_control_code.stCtrlCode._bfDir = Sankyo::_eMsgDir_Command;
	//packet.data.union_control_code.stCtrlCode._bfToggle = !bToggle;
	packet.data.union_control_code.stCtrlCode._bfToggle = true;
	packet.data._ucCommand_Code = nCommand;
	// header
	nDataLen = Sankyo::LEN_DATA_CONTROL + Sankyo::LEN_DATA_COMMAND;
	if( !sParam.empty() ){
		// Parameter 길이 수 만큼 더한다.
		nDataLen += sizeof(unsigned short)*sParam.size();
	}	 
	packet.header.set(nStation, nDataLen);

	//------------------------------- 
	// 
	//------------------------------- 
	int nLen_without_crc = sizeof(Sankyo::_ST_HEADER)+nDataLen;
	unsigned char szData[1024]={0,};
	szData[0] = packet.header.union_protocol_header._ucPH;
	szData[1] = packet.header._ucAddress;
	szData[2] = packet.data.union_control_code._ucCtrl_Code;
	szData[3] = packet.data._ucCommand_Code;
	// command에 따라서 Parameter의 길이가 달라지므로 추후 기능 확장을 위해서는 이 부분 수정 필요. >>>
	if( !sParam.empty() )
	{
		for(std::vector<unsigned char>::size_type i=0; i<sParam.size(); i++){
			// 2 byte를 쓰는데 상위/하위 바뀌지 않음.
			szData[4+(i*2)+0] = (sParam[i] & 0xFF00) >> 8;
			szData[4+(i*2)+1] = (sParam[i] & 0x00FF);
		}
	}
	// command에 따라서 Parameter의 길이가 달라지 므로 추후 기능 확장을 위해서는 이 부분 수정 필요. <<<

	unsigned short crc16 = 0xffff; // initial value
	for(int i=0; i<nLen_without_crc; i++){
		crc16 = update_crc_ccitt(crc16, szData[i]);
	}
	// 2 byte를 쓰는데 상위/하위 바뀌지 않음. *(unsigned char*)&szData[0] = crc; <- 이렇게 하면 안됨.
	szData[nLen_without_crc+0] = (crc16 & 0xFF00) >> 8;
	szData[nLen_without_crc+1] = crc16 & 0x00FF;

	int nRet = m_pWinRs232c->Send(szData, nLen_without_crc + Sankyo::LEN_FCS);

	char szParamCode[256]={0,};
	if(!sParam.empty()){
		for(std::vector<unsigned char>::size_type i=0; i<sParam.size(); i++){
			char szTemp[32]={0,};
			sprintf_s(szTemp, sizeof(szTemp), "%X", sParam[i]);
			strcat(szParamCode, szTemp);
		}
	}

	BroadcastLogView("[SND] [STATION:%d] command=%X, param code=%s", nStation, nCommand, szParamCode );
	m_bReceiveReply = false;

	return nRet;
}

bool CSankyoDrv_Imp::CheckReceiveReply()
{
	bool bRet = false;
	if( m_bReceiveReply ){
		bRet = m_bReceiveReply;
		m_bReceiveReply = false;
	}
	return bRet;
}

void CSankyoDrv_Imp::Op_Action(int nAction)
{
	m_nSelectedOperatorAction = nAction;
}

void CSankyoDrv_Imp::Op_RunMode(int nRunMode)
{
	m_nRunMode = nRunMode;
}

int CSankyoDrv_Imp::CheckOpAction()
{
	int nRet = Sankyo::eOpActionNoAction;
	if( m_nSelectedOperatorAction != Sankyo::eOpActionNoAction ){
		nRet = m_nSelectedOperatorAction;
		m_nSelectedOperatorAction = Sankyo::eOpActionNoAction;
	}
	return nRet;
}

void CSankyoDrv_Imp::OnReceivePacket(LPARAM lParam, WPARAM wParam)
{
	::EnterCriticalSection(&m_csRcvMsg);

	int i=0;
	DWORD dwRemainLength=0; 
	char szTemp[1024]={0};
	char* pStart = NULL;
	m_dwRcvLen = (DWORD)wParam;

	memset(m_szRcvBuff, 0x00, sizeof(m_szRcvBuff));
	memcpy(m_szRcvBuff, (char*)lParam, m_dwRcvLen);

	// 메세지가 잘린 경우 처리
	if( m_nSplitCount != 0 ){
		memcpy(&m_szSplit[m_nSplitCount], m_szRcvBuff, m_dwRcvLen);
		memcpy(m_szRcvBuff, m_szSplit, m_nSplitCount + m_dwRcvLen );
		m_dwRcvLen += m_nSplitCount;
		memset(m_szSplit, 0, sizeof(m_szSplit));
		m_nSplitCount = 0;
	}

	//TRACE("receive, length=%d\n", m_dwRcvLen);

	dwRemainLength = m_dwRcvLen;
	pStart = m_szRcvBuff;
	while( dwRemainLength != 0 )
	{
		WORD wLen = 0;
		struct Sankyo::_ST_HEADER packet_header;
		memcpy(&packet_header, pStart, sizeof(Sankyo::_ST_HEADER));
		// 남은 Message의 길이가 1개의 Packet 길이보다 짧으면 Split에 저장해 둔다.
		int nDataLen = packet_header.get_data_len();
		if( dwRemainLength < nDataLen + sizeof(Sankyo::_ST_HEADER)+ sizeof(Sankyo::_ST_CRC)){
			memcpy(m_szSplit, (void*)pStart, dwRemainLength);
			m_nSplitCount = dwRemainLength;
			dwRemainLength = 0;
		}
		else
		{
			// check crc
			unsigned short crc16 = 0xffff; // initial value
			int nLen_without_crc = nDataLen + sizeof(Sankyo::_ST_HEADER);
			for(int i=0; i<nLen_without_crc; i++){
				crc16 = update_crc_ccitt(crc16, pStart[i]);
			}
			// crc가 맞지 않는다면 노이즈로 본다.
			union Sankyo::_SHORT_SWAP unionCRC(pStart[nLen_without_crc], pStart[nLen_without_crc+1]);
			if(crc16 == unionCRC.sValue )
			{
				struct Sankyo::_ST_DATA packet_data;
				memcpy(&packet_data, (pStart+sizeof(Sankyo::_ST_HEADER)), nDataLen);
				switch( packet_data.get_command_code() )
				{
				case Sankyo::_eCmd_Code_GetStateValue2:
					{
						char* pValue = pStart+sizeof(Sankyo::_ST_HEADER)+Sankyo::LEN_DATA_COMMAND+Sankyo::LEN_DATA_CONTROL;
						union Sankyo::_SHORT_SWAP unionValue(pValue[0], pValue[1]);
						int nStation = packet_header.get_address();
						if( nStation>0 && nStation<=Sankyo::MAX_STATION_COUNT ){
							m_vData[nStation-1].dTorque = (double)unionValue.sValue;
						}
						BroadcastLogView("[RCV] [STATION:%d] GetStateValue2, VALUE:%d", nStation, unionValue.sValue);
					}break;
				}
			}
			pStart += nDataLen+sizeof(Sankyo::_ST_HEADER)+sizeof(Sankyo::_ST_CRC);
			dwRemainLength -= nDataLen+sizeof(Sankyo::_ST_HEADER)+sizeof(Sankyo::_ST_CRC);
			m_bReceiveReply = true;
		}
	}

	::LeaveCriticalSection(&m_csRcvMsg);
}


void CSankyoDrv_Imp::ShowManualDialog()
{
	CDlgSankyoManual cDlgManual(this);
	cDlgManual.DoModal();
}

void CSankyoDrv_Imp::AddCommLogView(HWND hWnd)
{
	::EnterCriticalSection(&m_csLogObserver);
	m_vLogObserverList.push_back(hWnd);
	::LeaveCriticalSection(&m_csLogObserver);
}

void CSankyoDrv_Imp::RemoveLogView(HWND hWnd)
{
	::EnterCriticalSection(&m_csLogObserver);
	std::remove(std::begin(m_vLogObserverList), std::end(m_vLogObserverList), hWnd);
	::LeaveCriticalSection(&m_csLogObserver);
}

void CSankyoDrv_Imp::ClearLogView()
{
	::EnterCriticalSection(&m_csLogObserver);
	m_vLogObserverList.clear();
	::LeaveCriticalSection(&m_csLogObserver);
}

int CSankyoDrv_Imp::GetLogViewCount()
{
	::EnterCriticalSection(&m_csLogObserver);
	if( !m_vLogObserverList.empty() ){
		return (int)m_vLogObserverList.size();
	}
	return 0;
	::LeaveCriticalSection(&m_csLogObserver);
}

void CSankyoDrv_Imp::BroadcastLogView(char* szAscii, int nLen)
{
	::EnterCriticalSection(&m_csLogObserver);

	if( !m_vLogObserverList.empty() ){
		for(std::vector<HWND>::size_type i=0; i<m_vLogObserverList.size(); i++){
			::SendMessage(m_vLogObserverList[i], UM_SANKYO_LOG, (WPARAM)nLen, (LPARAM)szAscii);
		}
	}

	::LeaveCriticalSection(&m_csLogObserver);
}

void CSankyoDrv_Imp::BroadcastLogView(const char* fmt, ...)
{
	char szEventMessage[1024]={0,};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);
	BroadcastLogView(szEventMessage, strlen(szEventMessage));
}