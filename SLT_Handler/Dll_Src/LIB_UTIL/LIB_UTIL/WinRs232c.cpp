// WinRs232c.cpp : implementation file

#include "stdafx.h"
#include "WinRs232c.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SEMAPHORE_NAME										"RS232C_SEND_SEMA"

#define	ASCII_XON	0x11
#define	ASCII_XOFF	0x13


//***************************************************************************
// Thread 선언

// 데이터를 수신하는 Thread
UINT Receive232Thread(LPVOID wndParam);


/////////////////////////////////////////////////////////////////////////////
// CWinRs232c

/*========================================================================================================

========================================================================================================*/
CWinRs232c::CWinRs232c()
{
	m_pParent = NULL;
	m_Data = NULL;
	Receive232WinThread = NULL;
	m_hFModuleSemaphore = NULL;


	m_ioRead.hEvent = nullptr;
	m_ioWrite.hEvent = nullptr;
	m_Handle = NULL;
	m_nRcvMaxLen = 0;
	m_nSndMaxLen= 0;
	m_RcvLen =0;
}

/*========================================================================================================

========================================================================================================*/
CWinRs232c::~CWinRs232c()
{
	Close();
	if (m_Data)	delete m_Data;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

/*========================================================================================================

========================================================================================================*/
BOOL CWinRs232c::Create(CWnd* pParent, DWORD nRcvMaxLen, DWORD nSndMaxLen)
{
	m_ioRead.Offset		= 0;
	m_ioRead.OffsetHigh = 0;
	m_ioWrite.Offset	= 0;
	m_ioWrite.OffsetHigh= 0;

	if (m_ioRead.hEvent != nullptr) {
		::CloseHandle(m_ioRead.hEvent);
		m_ioRead.hEvent = nullptr;
	}

	if (m_ioWrite.hEvent != nullptr) {
		::CloseHandle(m_ioWrite.hEvent);
		m_ioWrite.hEvent = nullptr;
	}

	m_ioRead.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_ioRead.hEvent)
	{
		m_ioWrite.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_ioWrite.hEvent)
		{
			m_pParent = pParent;
			m_nRcvMaxLen = nRcvMaxLen;		//수신버퍼..수정..
			m_nSndMaxLen = nSndMaxLen;		//송신버퍼 ..수정..
			m_Data = new BYTE[nRcvMaxLen + 1];

			return TRUE;
		}

		::CloseHandle(m_ioRead.hEvent);
	}
	return FALSE;
}

/*========================================================================================================

========================================================================================================*/
BOOL CWinRs232c::Open(CString strPort, DWORD nRate, BYTE nParity, BYTE nSize, BYTE nStop)
{
	if (Receive232WinThread)	return FALSE;

	m_Handle = ::CreateFile(strPort, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL);
	if (m_Handle == HANDLE(-1))
		return FALSE;
	if (::SetCommMask(m_Handle, EV_RXCHAR) == FALSE)
		return FALSE;

	if (::SetupComm(m_Handle, m_nRcvMaxLen, m_nSndMaxLen) == FALSE)
		return FALSE;

	if (::PurgeComm(m_Handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
		return FALSE;

	COMMTIMEOUTS	CommTimeouts = { 0xFFFFFFFFL, 0, 1000, 0, 1000 };
	if (::SetCommTimeouts(m_Handle, &CommTimeouts) == FALSE)
		return FALSE;
	CommTimeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant =0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600/nRate;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(m_Handle, &CommTimeouts);

	DCB	Dcb;

	Dcb.DCBlength = sizeof(Dcb);
	if (::GetCommState(m_Handle, &Dcb) == FALSE)
		return FALSE;

	Dcb.BaudRate = nRate;
	Dcb.ByteSize = nSize;
	Dcb.StopBits = nStop;
	Dcb.Parity	 = nParity;

	Dcb.fOutxDsrFlow = 0;
	//Dcb.fDtrControl	 = DTR_CONTROL_DISABLE;
	//Dcb.fDtrControl	 = DTR_CONTROL_ENABLE;
	Dcb.fOutxCtsFlow = 0;
	Dcb.fRtsControl	 = RTS_CONTROL_ENABLE;
	//Dcb.fInX	 = 1;
	//Dcb.fOutX	 = 1;
	//Dcb.XonChar	 = ASCII_XON;
	//Dcb.XoffChar = ASCII_XOFF;
	Dcb.XonLim	 = 100;
	Dcb.XoffLim	 = 100;
	//Dcb.fBinary	 = TRUE;
	Dcb.fParity	 = TRUE;

	if (::SetCommState(m_Handle, &Dcb) == FALSE)
	{
		int nErr = ::GetLastError();
		::CloseHandle(m_Handle);
		return FALSE;
	}

	Receive232WinThread = ::AfxBeginThread((AFX_THREADPROC)Receive232Thread, LPVOID(this), THREAD_PRIORITY_NORMAL);

	return Receive232WinThread != NULL;
}

/*========================================================================================================

========================================================================================================*/
void CWinRs232c::Close()
{
//	m_Logger.SetLogString("COM Close");

	if(m_hFModuleSemaphore != NULL)
	{
		CloseHandle(m_hFModuleSemaphore);
		m_hFModuleSemaphore = NULL;
	}
	if (Receive232WinThread)
	{
		if (::TerminateThread(Receive232WinThread->m_hThread, 0) == FALSE)
			return;
		Receive232WinThread = NULL;

		if (::SetCommMask(m_Handle, 0 ) == FALSE)
			return;
		if (::EscapeCommFunction(m_Handle, CLRDTR ) == FALSE)
			return;
		if (::PurgeComm(m_Handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) == FALSE)
			return;
		if (::CloseHandle(m_Handle) == FALSE)
			return;
	}
}

//========================================================================================================
//
//========================================================================================================
BOOL CWinRs232c::isOpen()
{
	return Receive232WinThread != NULL;
}

/*========================================================================================================

========================================================================================================*/
DWORD CWinRs232c::Receive(LPVOID lpData)
{
//	CString strData;
	memcpy(lpData, m_Data, m_RcvLen);
	
// 	strData.Format("[RCV] %s : Length %d", lpData, m_RcvLen);
// 	m_Logger.SetLogString(strData);

	return m_RcvLen;
}

/*========================================================================================================

========================================================================================================*/
BOOL CWinRs232c::Send(LPVOID lpData, DWORD nLength)
{
	BOOL bRet = FALSE;
//	CString strData;

	if(WaitForSingleObject(m_hFModuleSemaphore, INFINITE) != WAIT_FAILED) {
		bRet = ::WriteFile(m_Handle, lpData, nLength, &nLength, &m_ioWrite);
//		strData.Format("[SND] %s : Success %d", lpData, bRet);
//		m_Logger.SetLogString(strData);
		ReleaseSemaphore(m_hFModuleSemaphore, 1, NULL);
	} else {
//Error Log
	}

	return bRet;
}


/*========================================================================================================
  데이터를 수신하는 Thread
========================================================================================================*/
UINT Receive232Thread(LPVOID wndParam)
{
	CWinRs232c*	WinRs232c = (CWinRs232c*)wndParam;

	DWORD	nEvtMask = 0;

	while (TRUE)
	{
		::WaitCommEvent(WinRs232c->m_Handle, &nEvtMask, NULL);
		if ((nEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			do
			{
				COMSTAT	ComStat;
				DWORD	nError;
				if (::ClearCommError(WinRs232c->m_Handle, &nError, &ComStat))
				{
					DWORD	nLength = WinRs232c->m_nRcvMaxLen;

					Sleep(100);
					if (::ReadFile(WinRs232c->m_Handle, WinRs232c->m_Data, nLength, &WinRs232c->m_RcvLen, &WinRs232c->m_ioRead))
					{
						if (WinRs232c->m_RcvLen && WinRs232c->m_pParent) {
							char szData[MAX_BUFF] = {0};
							WinRs232c->Receive((LPVOID) szData);
							WinRs232c->m_fnParentCallBack(WinRs232c->m_pParent, (LPARAM) szData, (WPARAM)WinRs232c->m_RcvLen);
						}
					}
				}
			} while(WinRs232c->m_RcvLen);
		}
	}

	return TRUE;
}

/*========================================================================================================

========================================================================================================*/
BOOL CWinRs232c::m_fnInitial(int nPort, 
	                         DWORD nRate, 
							 BYTE nParity , 
							 BYTE nSize, 
							 BYTE nStop, 
							 char *szLogFileName, 
							 int nLogPeriod, 
	                         void (*FN_PARENT_CALLBACK)(CWnd* pWnd, LPARAM lParam, WPARAM wParam))
{
	CString strData;
	
//	strData = szLogFileName;
//	m_Logger.SetFileName(strData);
	//m_Logger.m_fnSetLogSaveDays(nLogPeriod);						// By mjna [141104] Log Save Days 지정하지 않는 버그 수정. (4)

	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength= sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;

	m_hFModuleSemaphore = CreateSemaphore(&sa, 1L, 1L, SEMAPHORE_NAME);

	m_fnParentCallBack = FN_PARENT_CALLBACK;

	if(nPort < 10) {
		strData.Format("COM%d", nPort);
	} else {
		strData.Format("//./COM%d", nPort);
	}

	BOOL bOpen = Open(strData, nRate, nParity, nSize, nStop);
	
	if(bOpen) {
// 		strData.Format("COM %d Open Success", nPort);
// 		m_Logger.SetLogString(strData);
	} else {
//		strData.Format("COM %d Open Fail", nPort);
//		m_Logger.SetLogString(strData);
	}

	return bOpen;
}

/*========================================================================================================

========================================================================================================*/
int CWinRs232c::m_fnGetDTR()
{
	DCB	Dcb;
	
	Dcb.DCBlength = sizeof(Dcb);
	if (::GetCommState(m_Handle, &Dcb) == FALSE)
		return -1;
	
	return Dcb.fDtrControl;
}

/*========================================================================================================

========================================================================================================*/
int CWinRs232c::m_fnSetDTR(int nMode)
{
	int nRet=0;
	DCB	Dcb;

	Dcb.DCBlength = sizeof(Dcb);
	if (::GetCommState(m_Handle, &Dcb) == FALSE)
		return -1;

	Dcb.fDtrControl = nMode;

	if (::SetCommState(m_Handle, &Dcb) == FALSE)
	{
		int nErr = ::GetLastError();
		::CloseHandle(m_Handle);
		return -2;
	}

	return nRet;
}
