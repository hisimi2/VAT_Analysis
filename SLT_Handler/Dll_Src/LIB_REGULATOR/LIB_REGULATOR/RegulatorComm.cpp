// RegulatorComm.cpp: implementation of the CRegulatorComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigData.h"
#include "DEF_ERROR_CODE.h"
#include "RegulatorComm.h"
#include <stdarg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// #define SET		0
// #define INC		1
// #define DEC		2
// #define REQ		3
// #define MON		4 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const double dSpecValue = 0.9;	// Model : ITV1050-RC2N
const int nDecResolution = 1023;

CRegulatorComm::CRegulatorComm()
{
	m_nCommand						= -1;
	m_nState						= -1;
	m_dMPa							= 0;
	m_bConnected					= FALSE;
	m_dwRxCnt					    = 0;
	m_dSettingPressure		        = 0;

	m_pParentWnd = NULL;
	memset(m_szRxBuff, 0, sizeof(m_szRxBuff));

	m_bCommandLock = false;
}

CRegulatorComm::~CRegulatorComm()
{
}

BOOL CRegulatorComm::InitRs232c(CWnd* pParentWnd, ST_REGUL_DATA &stSetData, char* szLogFile/*, FN_PARENT_CALLBACK cbAlarm*/)
{
	m_pParentWnd = pParentWnd;
	//m_cbAlarm = cbAlarm;

	BOOL bCreated = m_cWrs232C.Create(this);
	
	if(bCreated != FALSE)
	{
		if(!m_cWrs232C.m_fnInitial(stSetData.nPort, stSetData.dwRate, stSetData.btParity, stSetData.btSize, stSetData.btStop, szLogFile, 0, CRegulatorComm::cbReceive))
		{
			return FALSE;
		}
		else
		{
			CreateThread(0);
			m_bConnected = TRUE;
		}
	}

	return TRUE;
}

void CRegulatorComm::Close()
{
	SetThreadAliveFlag(0, FALSE);
	CloseThread(0);
}

void CRegulatorComm::SetCommand(int nCommand, double dMPa)
{	
	int	nSettingValue		= 0;
	//CString strSendMsg		= _T("");
	char szData[1024]={0,};

	if(!m_bConnected){
		return;
	}

	if( dMPa != 0.0 ){
		// 0.3MPa = (0.3MPa/0.9MPa)x1023 = 34.1(DEC)
		nSettingValue = (int)((dMPa / dSpecValue) * (double)nDecResolution);
	}

	switch ( nCommand )
	{
	case SET:
		sprintf_s(szData, sizeof(szData), "SET %d\r\n", nSettingValue);
		//strSendMsg.Format(_T("SET %d\r\n"), nSettingValue);
		break;

	case INC:
		sprintf_s(szData, sizeof(szData), "INC\r\n");
		//strSendMsg = _T("INC\r\n");
		break;

	case DEC:
		sprintf_s(szData, sizeof(szData), "DEC\r\n");
		//strSendMsg = _T("DEC\r\n");
		break;

	case REQ:
		sprintf_s(szData, sizeof(szData), "REQ\r\n");
		//strSendMsg = _T("REQ\r\n");
		break;

	case MON:
		sprintf_s(szData, sizeof(szData), "MON\r\n");
		//strSendMsg = _T("MON\r\n");
		break;

	default:
		sprintf_s(szData, sizeof(szData), "MON\r\n");
		//strSendMsg = _T("MON\r\n");
		break;
	}

	m_cWrs232C.Send(szData/*(LPSTR)(LPCTSTR)strSendMsg*/, strlen(szData)/*strSendMsg.GetLength()*/);

	// Save command
	m_nCommand = nCommand;
}

void CRegulatorComm::SetRecvData(int nState, int nMPa)
{
	m_nState = nState;

	// x MPa = (1023(DEC) * 0.9MPa) / 1023 = 0.9 MPa
	m_dMPa = ((double)nMPa * dSpecValue) / (double)nDecResolution;
}

void CRegulatorComm::GetRecvData(int *nState, double *dMPa)
{
	if( dMPa != NULL)
		*dMPa = m_dMPa;

	if( nState != NULL)
		*nState = m_nState;
}

double CRegulatorComm::GetSettingMpa()
{
	return m_dSettingPressure;
}

int CRegulatorComm::GetDecFromMpa(double dMpa)
{
	int nRes = int((dMpa * (double)nDecResolution) / dSpecValue);

	return nRes;
}

BOOL CRegulatorComm::RegulatorPressCheck(double dRegulatorPermit)
{
	double dSettingValue = GetSettingMpa();

	// 단위 변경
	double dSet		= dSettingValue;				// 설정값
	double dState	= m_dMPa;						// 현재값
	double dHighLimitValue = 0.0;
	double dLowLimitValue  = 0.0;

	if(0 < dRegulatorPermit)
	{
		double dPercentage = dRegulatorPermit / 100.0;
		dHighLimitValue = dSet + dSet * dPercentage;
		dLowLimitValue  = dSet - dSet * dPercentage;
	}
	else
	{
		dHighLimitValue = dSet * 1.05; // 5% 높은지 체크
		dLowLimitValue  = dSet * 0.95; // 5% 낮은지 체크
	}

	if( dSet > 0 &&  (dState < dLowLimitValue || dState > dHighLimitValue))	// 퍼센티지로 압력 차이 비교
	{
		return FALSE;
	}

	return TRUE;
}

void CRegulatorComm::ThreadProc1()
{
	SetThreadInterval(0, 50);

	while( GetThreadAliveFlag(0) )
	{
		if( GetPauseFlag(0) )
		{ 			
			continue;
		}
		
		if (true != GetCommandLock())
		{
			SetCommand(MON);				// 전공레귤레이터 압력값을 받아온다.
		}

		Sleep(100);
	}
}

void CRegulatorComm::SetRegulator(double dSetPressure)
{
	if( !m_bConnected)
	{
		return;
	}
	
	// Lock 이라면 이미 SET커맨드를 보내고 Receive를 기다리는 상태이다.
	if (true == GetCommandLock()) {
		SetCommandLock(false);
	}

	m_dSettingPressure = dSetPressure;

	// MON 커맨드를 보내지 못하게 Lock을 건다.
	SetCommandLock(true);

	SetCommand(SET, dSetPressure);
}

void CRegulatorComm::cbReceive(CWnd* pWnd, LPARAM lParam, WPARAM wParam)
{
	CRegulatorComm* pSelf = (CRegulatorComm*)pWnd;
	pSelf->OnReceive((char*)lParam, wParam);
}

void CRegulatorComm::OnReceive(char* lParam, WPARAM wParam)
{
	// check UNKNOWN_COMMAND
	m_dwRxCnt = (DWORD)wParam;
	memset(m_szRxBuff, 0, sizeof(m_szRxBuff));

	// Receive Data Copy
	memcpy(m_szRxBuff, lParam, m_dwRxCnt);

	if( strstr(m_szRxBuff, "UNKNOWN COMMAND") != NULL )
	{
		SetRecvData(UNKNOWN_COMMAND, 0);

		// Unknown Command Error
		//m_cbAlarm(m_pParentWnd, (WPARAM)ERR_REGULATOR_UNKNOWN_COMMAND, NULL);

		return ;
	}

	// check OUT_OF_RANGE
	int	nRecvData = atoi(m_szRxBuff);
	if( nRecvData >= MAX_BUFF )
	{
		SetRecvData(OUT_OF_RANGE, 0);

		// Out of Range Error
		//m_cbAlarm(m_pParentWnd, (WPARAM)ERR_REGULATOR_OUT_OF_RANGE, NULL);

		return;
	}

	// Check data
	int nCommand = GetCommand();
	switch ( nCommand )
	{
	case SET:
		if( nRecvData >= 0 && nRecvData <= nDecResolution )
		{
			// SET Value 그대로 Receive가 된다.
			SetCommandLock(false);
		}
		break;

	case INC:
	case DEC:
		if( nRecvData >= 0 && nRecvData <= nDecResolution )
		{
			SetRecvData(SET_MM, nRecvData);
		}
		break;

	case REQ:
	case MON:
		if( nRecvData >= 0 && nRecvData <= nDecResolution )
		{
			SetRecvData(SET_NN, nRecvData);
		}
		break;

	default:
		{
			// Unknown Response Error
			//m_cbAlarm(m_pParentWnd, (WPARAM)ERR_REGULATOR_UNKNOWN_RESPONSE, NULL);
		}
		break;
	}
}
